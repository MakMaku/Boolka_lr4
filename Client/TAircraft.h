#pragma once
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#define Deg2Rad M_PI / 180
#define Rad2Deg 180 / M_PI
#include "TINS.h"
#include "TSNS.h"
#include "TOPS.h"
#include "TASP.h"
#include "TServer.h"
#include "AllMath.h"
class TAircraft: public TClient, public TServer{
protected:
	enum FlightStage {
		STAGE_IDLE = 0,
		STAGE_TAKEOFF = 1,
		STAGE_FLIGHT = 2
	}_fs;
	double initLatitude, initLongitude,
		curLatitude, curLongitude, x, y, z,
		curCourse, curPitch, curRoll,
		demCourse, demPitch, demRoll,
		curVelocity, demVelocity, TakeOffVel,
		curHeight, demHeight,
		curV_x = 0, curV_z = 0, curV_y = 0,
		_intg_roll_prev = 0, _intg_pitch_prev = 0,
		R = 6378136, g = 9.81, m = 420;
	TINS* INS;
	TSNS* SNS;
	TOPS* OPS;
	TASP* ASP;
	mutex mutex;
	time_point<steady_clock> prev_time, time_begin;
	bool f_ins_ready = false, f_sns_ready = false, 
		f_pitch_clear = false, f_roll_clear = false,
		f_ASP_launched = false;
	int prev_idPPM = -1, idPPM = -1;
	ofstream log_file;

	void Log(vector<double>temp);
public:
	TAircraft(double Latitude, double Longitude, double Course, double Pitch, double Roll, string FilePPMs, double TakeOffVel = 160, double demVelocity = 300, double demHeight = 2000);
	void StartAircraft();
	void Working() override;
	void Sending() override;
};

