#include "TAircraft.h"
#define _k_intg_pitch 0.0005
#define _k_intg_roll 0.01
#define idStartASP 1

void thread_work(TClient* Client, microseconds delay, int workTime) {
	double time = 0;
	time_point<steady_clock> Tend = steady_clock::now(), Tbegin = steady_clock::now();
	while (true) {
		time = duration<double>(Tend - Tbegin).count();
		Client->Working();
		Tend = steady_clock::now();
		this_thread::sleep_for(delay);
	}
}

void thread_send(TClient* Client, milliseconds delay, int workTime) {
	double time = 0; time_point<steady_clock> Tend = steady_clock::now(), Tbegin = steady_clock::now();
	while (true) {
		time = duration<double>(Tend - Tbegin).count();
		Client->Sending();
		Tend = steady_clock::now();
		this_thread::sleep_for(delay);
	}
}

TAircraft::TAircraft(double Latitude, double Longitude, double Course, double Pitch, double Roll, string FilePPMs, double TakeOffVel, double demVelocity, double demHeight) {
	this->initLatitude = Latitude;
	this->curLatitude = Latitude;
	this->initLongitude = Longitude;
	this->curLongitude = Longitude;
	this->curCourse = Course;
	this->curPitch = Pitch;
	this->curRoll = Roll;
	this->TakeOffVel = TakeOffVel;
	this->demVelocity = demVelocity;
	this->demHeight = demHeight;
	vector<double> temp = Geo2Top(R, Latitude * Deg2Rad, Longitude * Deg2Rad);
	x = temp[0];
	y = temp[1];
	z = temp[2];
	OPS = new TOPS(FilePPMs, Latitude, Longitude);
	ASP = new TASP(Latitude, Longitude);
	log_file.open("../Log_from_aircraft.txt");
}

void TAircraft::StartAircraft() {

	this->TServer::set_port(12346);
	this->TServer::Creat_Socket();

	INS = new TINS();
	INS->set_port(12346);
	INS->set_ipaddres("127.0.0.1");
	INS->Creat_Socket();
	INS->setData(curLongitude, curLatitude, 0, curCourse, 0, 0, 0);

	SNS = new TSNS();
	SNS->set_port(12346);
	SNS->set_ipaddres("127.0.0.1");
	SNS->Creat_Socket();
	SNS->setData(0, curCourse, curLatitude, curLongitude, 0);

	thread tWorkINS(thread_work, INS, microseconds(2500), 300);
	thread tSendIns(thread_send, INS, milliseconds(10), 300);

	thread tWorkSNS(thread_work, SNS, microseconds(100000), 300);
	thread tSendSNS(thread_send, SNS, milliseconds(1000), 300);

	thread tWorkOPS(thread_work, OPS, microseconds(1000), 300);
	thread tWorkASP(thread_work, ASP, microseconds(1000), 300);
	thread tWorkAircraft(thread_work, this, microseconds(1000), 300);

	prev_time = steady_clock::now();
	time_begin = steady_clock::now();

	tWorkAircraft.join();	tWorkOPS.join();	tWorkASP.join();
	tWorkINS.join();		tWorkSNS.join();
	tSendIns.join();		tSendSNS.join();

	INS->Clean_n_Close();
	SNS->Clean_n_Close();

}

void TAircraft::Working() {
	time_point<steady_clock> timeLabel = steady_clock::now();
	double dt = duration<double>(timeLabel - prev_time).count();
	prev_time = steady_clock::now();
	BNR Message;
	StatucWordINS SW_INS;
	SRNS SW_SNS;
	double _intg_pitch_input = 0,
		_intg_roll_input = 0;
	double a_x = 0, a_y = 0, a_z = 0;
	switch (_fs){
	case TAircraft::STAGE_IDLE:

		Message = this->Receive();

		mutex.lock();
		if (Message.Value.Label == 184) {
			memset(&SW_INS, 0, sizeof(StatucWordINS));
			memcpy(&SW_INS, &Message, sizeof(Message));
			if (SW_INS.Value.Ready == 1)
				f_ins_ready = true;
		}
		else if (Message.Value.Label == 187) {
			memset(&SW_SNS, 0, sizeof(SRNS));
			memcpy(&SW_SNS, &Message, sizeof(Message));
			if (SW_SNS.Value.OperatingSubMode == 0)
				f_sns_ready = true;
		}

		if (f_ins_ready && f_sns_ready)
			_fs = FlightStage::STAGE_TAKEOFF;
		curVelocity = 0;
		mutex.unlock();
		break;

	case TAircraft::STAGE_TAKEOFF:
		mutex.lock();
		curVelocity = TakeOffVel;
		if (abs(demHeight - curHeight) <= 100) {
			_fs = FlightStage::STAGE_FLIGHT;
			idPPM = OPS->get_idPPM();
			f_pitch_clear = true;
		}
		//”правление тангажом
		_intg_pitch_input = (demHeight - curHeight) * _k_intg_pitch;
		a_y = -g / m;
		mutex.unlock();
		break;

	case TAircraft::STAGE_FLIGHT:
		mutex.lock();
		//—брос текущего тангажа при выходе на заданную высоту
		if (f_pitch_clear) {
			curPitch = 0; 
			_intg_pitch_prev = 0;
			f_pitch_clear = false;
		}
		//—брос текущего крена при выходе на заданный курс
		if (abs(OPS->getCourse() * Rad2Deg - curCourse) <= 1)
			f_roll_clear = true;
		if (f_roll_clear) {
			curRoll = 0;
			_intg_roll_prev = 0;
			f_roll_clear = false;
		}
		//”правление тангажом
		curPitch += (demHeight - curHeight) * 0.005;
		_intg_pitch_input = (demHeight - curHeight) * _k_intg_pitch;
		//”правдение креном
		_intg_roll_input = (OPS->getCourse() * Rad2Deg - curCourse) * _k_intg_roll;
		curVelocity = demVelocity;
		a_y = -g / m;
		mutex.unlock();
		break;
	}
	mutex.lock();

	//”правление тангажом
	curPitch += (_intg_pitch_input + _intg_pitch_prev) * dt;
	curPitch = saturation(curPitch, -45, 45);
	_intg_pitch_prev = _intg_pitch_input;

	//”правление креном
	curRoll += (_intg_roll_input + _intg_roll_prev) * dt;
	curRoll = saturation(curRoll, -180, 180);
	_intg_roll_prev = _intg_roll_input;

	//”правление курсом
	curCourse += 0.1 * curRoll;
	curCourse = saturation(curCourse, -180, 180);

	curV_x += a_x * dt;
	curV_y += a_y * dt;
	curV_z += a_z * dt;

	x += (curV_x + curVelocity * cos(curCourse * Deg2Rad) * cos(curPitch * Deg2Rad)) * dt;
	y += (curV_y + curVelocity * sin(curPitch * Deg2Rad)) * dt;
	z += (curV_z + curVelocity * sin(curCourse * Deg2Rad) * cos(curPitch * Deg2Rad)) * dt;
	
	vector<double> temp = Top2Geo(x, y, z, initLatitude * Deg2Rad, initLongitude * Deg2Rad);
	curHeight = temp[0] - R;
	//¬несение данных в аппаратуру
	INS->setData(temp[2] * Rad2Deg, temp[1] * Rad2Deg, temp[0] - R, curCourse * Deg2Rad, curPitch * Deg2Rad, curRoll * Deg2Rad, curVelocity);
	SNS->setData(temp[0] - R, curCourse, temp[1] * Rad2Deg, temp[2] * Rad2Deg, curVelocity * sin(curPitch * Deg2Rad));
	OPS->setData(temp[1], temp[2], curHeight, curPitch * Deg2Rad, curRoll * Deg2Rad, curCourse * Deg2Rad);
	Log(temp);
	if ((OPS->get_idPPM() == idStartASP) && !f_ASP_launched) {
		ASP->Start_ASP({ x, y, z, curVelocity}, { curPitch, curCourse }, { temp[1] * Rad2Deg , temp[2] * Rad2Deg , curHeight });
		f_ASP_launched = true;
	}

	mutex.unlock();
}

void TAircraft::Log(vector <double> temp) {
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	log_file << "\n“екущее врем€: " << asctime(localtime(&in_time_t)) << "\tFS: " << _fs << endl;
	log_file << "curPitch: " << curPitch << "\tcurRoll: " << curRoll << "\tcurCourse: " << curCourse << "\tdemCourse: " << OPS->getCourse() * Rad2Deg <<  endl;
	log_file << OPS->get_LatPPM() << "\t" << OPS->get_LngPPM() << "\tidPPMs: " << OPS->get_idPPM() << "\tS to PPM: " << OPS->get_S() << endl;
	log_file << temp[1] * Rad2Deg << "\t" << temp[2] * Rad2Deg << "\t" << temp[0] - R << endl;
}

void TAircraft::Sending() {

}