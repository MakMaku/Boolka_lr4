#pragma once
#include <iostream>
#include <vector>
using namespace std;

//input, ro, phi, lambda
//output: x, y, z
vector<double> S2D(double, double, double);

//input: x, y, z
//output: ro, phi, lambda
vector<double> D2S(double, double, double);

//input: x, y, z, phi0, lambda0
//output: x, y, z
vector<double> Dec2Top(double, double, double, double, double);

//input: ro, phi, lambda
//output: x, y, z
vector<double> Geo2Top(double, double, double);

vector<double> Geo2Top(double, double, double, double, double);

//input: x, y, z, phi0, lambda0
//output: ro, phi, lambda
vector<double> Top2Geo(double, double, double, double, double);

//input: value, min, max
//output: min<=value<=max
double saturation(double, double, double);

//input: matrix
//output: matrix^-1
vector<vector<double>> obr(vector<vector<double>>);

vector<double> product(vector<vector<double>>, vector<double>);

double ro(double H);