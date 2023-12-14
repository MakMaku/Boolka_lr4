#pragma once
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include "AllMath.h"
#include "TClient.h"
#include "KMLreader.h"
#include <mutex>

class TOPS: public TClient{
protected:
	vector<vector<double>> PPMs;		//Latitude - 0, Longitude - 1, H - 2
	vector<vector<double>> outPPMs;
	time_point<steady_clock> prev_time, time_begin;
	int idPPMs = 0, count = 0, prev_idPPMs = 0;
	bool f_new_data = false;
	ofstream Log;
	mutex mutex;
	KMLreader reader;
	double A12, S, rollOPS, pitchOPS,
		curRoll, curPitch, curCourse,
		curLatitude, curLongitude, Height = 0,
		e2 = 0.0066934216, a = 6378136,
		b = 1 / 298.25784;
public:
	TOPS(string NameFileKML, double initLat, double initLng);
	void Working() override;
	void Sending() override;
	void LookingForPPMs();
	//input radians
	void setData(double curLatitude, double curLongitude, double Height, double curPitch, double curRoll, double curCourse);
	//output radians
	double getCourse() { return A12; }
	//output ¹ PPM
	int get_idPPM() { return idPPMs; }
	//output deg
	double get_LatPPM() { return PPMs[idPPMs][0]; }
	//output deg
	double get_LngPPM() { return PPMs[idPPMs][1]; }
	//Output
	double get_S() { return S; }
};

