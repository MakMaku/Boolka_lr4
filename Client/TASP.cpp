#include "TASP.h"

TASP::TASP(double initLat, double initLng) {
	this->initLatitude = initLat;
	this->initLongitude = initLng;
	log_file.open("../Log_from_ASP.txt");
	log_file << "X\t" << "Y\t" << "V\t" << "Theta" << endl;
}

void TASP::Start_ASP(vector<double>X, vector<double> Euler, vector<double> Geog) {
	mutex.lock();
	Theta = Euler[0] * Deg2Rad;
	Course = Euler[1] * Deg2Rad;
	x = X[0];	y = X[1];	z = X[2];
	f_ready = true;
	prev_time = steady_clock::now();
	H = Geog[2];
	this->X = 0; this->Y = H;
	V = X[3];
	double A = V * sqrt(2 * H) * (1 - pow(e, -0.000106 * H) * c * H * (1 + 0.000031 * H / 5) / 6) / g,
		T = sqrt(2 * H) * (1 - pow(e, -0.000106 * H) * c * H * (1 + 0.000063 * H / 5) / 6) / g;
	cout << "Start ASP: A = " << A << "\tT(H) = " << T << "\tV = " << V << endl;
	WayPoints.resize(1);
	WayPoints[0].resize(3);
	WayPoints[0][0] = Geog[0];
	WayPoints[0][1] = Geog[1];
	WayPoints[0][2] = H;
	mutex.unlock();
}

void TASP::Working() {
	if (!f_ready)
		return;

	mutex.lock();
	time_point<steady_clock> timeLabel = steady_clock::now();
	double dt = duration<double>(timeLabel - prev_time).count();
	prev_time = timeLabel;

	double _intg_Theta_input = -g * cos(Theta) / V,
		_intg_V_input = -Cx * S * V * V * ro / (2 * m) - g * sin(Theta);
	
	V += (_intg_V_input + _intg_V_prev)*dt;
	_intg_V_prev = _intg_V_input;

	Theta += (_intg_Theta_input + _intg_Theta_prev) * dt;					
	_intg_Theta_prev = _intg_Theta_input;
	
	X += (V * cos(Theta) + _intg_x_prev) * dt;
	_intg_x_prev = V * cos(Theta);

	Y += (V * sin(Theta) + _intg_y_prev) * dt;
	_intg_y_prev = V * sin(Theta);

	double x = this->x + X * cos(Course),
		y = this->y + Y,
		z = this->z + X * sin(Course);

	vector<double> temp = Top2Geo(x, y, z, initLatitude * Deg2Rad, initLongitude * Deg2Rad);
	f_ready = (Y <= 0) ? false : true;

	if ((count == 0) || !f_ready) {
		WayPoints.resize(WayPoints.size() + 1);
		WayPoints[WayPoints.size() - 1].resize(3);
		WayPoints[WayPoints.size() - 1][0] = temp[1] * Rad2Deg;
		WayPoints[WayPoints.size() - 1][1] = temp[2] * Rad2Deg;
		WayPoints[WayPoints.size() - 1][2] = Y;
		if (!f_ready) {
			WayPoints[WayPoints.size() - 1][0] = prevLng;
			WayPoints[WayPoints.size() - 1][1] = prevLat;
			WayPoints[WayPoints.size() - 1][2] = prevH;
		}
		//count = 50;
	}
	else
		count--;
	if ((duration<double>(timeLabel - time_begin).count() >= 5) || !f_ready) {
		writer.writePPMs(WayPoints, "ASP_way.kml", "ASP");
		time_begin = steady_clock::now();
	}
	prevLng = temp[1] * Rad2Deg;
	prevLat = temp[2] * Rad2Deg;
	prevH = Y;
	Log(dt);
	mutex.unlock();
}

void TASP::Log(double dt) {
	log_file << X << "\t" << Y << "\t" << V << "\t" << Theta << endl;
}

void TASP::Sending() {

}