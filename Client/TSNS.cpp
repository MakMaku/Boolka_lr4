#include "TSNS.h"
#define adr_Height 62
#define adr_HDOP 65
#define adr_VDOP 66
#define adr_WayAngle 67
#define adr_Latitude 72
#define adr_Latitude_tchn 80
#define adr_Longitude 73
#define adr_Longitude_tchn 81
#define adr_Zadergka 75
#define adr_UTC_st 104
#define adr_UTC_ml 96
#define adr_Vh 117
#define adr_Date 176
#define adr_SW_SNS 187

void TSNS::Working() {
	auto new_now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(new_now);
	string data_time = asctime(localtime(&in_time_t));

	int work = 1;
	double cur_t = 0;

	time_point<steady_clock> timeLabel = steady_clock::now();
	switch (_OperatingMode){
	case TSNS::MODE_TURNON:
		Init();
		prevTime = timeLabel;
		startTime = timeLabel;
		_OperatingMode = OperatingMode::MODE_CONTROL;
		cout << "\nAt: " << duration<double>(timeLabel - startTime).count() << "\tSNS - TurnON and start mode Control:\n";
		break;
	case TSNS::MODE_CONTROL:
		if (duration<double>(timeLabel - prevTime).count() >= 5) {		//20 seconds

			cout << "\nAt: " << duration<double>(timeLabel - startTime).count() << "\tSNS - End mode Control and start mode Synchronization\n";
			prevTime = timeLabel;
			_OperatingMode = OperatingMode::MODE_SYNCHRONIZATION;
		}

		break;
	case TSNS::MODE_SYNCHRONIZATION:
		if (duration<double>(timeLabel - prevTime).count() >= 12) {		//120 seconds
			work = 0;
			cout << "\nAt: " << duration<double>(timeLabel - startTime).count() << "\tSNS - End mode Synchronization and start mode Navigation\n";
			prevTime = timeLabel;
			_OperatingMode = OperatingMode::MODE_NAVIGATION;
		}

		mutex.lock();

		SW_SNS.Value.Label = adr_SW_SNS;		//10111011 -> 6
		SW_SNS.Value.RequestInitalData = 1;		//1
		SW_SNS.Value.TypeSRNS = 0;
		SW_SNS.Value.Almanac_GPS = 0;
		SW_SNS.Value.Almanac_GLONASS = 0;
		SW_SNS.Value.OperatingMode = 2;			//1
		SW_SNS.Value.OperatingSubMode = work;	//1, 0
		SW_SNS.Value.TimeInd = 0;
		SW_SNS.Value.DiffMode = 0;
		SW_SNS.Value.Failure = 0;
		SW_SNS.Value.AlamThreshold = 0;
		SW_SNS.Value.ReferenceFrame = 0;
		SW_SNS.Value.StatusMatrix = 0;
		SW_SNS.Value.P = ((8 + work) % 2) == 0 ? 1 : 0;

		memcpy(paketiki[0], &SW_SNS, sizeof(SRNS));

		mutex.unlock();

		break;
	case TSNS::MODE_NAVIGATION:
		cur_t = duration<double>(timeLabel - startTime).count();

		f_data_ready_to_send = false;
		HDOP = 125;
		VDOP = 100;
		Latitude_tchn = 0.0000858 * sin(cur_t / 57.3);
		Longitude_tchn = 0.0000858 * cos(cur_t / 57.3);
		Zadergka = 78;
		_time_UTC_h = stoi(data_time.substr(11, 2));
		_time_UTC_m = stoi(data_time.substr(14, 2));
		_time_UTC_s = stoi(data_time.substr(17, 2));
		time_UTC_ml = 251;
		mutex.lock();

		//Пакет слова состояния СНС
		SW_SNS.Value.Label = adr_SW_SNS;		//6
		SW_SNS.Value.RequestInitalData = 1;		//1
		SW_SNS.Value.TypeSRNS = 0;
		SW_SNS.Value.Almanac_GPS = 0;
		SW_SNS.Value.Almanac_GLONASS = 0;
		SW_SNS.Value.OperatingMode = 2;			//1
		SW_SNS.Value.OperatingSubMode = 0;
		SW_SNS.Value.TimeInd = 0;
		SW_SNS.Value.DiffMode = 0;
		SW_SNS.Value.Failure = 0;
		SW_SNS.Value.AlamThreshold = 0;
		SW_SNS.Value.ReferenceFrame = 0;
		SW_SNS.Value.StatusMatrix = 0;
		SW_SNS.Value.P = 1;

		memcpy(paketiki[0], &SW_SNS, sizeof(SRNS));

		//Пакет "ДАТА"
		_Date.Value.Label = adr_Date; 
		_Date.Value.RSVD = 0;
		_Date.Value.Day_ed = stoi(data_time.substr(8, 2)) % 10;
		_Date.Value.Day_de = (int)(stoi(data_time.substr(8, 2)) / 10);
		_Date.Value.Month_ed = get_numMonth(data_time.substr(4, 3)) % 10;
		_Date.Value.Month_de = (int)(get_numMonth(data_time.substr(4, 3)) / 10);
		_Date.Value.Year_ed = stoi(data_time.substr(22, 2)) % 10;
		_Date.Value.Year_de = (int)(stoi(data_time.substr(22, 2)) / 10);
		_Date.Value.StatusMatrix = 0;
		_Date.Value.P = 0;

		memcpy(paketiki[1], &_Date, sizeof(_Date));
		
		//Пакет высоты
		_Height.Value.Label = Height.get_Label();
		_Height.Value.SDI = Height.get_SDI();
		_Height.Value.D = Height.get_binValue();
		_Height.Value.SSM = Height.get_SSM();
		_Height.Value.P = Height.get_P();

		memcpy(paketiki[2], &_Height, sizeof(_Height));

		//Пакет HDOP
		_HDOP.Value.Label = HDOP.get_Label();
		_HDOP.Value.SDI = HDOP.get_SDI();
		_HDOP.Value.D = HDOP.get_binValue();
		_HDOP.Value.SSM = HDOP.get_SSM();
		_HDOP.Value.P = HDOP.get_P();

		memcpy(paketiki[3], &_HDOP, sizeof(_HDOP));

		//Пакет VDOP
		_VDOP.Value.Label = VDOP.get_Label();
		_VDOP.Value.SDI = VDOP.get_SDI();
		_VDOP.Value.D = VDOP.get_binValue();
		_VDOP.Value.SSM = VDOP.get_SSM();
		_VDOP.Value.P = VDOP.get_P();

		memcpy(paketiki[4], &_VDOP, sizeof(_VDOP));

		//Пакет WayAngle
		_WayAngle.Value.Label = WayAngle.get_Label();
		_WayAngle.Value.SDI = WayAngle.get_SDI();
		_WayAngle.Value.D = WayAngle.get_binValue();
		_WayAngle.Value.SSM = WayAngle.get_SSM();
		_WayAngle.Value.P = WayAngle.get_P();

		memcpy(paketiki[5], &_WayAngle, sizeof(_WayAngle));

		//Пакет Latitude
		_Latitude.Value.Label = Latitude.get_Label();
		_Latitude.Value.SDI = Latitude.get_SDI();
		_Latitude.Value.D = Latitude.get_binValue();
		_Latitude.Value.SSM = Latitude.get_SSM();
		_Latitude.Value.P = Latitude.get_P();

		memcpy(paketiki[6], &_Latitude, sizeof(_Latitude));

		//Пакет Latitude_tchn
		_Latitude_tchn.Value.Label = Latitude_tchn.get_Label();
		_Latitude_tchn.Value.SDI = Latitude_tchn.get_SDI();
		_Latitude_tchn.Value.D = Latitude_tchn.get_binValue();
		_Latitude_tchn.Value.SSM = Latitude_tchn.get_SSM();
		_Latitude_tchn.Value.P = Latitude_tchn.get_P();

		memcpy(paketiki[7], &_Latitude_tchn, sizeof(_Latitude_tchn));

		//Пакет Longitude
		_Longitude.Value.Label = Longitude.get_Label();
		_Longitude.Value.SDI = Longitude.get_SDI();
		_Longitude.Value.D = Longitude.get_binValue();
		_Longitude.Value.SSM = Longitude.get_SSM();
		_Longitude.Value.P = Longitude.get_P();

		memcpy(paketiki[8], &_Longitude, sizeof(_Longitude));

		//Пакет Longitude_tchn
		_Longitude_tchn.Value.Label = Longitude_tchn.get_Label();
		_Longitude_tchn.Value.SDI = Longitude_tchn.get_SDI();
		_Longitude_tchn.Value.D = Longitude_tchn.get_binValue();
		_Longitude_tchn.Value.SSM = Longitude_tchn.get_SSM();
		_Longitude_tchn.Value.P = Longitude_tchn.get_P();

		memcpy(paketiki[9], &_Longitude_tchn, sizeof(_Longitude_tchn));

		//Пакет Zadergka
		_Zadergka.Value.Label = Zadergka.get_Label();
		_Zadergka.Value.SDI = Zadergka.get_SDI();
		_Zadergka.Value.D = Zadergka.get_binValue();
		_Zadergka.Value.SSM = Zadergka.get_SSM();
		_Zadergka.Value.P = Zadergka.get_P();

		memcpy(paketiki[10], &_Zadergka, sizeof(_Zadergka));

		//Пакет time_UTC_st
		_time_UTC_st.Value.Label = _time_UTC_h.get_Label();
		_time_UTC_st.Value.SDI = 0;
		_time_UTC_st.Value.D = _time_UTC_h.get_binValue();							//hhhhh
		_time_UTC_st.Value.D = _time_UTC_st.Value.D << _time_UTC_m.get_NumBit();	//hhhhh000000
		_time_UTC_st.Value.D = _time_UTC_st.Value.D + _time_UTC_m.get_binValue();	//hhhhhmmmmmm
		_time_UTC_st.Value.D = _time_UTC_st.Value.D << _time_UTC_s.get_NumBit();	//hhhhhmmmmmm000000
		_time_UTC_st.Value.D = _time_UTC_st.Value.D + _time_UTC_s.get_binValue();	//hhhhhmmmmmmssssss
		_time_UTC_st.Value.SSM = 0;
		_time_UTC_st.Value.P = ((_time_UTC_s.get_P() + _time_UTC_m.get_P() + _time_UTC_h.get_P()) % 2) == 0 ? 1 : 0;


		memcpy(paketiki[11], &_time_UTC_st, sizeof(_time_UTC_st));

		//Пакет time_UTC_ml
		_time_UTC_ml.Value.Label = time_UTC_ml.get_Label();
		_time_UTC_ml.Value.SDI = time_UTC_ml.get_SDI();
		_time_UTC_ml.Value.D = time_UTC_ml.get_binValue();
		_time_UTC_ml.Value.SSM = time_UTC_ml.get_SSM();
		_time_UTC_ml.Value.P = time_UTC_ml.get_P();

		memcpy(paketiki[12], &_time_UTC_ml, sizeof(_time_UTC_ml));

		//Пакет Vh
		_Vh.Value.Label = Vh.get_Label();
		_Vh.Value.SDI = Vh.get_SDI();
		_Vh.Value.D = Vh.get_binValue();
		_Vh.Value.SSM = Vh.get_SSM();
		_Vh.Value.P = Vh.get_P();

		memcpy(paketiki[13], &_Vh, sizeof(_Vh));
		f_data_ready_to_send = true;
		mutex.unlock();

		break;
	case TSNS::MODE_DIFNAVIGATION:
		break;
	default:
		cout << "\nAt: " << duration<double>(timeLabel - startTime).count() << "\tSNS - Somthing went wrong\n";
		break;
	}
	Log();
}

void TSNS::Sending() {
	switch (_OperatingMode){
	case TSNS::MODE_SYNCHRONIZATION:
		TClient::Send(paketiki[0]);
		break;
	case TSNS::MODE_NAVIGATION:
		if(f_data_ready_to_send)
			for (int i = 0; i < paketiki.size(); i++)
				TClient::Send(paketiki[i]);
		break;
	default:
		break;
	}
}
void TSNS::Init() {
	Height = P_INFO("Height", 20, 65536);							Height.set_Label(adr_Height);
	HDOP = P_INFO("HDOP", 15, 512);									HDOP.set_Label(adr_HDOP);
	VDOP = P_INFO("VDOP", 15, 512);									VDOP.set_Label(adr_VDOP);
	WayAngle = P_INFO("WayAngle", 15, 90);							WayAngle.set_Label(adr_WayAngle);
	Latitude = P_INFO("Latitude", 20, 90);							Latitude.set_Label(adr_Latitude);
	Latitude_tchn = P_INFO("Latitude_tchn", 11, 0.000858);		Latitude_tchn.set_Label(adr_Latitude_tchn);
	Longitude = P_INFO("Longitude", 20, 90);						Longitude.set_Label(adr_Longitude);
	Longitude_tchn = P_INFO("Longitude_tchn", 11, 0.000858);		Longitude_tchn.set_Label(adr_Longitude_tchn);
	Zadergka = P_INFO("Zadergka", 20, 512);							Zadergka.set_Label(adr_Zadergka);
	_time_UTC_h = P_INFO("Chas", 5, 16);							_time_UTC_h.set_Label(adr_UTC_st);
	_time_UTC_m = P_INFO("Minuta", 6, 32);							_time_UTC_m.set_Label(adr_UTC_st);
	_time_UTC_s = P_INFO("Secunda", 6, 32);							_time_UTC_s.set_Label(adr_UTC_st);
	time_UTC_ml = P_INFO("UTC_ml", 20, 512);						time_UTC_ml.set_Label(adr_UTC_ml);
	Vh = P_INFO("Vh", 16, 16384);									Vh.set_Label(adr_Vh);
	paketiki.resize(14);
	for (int i = 0; i < paketiki.size(); i++) {
		paketiki[i] = new char(sizeof(BNR));
		memset(paketiki[i], 0, sizeof(BNR));
	}
	fLog.open("../log_from_SNS.txt");
}
void TSNS::Log() {

	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);
	fLog << "\nТекущее время: " << asctime(localtime(&in_time_t));
	fLog << "Слово состояния СНС:\n";
	fLog << "Запрос начальных данных: " << SW_SNS.Value.RequestInitalData << endl;
	fLog << "Тип рабочей СРНС: " << SW_SNS.Value.TypeSRNS <<endl;
	fLog << "Альманах GPS: " << SW_SNS.Value.Almanac_GPS << endl;
	fLog << "Альманах ГЛОНАСС: "<< SW_SNS.Value.Almanac_GLONASS << endl;
	fLog << "Режим работы: " << SW_SNS.Value.OperatingMode << endl;
	fLog << "Подрежим работы: " << SW_SNS.Value.OperatingSubMode << endl;
	fLog << "Признак времени: " << SW_SNS.Value.TimeInd << endl;
	fLog << "Дифф. режим измерений: " << SW_SNS.Value.DiffMode << endl;
	fLog << "Отказ изделия: " << SW_SNS.Value.Failure << endl;
	fLog << "Порог сигнализации: " << SW_SNS.Value.AlamThreshold << endl;
	fLog << "Система координат: " << SW_SNS.Value.ReferenceFrame << endl;

	fLog << "Слово 'ДАТА':\n";
	fLog << "День: " << _Date.Value.Day_de * 10 + _Date.Value.Day_ed << endl;
	fLog << "Месяц: " << _Date.Value.Month_de * 10 + _Date.Value.Month_ed << endl;
	fLog << "Год: " << _Date.Value.Year_de * 10 + _Date.Value.Year_ed << endl;

	fLog << "Height: " << Height.get_Value() << endl;
	fLog << "HDOP: " << HDOP.get_Value() << endl;
	fLog << "VDOP: " << VDOP.get_Value() << endl;
	fLog << "WayAngle: " << WayAngle.get_Value() << endl;
	fLog << "Latitude: " << Latitude.get_Value() << endl;
	fLog << "Latitude_tchn: " << Latitude_tchn.get_Value() << endl;
	fLog << "Longitude: " << Longitude.get_Value() << endl;
	fLog << "Longitude_tchn: " << Longitude_tchn.get_Value() << endl;
	fLog << "Zadergka: " << Zadergka.get_Value() << endl;
	fLog << "UTC_st: " << _time_UTC_h.get_Value() << "/" << _time_UTC_m.get_Value() << "/" << _time_UTC_s.get_Value() << endl;
	fLog << "UTC_ml: " << time_UTC_ml.get_Value() << endl;
	fLog << "Vh: " << Vh.get_Value() << endl;
}

int TSNS::get_numMonth(string month) {
	if (month == "Jan")
		return 1;
	else if (month == "Feb")
		return 2;
	else if (month == "Mar")
		return 3;
	else if (month == "Apr")
		return 4;
	else if (month == "May")
		return 5;
	else if (month == "Jun")
		return 6;
	else if (month == "Jul")
		return 7;
	else if (month == "Aug")
		return 8;
	else if (month == "Sep")
		return 9;
	else if (month == "Oct")
		return 10;
	else if (month == "Nov")
		return 11;
	else
		return 12;
}

void TSNS::setData(double Height, double WayAngle, double Latitude, double Longitude, double Vh) {
	mutex.lock();
	this->Height = Height;
	this->WayAngle = WayAngle;
	this->Latitude = Latitude;
	this->Longitude = Longitude;
	this->Vh = Vh;
	mutex.unlock();
}