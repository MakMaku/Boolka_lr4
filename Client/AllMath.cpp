#include "AllMath.h"

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
	/*return {-x * cos(lambda) * sin(phi) + y * cos(phi) * cos(lambda) - z * sin(lambda),
	-x * sin(lambda) * sin(phi) + y * cos(phi) * sin(lambda) + z * cos(lambda),
	x * cos(phi) + y * sin(phi) };*/
	return { -cos(lambda) * sin(phi) * x + -y * sin(lambda) * sin(phi) + z * cos(phi),
		x * cos(phi) * cos(lambda) + y * cos(phi) * sin(lambda) + z * sin(phi),
		-x * sin(lambda) + y * cos(lambda) };
}

//output: x, y, z
vector<double> Geo2Top(double ro, double phi, double lambda) {
	vector<double> temp = S2D(ro, phi, lambda);
	return Dec2Top(temp[0], temp[1], temp[2], phi, lambda);
}
//output: x, y, z
vector<double> Geo2Top(double ro, double phi, double lambda, double phi_init, double lambda_init) {
	vector<double> temp = S2D(ro, phi, lambda);
	return Dec2Top(temp[0], temp[1], temp[2], phi_init, lambda_init);
}

//output: ro, phi, lambda
vector<double> Top2Geo(double x, double y, double z, double phi, double lambda) {
	vector<vector<double>> temp = { {-cos(lambda) * sin(phi), -sin(lambda) * sin(phi), cos(phi)},
									{cos(phi) * cos(lambda), cos(phi) * sin(lambda), sin(phi)},
									{-sin(lambda), cos(lambda), 0} };
	temp = obr(temp);
	vector<double> v_t = product(temp, { x,y,z });
	return D2S(v_t[0], v_t[1], v_t[2]);
};

double saturation(double amt, double low, double high) {
	if (isnan(amt)) {
		return (low + high) * 0.5f;
	}
	return ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)));
}

vector<vector<double>> obr(vector<vector<double>> p) {
	vector<vector<double>> temp = { {0, 0, 0},
		{0,0,0},
		{0,0,0} };
	double det = p[0][0] * p[1][1] * p[2][2] + p[0][1] * p[1][2] * p[2][0] + p[0][2] * p[1][0] * p[2][1] -
		p[0][2] * p[1][1] * p[2][0] - p[0][1] * p[1][0] * p[2][2] - p[0][0] * p[1][2] * p[2][1];
	temp[0][0] = p[1][1] * p[2][2] - p[1][2] * p[2][1];
	temp[0][1] = -p[1][0] * p[2][2] + p[1][2] * p[2][0];
	temp[0][2] = p[1][0] * p[2][2] - p[1][2] * p[2][0];
	temp[1][0] = -p[0][1] * p[2][2] + p[0][2] * p[2][1];
	temp[1][1] = p[0][0] * p[2][2] - p[0][2] * p[2][0];
	temp[1][2] = -p[0][0] * p[2][1] + p[0][1] * p[2][0];
	temp[2][0] = p[0][1] * p[2][2] - p[0][2] * p[1][1];
	temp[2][1] = -p[0][0] * p[1][2] + p[0][2] * p[1][0];
	temp[2][2] = p[0][0] * p[1][1] - p[0][1] * p[1][0];
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			temp[i][j] /= det;
	for (int i = 0; i < 3; i++)
		for (int j = i; j < 3; j++)
			if (i != j) {
				det = temp[i][j];
				temp[i][j] = temp[j][i];
				temp[j][i] = det;
			}
	return temp;
}

vector<double> product(vector<vector<double>> m, vector<double> v) {
	vector <double> outV = { 0, 0, 0 };
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			outV[i] += m[i][j] * v[j];
	return outV;
}

double ro(double H) {
	return 101325 * pow(1 + (-0.0065) * H / 288.15, -9.81 * 0.0289644 / (8.31447 * (-0.0065)));
}