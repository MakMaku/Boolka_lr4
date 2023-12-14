#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

class KMLreader{
public:
	vector<vector<double>> readPPMs(string NameFile);
	void writePPMs(vector<vector<double>> outPPMs, string NameFile = "newPPMs.kml", string ProjectName = "CurMov", string PathName = "CurParh");
};

