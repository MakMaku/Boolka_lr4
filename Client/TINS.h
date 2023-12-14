#pragma once
#include "TClient.h"
#include <mutex>
#include "../P_INFO.h"
#include <fstream>

class TINS: public TClient{
protected:
	enum OperatingMode {
		MODE_STANDBY = 0,
		MODE_TURNON = 1,
		MODE_PREPARING = 2,
		MODE_NAVIGATION = 3,
		MODE_RESTART = 4,
		MODE_CONTROL = 5
	} _OperatingMode;
	mutex mutex;
	bool f_data_ready_to_send = false;
	time_point<steady_clock> prevTime, startTime;
	P_INFO Longitude, Latitude, Height,
		trueCourse, Pitch, Roll,
		Vn, Ve, Vy,
		Ax, Ay, Az;
	BNR lng, lat, h, 
		c_true, pitch, roll, 
		vn, ve, vy, 
		ax, ay, az;
	vector <char*> paketiki;
	StatucWordINS SW_INS;
	int count = 0, SW_INS_log;
	ofstream fLog;
	void Init();
	void Log();
public:
	void Working() override;
	void Sending() override;
	void setData(double Longitude, double Latitude, double Height, double trueCourse, double Pitch, double Roll, double V);
};

