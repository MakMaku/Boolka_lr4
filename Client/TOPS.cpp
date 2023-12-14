#include "TOPS.h"

TOPS::TOPS(string NameFileKML, double initLat, double initLng) {
	KMLreader reader;
	this->PPMs = reader.readPPMs(NameFileKML);
	cout << "PPMs is ready: " << PPMs.size() << " points" << endl;
	for (int i = 0; i < PPMs.size(); i++)
		cout << i + 1 << ": lat= " << PPMs[i][1] << "\tlng= " << PPMs[i][0] << endl;
	cout << endl;
	prev_time = steady_clock::now();
	Log.open("../LV_from_OPS.txt");
	outPPMs.resize(1);
	outPPMs[0].resize(3);
	outPPMs[0][0] = initLat;
	outPPMs[0][1] = initLng;
	outPPMs[0][2] = 0;
	time_begin = steady_clock::now();
};

void TOPS::setData(double curLatitude, double curLongitude, double Height, double curPitch, double curRoll, double curCourse) {
	mutex.lock();
	this->curLatitude = curLatitude;
	this->curLongitude = curLongitude;
	this->Height = Height;
	this->curPitch = curPitch;
	this->curRoll = curRoll;
	this->curCourse = curCourse;
	f_new_data = true;
	mutex.unlock();
}

void TOPS::Working() {
	mutex.lock();

	time_point<steady_clock> timeLabel = steady_clock::now();
	double phi2 = PPMs[idPPMs][0] * M_PI / 180, lmbd = PPMs[idPPMs][1] * M_PI / 180,
		sinu1 = (sin(curLatitude) * sqrt(1 - e2)) / sqrt(1 - e2 * pow(sin(curLatitude), 2)),
		cosu1 = cos(curLatitude) / sqrt(1 - e2 * pow(sin(curLatitude), 2)),
		sinu2 = (sin(phi2) * sqrt(1 - e2)) / sqrt(1 - e2 * pow(sin(phi2), 2)),
		cosu2 = cos(phi2) / sqrt(1 - e2 * pow(sin(phi2), 2)),
		delta_lmbd = lmbd - curLongitude,
		sin_s = sqrt(pow(cosu2 * sin(delta_lmbd), 2) + pow(cosu1 * sinu2 - sinu1 * cosu2 * cos(delta_lmbd), 2)),
		cos_s = sinu1 * sinu2 + cosu1 * cosu2 * cos(delta_lmbd),
		sigma = atan(sin_s / cos_s),
		M = (sigma - sin_s) / (1 + cos_s),
		N = (sigma + sin_s) / (1 - cos_s),
		U = pow(sinu1 + sinu2, 2),
		V = pow(sinu1 - sinu2, 2),
		kinf = (a - b) / a,
		delta_s = -0.25 * kinf * (M * U + N * V);
	A12 = atan2(cosu2 * sin(delta_lmbd), (cosu1 * sinu2 - sinu1 * cosu2 * cos(delta_lmbd)));
	S = a * (sigma + delta_s);
	if (f_new_data)
		LookingForPPMs();
	if (S <= 50) {
		cout << "\nPPM " << idPPMs + 1 << " has been reached" << endl;
		idPPMs = idPPMs + 1;
		if(idPPMs == PPMs.size())
			idPPMs--;
	}
	if (count == 0) {
		if (f_new_data) {
			outPPMs.resize(outPPMs.size() + 1);
			outPPMs[outPPMs.size() - 1].resize(3);
			outPPMs[outPPMs.size() - 1][0] = curLatitude * 57.3;
			outPPMs[outPPMs.size() - 1][1] = curLongitude * 57.3;
			outPPMs[outPPMs.size() - 1][2] = Height;
		}
		//count = 50;
	}
	else
		count--;
	if (duration<double>(timeLabel - prev_time).count() >= 30) {
		reader.writePPMs(outPPMs, "pathPPMs_current.kml");
		prev_time = steady_clock::now();
		prev_idPPMs++;
	}
	mutex.unlock();
}

void TOPS::LookingForPPMs() {
	time_point<steady_clock> timeLabel = steady_clock::now();
	//Самолёт в топ.
	vector<double> X = Geo2Top(a + Height, curLatitude * M_PI / 180, curLongitude * M_PI / 180);
	//ППМ в топ.
	vector<double> X_PPM = Geo2Top(a, PPMs[idPPMs][1] * M_PI / 180, PPMs[idPPMs][0] * M_PI / 180,
		curLatitude * M_PI / 180, curLongitude * M_PI / 180);
	//Матрица из топ. в св.
	vector<vector<double>> N2Sv = { {cos(curCourse) * cos(curPitch), sin(curPitch), -sin(curCourse) * cos(curPitch)},
		{sin(curCourse) * sin(curRoll) - cos(curCourse) * sin(curPitch) * cos(curRoll), cos(curPitch) * cos(curRoll), cos(curCourse) * sin(curRoll) + sin(curCourse) * sin(curPitch) * cos(curRoll)},
		{sin(curCourse) * cos(curRoll) + cos(curCourse) * sin(curPitch) * sin(curRoll), -cos(curPitch) * sin(curRoll), cos(curCourse) * cos(curRoll) - sin(curCourse) * sin(curPitch) * sin(curRoll)}
	};

	X = product(N2Sv, X);
	//ППМ в св.
	X_PPM = product(N2Sv, X_PPM);
	X_PPM[0] = X_PPM[0] - X[0];
	X_PPM[1] = X_PPM[1] - X[1];
	X_PPM[2] = X_PPM[2] - X[2];
	X_PPM = D2S(X_PPM[0], X_PPM[1], X_PPM[2]);
	pitchOPS = saturation(90 - X_PPM[1] * 180 / M_PI, 0, 90);
	rollOPS = saturation(X_PPM[2] * 180 / M_PI, -45, 45);
	Log << "\nAt: " << duration<double>(timeLabel - time_begin).count() << endl;
	Log << "\tPitch: " << pitchOPS << "\tRoll: " << rollOPS << endl;

}

void TOPS::Sending() {

}