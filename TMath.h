#pragma once
#include <iostream>
#include <vector>

using namespace std;

//output: x, y, z
vector<double> S2D(double ro, double phi, double lambda) {
	return { ro * cos(phi) * cos(lambda), ro * cos(phi) * sin(lambda), ro * sin(phi) };
}

//output: ro, phi, lambda
vector<double> D2S(double x, double y, double z) {
	return { sqrt(x * x + y * y + z * z), atan(z / sqrt(x * x + y * y)), atan(y / x) };
}
//output: x, y, z
vector<double> Dec2Top(double x, double y, double z, double phi, double lambda) {
	return { -x * cos(lambda) * sin(phi) + y * cos(phi) * cos(lambda) - z * sin(lambda),
	-x * sin(lambda) * sin(phi) + y * cos(phi) * sin(lambda) + z * cos(lambda),
	x * cos(phi) + y * sin(phi) };
}

//output: x, y, z
vector<double> Geo2Top(double ro, double phi, double lambda) {
	vector<double> temp = S2D(ro, phi, lambda);
	return Dec2Top(temp[0], temp[1], temp[2], phi, lambda);
}

//output: ro, phi, lambda
vector<double> Top2Geo(double x, double y, double z, double phi, double lambda) {
	vector<double> temp = { -x * cos(lambda) * sin(phi) - y * sin(lambda) * sin(phi) + z * cos(phi),
	x * cos(phi) * cos(lambda) + y * cos(phi) * sin(lambda) + z * sin(phi),
	-x * sin(lambda) + y * cos(lambda) };
	return D2S(temp[0], temp[1], temp[2]);
};

double saturation(double amt, double low, double high){
	if (isnan(amt)) {
		return (low + high) * 0.5f;
	}
	return ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)));
}
