#include "TINS.h"
#define adr_SW_INS 184
#define adr_Lat 200
#define adr_Lng 201
#define adr_H 241
#define adr_C_True 204
#define adr_Pitch 212
#define adr_Roll 213
#define adr_Vn 246
#define adr_Ve 247
#define adr_Vy 245
#define adr_Ax 217
#define adr_Ay 218
#define adr_Az 219

void TINS::Init() {
	Latitude = P_INFO("Latitude", 20, 90);			Latitude.set_Label(adr_Lat);
	Longitude = P_INFO("Longitude", 20, 90);		Longitude.set_Label(adr_Lng);
	Height = P_INFO("Height", 19, 19975.3728);		Height.set_Label(adr_H);
	trueCourse = P_INFO("True course", 16, 90);		trueCourse.set_Label(adr_C_True);
	Pitch = P_INFO("Pitch", 16, 90);				Pitch.set_Label(adr_Pitch);
	Roll = P_INFO("Roll", 16, 90);					Roll.set_Label(adr_Roll);
	Vn = P_INFO("Velocity north", 19, 1053.5822);	Vn.set_Label(adr_Vn);
	Ve = P_INFO("Velocity east", 19, 1053.5822);	Ve.set_Label(adr_Ve);
	Vy = P_INFO("Velocity vertical", 19, 83.2307);	Vy.set_Label(adr_Vy);
	Ax = P_INFO("Acceleration X", 12, 19.62);		Ax.set_Label(adr_Ax);
	Ay = P_INFO("Acceleration Y", 12, 19.62);		Ay.set_Label(adr_Ay);
	Az = P_INFO("Acceleration Z", 12, 19.62);		Az.set_Label(adr_Az);
	paketiki.resize(13);
	for (int i = 0; i < paketiki.size(); i++) {
		paketiki[i] = new char(sizeof(BNR));
		memset(paketiki[i], 0, sizeof(BNR));
	}
	fLog.open("../Log_from_INS.txt");
}

void TINS::Log() {
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);
	fLog << "\nТекущее время: " << asctime(localtime(&in_time_t));

	SW_INS_log = SW_INS.Value.P;
	SW_INS_log = SW_INS_log << 2; SW_INS_log += SW_INS.Value.SM;
	SW_INS_log = SW_INS_log << 3; SW_INS_log += SW_INS.Value.RSVD2;
	SW_INS_log = SW_INS_log << 1; SW_INS_log += SW_INS.Value.Ready;
	SW_INS_log = SW_INS_log << 1; SW_INS_log += SW_INS.Value.ReadyAcceleration;
	SW_INS_log = SW_INS_log << 1; SW_INS_log += SW_INS.Value.ServiceabilityINS;
	SW_INS_log = SW_INS_log << 1; SW_INS_log += SW_INS.Value.NoHeightReception;
	SW_INS_log = SW_INS_log << 1; SW_INS_log += SW_INS.Value.NoInitialData;
	SW_INS_log = SW_INS_log << 1; SW_INS_log += SW_INS.Value.TemperatureControl;
	SW_INS_log = SW_INS_log << 1; SW_INS_log += SW_INS.Value.ServiceabilityHeating;
	SW_INS_log = SW_INS_log << 3; SW_INS_log += SW_INS.Value.PreparationScale;
	SW_INS_log = SW_INS_log << 1; SW_INS_log += SW_INS.Value.Restart;
	SW_INS_log = SW_INS_log << 1; SW_INS_log += SW_INS.Value.RSVD1;
	SW_INS_log = SW_INS_log << 1; SW_INS_log += SW_INS.Value.Gyrocompassing;
	SW_INS_log = SW_INS_log << 1; SW_INS_log += SW_INS.Value.Navigation;
	SW_INS_log = SW_INS_log << 1; SW_INS_log += SW_INS.Value.Control;
	SW_INS_log = SW_INS_log << 1; SW_INS_log += SW_INS.Value.Preparation_ZK;
	SW_INS_log = SW_INS_log << 2; SW_INS_log += SW_INS.Value.SDI;
	SW_INS_log = SW_INS_log << 8; SW_INS_log += SW_INS.Value.Label;
	fLog << "Слово состояния ИНС: " << SW_INS_log << endl;
	fLog << "Широта: " << Latitude.get_Value() << endl;
	fLog << "Долгота: " << Longitude.get_Value() << endl;
	fLog << "Высота: " << Height.get_Value() << endl;
	fLog << "Курс истинный: " << trueCourse.get_Value() << endl;
	fLog << "Угол тангажа: " << Pitch.get_Value() << endl;
	fLog << "Угол крена: " << Roll.get_Value() << endl;
	fLog << "Скорость Север/Юг: " << Vn.get_Value() << endl;
	fLog << "Скорость Восток/Запад: " << Ve.get_Value() << endl;
	fLog << "Скорость вертикальная инерациальная: " << Vy.get_Value() << endl;
	fLog << "Ускорение продольное, ax: " << Ax.get_Value() << endl;
	fLog << "Усорение поперечное, az: " << Ay.get_Value() << endl;
	fLog << "Ускорение нормальное, ay: " << Az.get_Value() << endl;

}

void TINS::Working(){
	time_point<steady_clock> timeLabel = steady_clock::now();
	int ready = 0;
	double cur_t;
	switch (_OperatingMode) {
	case TINS::MODE_STANDBY:
		Init();
		prevTime = timeLabel;
		startTime = timeLabel;
		_OperatingMode = OperatingMode::MODE_TURNON;
		cout <<"\nAt: "<< duration<double>(timeLabel - startTime).count() << "\tTurnOn INS\n";
		break;
	case TINS::MODE_TURNON:
		if (duration<double>(timeLabel - prevTime).count() >= 5) {			//20 seconds
			cout << "\nAt: " << duration<double>(timeLabel - startTime).count() << "\tINS - Start preparing\n";
			prevTime = timeLabel;
			_OperatingMode = OperatingMode::MODE_PREPARING;
		}
		break;
	case TINS::MODE_PREPARING:
		if ((int)(duration<double>(timeLabel - prevTime).count() / 20) - count == 1)
			count >= 7 ? count = 7 : count++;

		if (duration<double>(timeLabel - prevTime).count() >= 12) {			//120 seconds
			ready = 1;
			cout << "\nAt: " << duration<double>(timeLabel - startTime).count() << "\tINS - Start mode Navigation\n";
			prevTime = timeLabel;
			
			_OperatingMode = OperatingMode::MODE_NAVIGATION;
		}

		mutex.lock();

		SW_INS.Value.Label = adr_SW_INS;			//4
		SW_INS.Value.SDI = 1;						//1
		SW_INS.Value.Preparation_ZK = 0;
		SW_INS.Value.Control = 0;
		SW_INS.Value.Navigation = ready;			//0, 1
		SW_INS.Value.Gyrocompassing = 0;
		SW_INS.Value.RSVD1 = 0;
		SW_INS.Value.Restart = 0;
		SW_INS.Value.PreparationScale = count;		//0, 1, 2, 3
		SW_INS.Value.ServiceabilityHeating = 1;		//1
		SW_INS.Value.TemperatureControl = 1;		//1
		SW_INS.Value.NoInitialData = 1;				//1
		SW_INS.Value.NoHeightReception = 0;
		SW_INS.Value.ServiceabilityINS = 1;			//1
		SW_INS.Value.ReadyAcceleration = 0;
		SW_INS.Value.Ready = ready;					//0, 1
		SW_INS.Value.RSVD2 = 0;
		SW_INS.Value.SM = 0;
		SW_INS.Value.P = ((9 +
			(count == 0 ? 0 :
				((count == 1) || (count == 2) || (count == 4) ? 1 :
					((count == 3) || (count == 5) || (count == 6) ? 2 : 3)))) % 2) == 0 ? 1 : 0;

		memcpy(paketiki[0], &SW_INS, sizeof(StatucWordINS));

		mutex.unlock();

		break;
	case TINS::MODE_NAVIGATION:
		cur_t = duration<double>(timeLabel - startTime).count();
		f_data_ready_to_send = false;
		//Вычисление параметров движения

		mutex.lock();
		
		//Формирование пакета с данными широты
		lat.Value.Label =	Latitude.get_Label();
		lat.Value.SDI =		Latitude.get_SDI();
		lat.Value.D =		Latitude.get_binValue();
		lat.Value.SSM =		Latitude.get_SSM();
		lat.Value.P	=		Latitude.get_P();

		memcpy(paketiki[1], &lat, sizeof(BNR));

		//Формирование пакета с данными долготы
		lng.Value.Label =	Longitude.get_Label();
		lng.Value.SDI =		Longitude.get_SDI();
		lng.Value.D =		Longitude.get_binValue();
		lng.Value.SSM =		Longitude.get_SSM();
		lng.Value.P =		Longitude.get_P();

		memcpy(paketiki[2], &lng, sizeof(BNR));

		//Формирование пакета с данными высоты
		h.Value.Label =		Height.get_Label();
		h.Value.SDI =		Height.get_SDI();
		h.Value.D =			Height.get_binValue();
		h.Value.SSM =		Height.get_SSM();
		h.Value.P =			Height.get_P();
		
		memcpy(paketiki[3], &h, sizeof(BNR));

		//Формирование пакета с данными истинного курса
		c_true.Value.Label =	trueCourse.get_Label();
		c_true.Value.SDI =		trueCourse.get_SDI();
		c_true.Value.D =		trueCourse.get_binValue();
		c_true.Value.SSM =		trueCourse.get_SSM();
		c_true.Value.P =		trueCourse.get_P();

		memcpy(paketiki[4], &c_true, sizeof(BNR));

		//Формирование пакета с данными тангажа
		pitch.Value.Label =	Pitch.get_Label();
		pitch.Value.SDI =	Pitch.get_SDI();
		pitch.Value.D =		Pitch.get_binValue();
		pitch.Value.SSM =	Pitch.get_SSM();
		pitch.Value.P =		Pitch.get_P();

		memcpy(paketiki[5], &pitch, sizeof(BNR));

		//Формирование пакета с данными крена
		roll.Value.Label =	Roll.get_Label();
		roll.Value.SDI =	Roll.get_SDI();
		roll.Value.D =		Roll.get_binValue();
		roll.Value.SSM =	Roll.get_SSM();
		roll.Value.P =		Roll.get_P();

		memcpy(paketiki[6], &roll, sizeof(BNR));

		//Формирование пакета с данными северной скорости
		vn.Value.Label =	Vn.get_Label();
		vn.Value.SDI =		Vn.get_SDI();
		vn.Value.D =		Vn.get_binValue();
		vn.Value.SSM =		Vn.get_SSM();
		vn.Value.P =		Vn.get_P();

		memcpy(paketiki[7], &vn, sizeof(BNR));

		//Формирование пакета с данными восточной скорости
		ve.Value.Label =	Ve.get_Label();
		ve.Value.SDI =		Ve.get_SDI();
		ve.Value.D =		Ve.get_binValue();
		ve.Value.SSM =		Ve.get_SSM();
		ve.Value.P =		Ve.get_P();

		memcpy(paketiki[8], &ve, sizeof(BNR));

		//Формирование пакета с данными вертикальной скорости
		vy.Value.Label =	Vy.get_Label();
		vy.Value.SDI =		Vy.get_SDI();
		vy.Value.D =		Vy.get_binValue();
		vy.Value.SSM =		Vy.get_SSM();
		vy.Value.P =		Vy.get_P();

		memcpy(paketiki[9], &vy, sizeof(BNR));

		//Формирование пакета с данными продольного ускорения
		ax.Value.Label =	Ax.get_Label();
		ax.Value.SDI =		Ax.get_SDI();
		ax.Value.D =		Ax.get_binValue();
		ax.Value.SSM =		Ax.get_SSM();
		ax.Value.P =		Ax.get_P();

		memcpy(paketiki[10], &ax, sizeof(BNR));

		//Формирование пакета с данными вертикального ускорения
		ay.Value.Label =	Ay.get_Label();
		ay.Value.SDI =		Ay.get_SDI();
		ay.Value.D =		Ay.get_binValue();
		ay.Value.SSM =		Ay.get_SSM();
		ay.Value.P =		Ay.get_P();

		memcpy(paketiki[11], &ay, sizeof(BNR));

		//Формирование пакета с данными бокового ускорения
		az.Value.Label =	Az.get_Label();
		az.Value.SDI =		Az.get_SDI();
		az.Value.D =		Az.get_binValue();
		az.Value.SSM =		Az.get_SSM();
		az.Value.P =		Az.get_P();

		memcpy(paketiki[12], &az, sizeof(BNR));

		//Формирование пакета с данными признаков ИНС
		SW_INS.Value.Label = adr_SW_INS;			//4
		SW_INS.Value.SDI = 1;						//1
		SW_INS.Value.Preparation_ZK = 0;
		SW_INS.Value.Control = 0;
		SW_INS.Value.Navigation = 1;				//1
		SW_INS.Value.Gyrocompassing = 0;
		SW_INS.Value.RSVD1 = 0;
		SW_INS.Value.Restart = 0;
		SW_INS.Value.PreparationScale = 9;			//3
		SW_INS.Value.ServiceabilityHeating = 1;		//1
		SW_INS.Value.TemperatureControl = 1;		//1
		SW_INS.Value.NoInitialData = 1;				//1
		SW_INS.Value.NoHeightReception = 0;
		SW_INS.Value.ServiceabilityINS = 1;			//1
		SW_INS.Value.ReadyAcceleration = 0;
		SW_INS.Value.Ready = 1;					//1
		SW_INS.Value.RSVD2 = 0;
		SW_INS.Value.SM = 0;
		SW_INS.Value.P = 1;							//14 ед. => P = 1

		memcpy(paketiki[0], &SW_INS, sizeof(StatucWordINS));
		f_data_ready_to_send = true;
		mutex.unlock();

		break;
	case TINS::MODE_RESTART:
		break;
	case TINS::MODE_CONTROL:
		break;
	default:
		cout << "\nAt: "<< duration<double>(timeLabel-startTime).count() <<"\tINS - Somthing went wrong\n";
		break;
	}
	Log();
}

void TINS::Sending() {
	switch (_OperatingMode){
	case TINS::MODE_PREPARING:
		TClient::Send(paketiki[0]);
		break;
	case TINS::MODE_NAVIGATION:
		if(f_data_ready_to_send)
			for(int i = 0; i < paketiki.size(); i++)
				TClient::Send(paketiki[i]);
		break;
	default:
		break;
	}
}

void TINS::setData(double Longitude, double Latitude, double Height, double trueCourse, double Pitch, double Roll, double V) {
	mutex.lock();
	
	this->Longitude = Longitude;
	this->Latitude = Latitude;
	this->Height = Height;
	this->trueCourse = trueCourse;
	this->Pitch = Pitch;
	this->Roll = Roll;
	this->Vn = V * cos(trueCourse) * cos(Pitch);
	this->Ve = V * sin(trueCourse) * cos(Pitch);
	this->Vy = V * sin(Pitch);
	this->Ax = 0;
	this->Ay = 0;
	this->Az = 0;

	mutex.unlock();
}