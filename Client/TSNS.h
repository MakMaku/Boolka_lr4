#pragma once
#include "TClient.h"
#include "..\P_INFO.h"
#include <mutex>
#include <fstream>
#include <ctime>
#include <string>

class TSNS: public TClient{
protected:
	enum OperatingMode {
		MODE_TURNON = 0,
		MODE_CONTROL = 1,
		MODE_SYNCHRONIZATION = 2,
		MODE_NAVIGATION = 3,
		MODE_DIFNAVIGATION = 4
	} _OperatingMode;
	mutex mutex;
	bool f_data_ready_to_send = false;
	time_point<steady_clock> prevTime, startTime;
	P_INFO Height, HDOP, VDOP, WayAngle,
		Latitude, Latitude_tchn, Longitude, Longitude_tchn,
		Zadergka, time_UTC_ml, Vh,
		_time_UTC_m, _time_UTC_h, _time_UTC_s;
	Date _Date;
	SRNS SW_SNS;
	BNR _Height, _HDOP, _VDOP, _WayAngle,
		_Latitude, _Latitude_tchn, _Longitude, _Longitude_tchn,
		_Zadergka, _time_UTC_st, _time_UTC_ml, _Vh;
	vector <char*> paketiki;
	StatucWordINS SW_INS;
	int count = 0, SW_INS_log;
	ofstream fLog;
	void Init();
	void Log();
	int get_numMonth(string month);
public:
	void Working() override;
	void Sending() override;
	void setData(double Height, double WayAngle, double Latitude, double Longitude, double Vh);
};

