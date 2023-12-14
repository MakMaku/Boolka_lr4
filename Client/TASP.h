#pragma once
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#define Deg2Rad M_PI / 180
#define Rad2Deg 180 / M_PI
#include <mutex>
#include "TClient.h"
#include "KMLreader.h"
#include "AllMath.h"

class TASP : public TClient {
protected:
	int count = 0;
	bool f_ready = false;
	double initLongitude = 0, initLatitude = 0, H, prevLng, prevLat, prevH,
		R = 6378136, g = 9.81, e = 2.718, Cx = 0.7, S = 0.25, m = 20, c = 0.9, ro = 1.11;
	double x, y, z, X, Y, V,
		Theta, Course,
		_intg_x_prev = 0, _intg_y_prev = 0,
		_intg_Theta_prev = 0, _intg_V_prev = 0;
	vector<vector<double>>WayPoints;
	KMLreader writer;
	mutex mutex;
	time_point<steady_clock> prev_time, time_begin;
	ofstream log_file;
	void Log(double dt);
public:
	TASP(double initLat, double initLng);
	void Sending() override;
	void Working() override;
	void Start_ASP(vector<double> X, vector<double> Eulers, vector<double> Geog);
};

