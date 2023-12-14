#include "KMLreader.h"
vector<vector <double>> KMLreader::readPPMs(string NameFile) {
	ifstream file(NameFile);
	string line, coord;
	vector<vector<double>> PPMs;
	int prev_z = 0, next_z = 0, size_line = 0, next_pr = 0;
	string poisk = "			<coordinates>";
	//Поиск места записи координат
	while (getline(file, line)) {
		if (line == poisk)
			break;
	}

	//Получение строки с координатами
	getline(file, line);
	
	//Подсчет количества координат
	for (int i = 0; i < line.size(); i++)
		if (line[i] == ' ')
			prev_z++;
	PPMs.resize(prev_z);
	prev_z = 0;

	//Запись координат
	for (int i = 0; i < PPMs.size(); i++) {
		PPMs[i].resize(3);

		//Поиск координаты
		next_pr = line.find(" ", next_pr + 1);

		//Получение координаты
		coord = line.substr(prev_z + 1, next_pr - size_line);
		size_line += coord.size();
		
		//Получение широты
		prev_z = coord.find(",", 0);
		PPMs[i][0] = stod(coord.substr(0, prev_z));

		//Получение долготы
		next_z = coord.find(",", prev_z + 1);
		PPMs[i][1] = stod(coord.substr(prev_z + 1, next_z - prev_z));

		//Получение высоты
		PPMs[i][2] = stod(coord.substr(next_z + 1, coord.size() - next_z + 1));
		
		prev_z = next_pr;
	}
	file.close();
	return PPMs;
}

void KMLreader::writePPMs(vector<vector<double>> outPPMs, string NameFile, string ProjectName, string PathName) {
	ofstream inFile("../" + NameFile);
	ifstream fromFile("ExampleKML.kml");
	string line, toFile, Points;
	stringstream stream;
	stream.precision(14);
	stream << fixed;
	while (getline(fromFile, line)) {
		if (line == "	<name>CHANGEPROJECTNAME</name>")
			toFile = "	<name>" + ProjectName + "</name>";
		else if (line == "		<name>CHANGEPATHNAME</name>")
			toFile = "		<name>" + PathName + "</name>";
		else if (line == "				CHANGEPOINTS") {
			toFile = "				";
			for (int i = 0; i < outPPMs.size(); i++)
				for (int j = 0; j < 3; j++) {

					if (outPPMs[i][j] == 0) {
						toFile += "0";
						if (j == 2)
							toFile += " ";
						else
							toFile += ",";
						continue;
					}

					stream << outPPMs[i][j];
					toFile += stream.str();
					stream.str("");

					if (j == 2)
						toFile += " ";
					else
						toFile += ",";
				}
		}
		else
			toFile = line;
		inFile << toFile << endl;;
	}
	inFile.close();
	fromFile.close();
}