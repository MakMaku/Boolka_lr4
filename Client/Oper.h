#pragma once

#include <cmath>
#include <iostream>
#include <vector>
#include <stdbool.h>

using namespace std;

vector <vector <long double>> vvod() {
	int n, m;
	cout << "*Creating matrix*\nEnter rows: "; cin >> n;
	cout << endl << "Enter columns: "; cin >> m;
	vector<vector<long double>> temp(n, vector <long double>(m));
	for (int i = 0; i < temp.size(); i++)
		for (int j = 0; j < temp[0].size(); j++)
			cin >> temp[i][j];
	cout << "\n\nCreated matrix:\n";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++)
			cout << temp[i][j] << " ";
		cout << endl;
	}
	cout<<"\n\n";
	return temp;
}

vector <long double> vvodv() {
	int n;
	cout << "*Creating vector*\nEnter columns: "; cin >> n;
	vector<long double> temp(n);
	for (int i = 0; i < temp.size(); i++)
			cin >> temp[i];
	cout << "\n\nCreated vector:\n";
	for (int i = 0; i < n; i++) {
			cout << temp[i]<< " ";
	}
	cout << "\n\n";
	return temp;
}

void vyvod(vector<vector<long double>> temp) {
	cout << endl;
	for (int i = 0; i < temp.size(); i++) {
		for (int j = 0; j < temp[0].size(); j++)
			cout << round(temp[i][j] * 10000) / 10000 << "\t"; 
		cout << endl;
	}
	return;
}

void vyvod(vector<long double> temp) {
	cout << endl;
	for (int i = 0; i < temp.size(); i++) {
			cout << round(temp[i] * 10000) / 10000 << "\t";
	}
	cout << endl;
	return;
}

class Tmatrix;
class Tquaternion;
class Tvector {
protected:
	vector <long double> opr;
public:
	Tvector(vector <long double> par) {
		opr = par;
	}
	Tvector(int i) {
		opr.resize(i);
	}
	Tvector(){}
	//Присваивание вектору
	Tvector operator = (Tvector temp) {
		if (opr.size() != temp.get().size())
			resize(temp.get().size());
		for (int i = 0; i < opr.size(); i++)
			opr[i] = temp.get()[i];
		return (*this);
	}
	int size() {
		return opr.size();
	}
	Tvector operator = (vector<long double> temp) {
		if (opr.size() != temp.size())
			resize(temp.size());
		for (int i = 0; i < opr.size(); i++)
			opr[i] = temp[i];
		return (*this);
	}
	Tvector operator = (Tmatrix par);
	//Получить вектор в vector
	vector<long double> get() {
		return opr;
	}
	//Задать вектор через массив
	void set(vector<long double> par) {
		opr = par;
	}
	//Полчить i-ый элемент
	long double& operator()(int i) {
		return opr[i];
	}
	//Изменить размер вектора
	void resize(int i) {
		opr.resize(i);
	}
	//Задать i-ый элемент
	void operator()(int i, long double par) {
		opr[i] = par;
	}
	Tvector operator + (Tvector par)const{
		Tvector temp(opr.size());
		for (int i = 0; i < opr.size(); i++) {
			temp(i) = opr[i] + par(i);
		}
		return temp;
	}
	Tvector operator * (long double par)const {
		Tvector temp(opr.size());
		for (int i = 0; i < opr.size(); i++)
			temp(i)= opr[i] * par;
		return temp;
	}
	friend Tvector operator * (long double par, Tvector mpar) {
		Tvector temp(mpar.get().size());
		for (int i = 0; i < mpar.get().size(); i++)
			temp(i)= mpar(i) * par;
		return temp;
	}
	long double operator * (Tvector par) {
		long double sum = 0;
		for (int i = 0; i < opr.size(); i++)
			sum = sum + opr[i] * par(i);
		return sum;
	}
	Tvector operator ^ (Tvector par) {
		Tvector temp(3);
		temp(0)= opr[1] * par(2) - opr[2] * par(1);
		temp(1)= opr[2] * par(0) - opr[0] * par(2);
		temp(2)= opr[0] * par(1) - opr[1] * par(0);
		return temp;
	}
	Tvector operator * (Tmatrix* par);
	//Получить модуль вектора
	long double modul() {//+
		long double sum = 0;
		for (int i = 0; i < opr.size(); i++)
			sum = sum + opr[i] * opr[i];
		return sqrt(sum);
	}
	~Tvector() {}
};

class Tmatrix {
protected:
	vector<vector<long double>> opr;
public:
	vector <vector <long double>> get() {
		return opr;
	}
	void set(vector < vector < long double >> par) {
		opr = par;
	}
	void resize(int i, int j) {
		opr.resize(i);
		for (int k = 0; k < i; k++)
			opr[k].resize(j);
	}
	long double& operator()(int i, int j) {
		return opr[i][j];
	}
	void operator()(int i, int j, long double par) {
		opr[i][j] = par;
	}

	Tmatrix(vector <vector <long double>> par) {
		opr = par;
	}
	Tmatrix(int i, int j){
		opr.resize(i, vector<long double>(j));
	}
	int get_size_i() {
		return opr.size();
	}
	int get_size_j() {
		return opr[0].size();
	}
	Tmatrix(){}
	~Tmatrix(){}
	Tmatrix operator = (Tmatrix temp) {
		if (opr.size() != temp.get().size())
			resize(temp.get().size(), temp.get()[0].size());
		for (int i = 0; i < opr.size(); i++)
			opr[i] = temp.get()[i];
		return (*this);
	}
	
	Tmatrix operator + (Tmatrix par)const {
		Tmatrix temp(opr.size(), opr[0].size());
		for (int i = 0; i < opr.size(); i++)
			for (int j = 0; j < opr[i].size(); j++)
				temp(i, j) = opr[i][j] + par(i, j);
		return temp;
	}
	Tmatrix operator * (long double par)const {
		Tmatrix temp(opr);
		for (int i = 0; i < opr.size(); i++)
			for (int j = 0; j < opr[0].size(); j++)
				temp(i,j)=opr[i][j] * par;
		return temp;
	}
	friend Tmatrix operator * (long double par, Tmatrix mpar) {
		Tmatrix temp = mpar;
		for (int i = 0; i < mpar.get().size(); i++)
			for (int j = 0; j < mpar.get()[0].size(); j++)
				temp(i,j)= mpar(i,j) * par;
		return temp;
	}
	Tvector operator * (Tvector par)const {
		Tvector temp(opr.size());
		double sum = 0;
		for (int i = 0; i < opr.size(); i++) {
			for (int j = 0; j < par.size(); j++)
				sum += opr[i][j] * par(j);
			temp(i) = sum;
			sum = 0;
		}
		return temp;
	}
	Tmatrix operator * (Tmatrix par)const {
		long double sum = 0;
		Tmatrix temp(opr.size(), par.get()[0].size());
		for (int i = 0; i < opr.size(); i++)
			for (int j = 0; j < par.get()[0].size(); j++) {
				for (int k = 0; k < opr[0].size(); k++)
					sum = sum + opr[i][k] * par(k,j);
				temp(i, j) = sum; sum = 0;
			}
		return temp;
	}
	
	bool checkSym() {//+
		bool check = false;
		if (opr.size() != opr[0].size())
			return check;
		for (int i = 1; i < opr.size(); i++)
			for (int j = 0; j < i; j++) 
				if (i != j){
					if(opr[i][j] == opr[j][i])
						check = true;
					else
						return false;
				}
		return check;
	}
	Tmatrix flip() { //+
		vector<vector<long double>> par(opr[0].size(), vector <long double> (opr.size()));
		Tmatrix temp(opr[0].size(), opr.size());
		for (int i = 0; i < opr.size(); i++)
			for (int j = 0; j < opr[0].size(); j++)
				temp(j,i) = opr[i][j];
		return temp;
	}
	long double opred(vector <vector <long double>> par) {//+
		long double tmp=0;
		if (par.size() == 2) {
			tmp = par[0][0] * par[1][1] - par[0][1] * par[1][0];
			return tmp;
		}
		else if (par.size() == 3) {
			tmp = par[0][0] * par[1][1] * par[2][2] + par[0][1] * par[1][2] * par[2][0] + par[1][0] * par[2][1] * par[0][2];
			tmp = tmp - par[0][2] * par[1][1] * par[2][0] - par[1][0] * par[0][1] * par[2][2] - par[0][0] * par[1][2] * par[2][1];
			return tmp;
		}
		else {
			vector<vector<long double>> temp(par.size()-1, vector <long double>(par[0].size()-1));
			for (int i = 0; i < par.size(); i++) {
				for (int k = 0; k < par.size() - 1; k++)
					for (int j = 0; j < par[0].size() - 1; j++) {
						if(i==0)
							temp[0][j] = par[1][j + 1];
						if(k<i)
							temp[k][j] = par[k][j + 1];
						if(k>i)
							temp[k][j] = par[k+1][j + 1];
					}
				tmp = tmp + par[i][0]*pow(-1,(i))*opred(temp);
			}
			return tmp;
		}
	}
	
	Tmatrix Holec() { //+
		Tmatrix temp(opr.size(), opr[0].size()); 
		long double sum = 0; int N = opr.size();
		if ((opred(opr) <= 0)||(!checkSym())) {
			cout << endl << "There is not positive-definite or symmetric matrix" << endl;
			temp.set(opr);
			return temp;
		}

		//иницализация матрицы L и её заполнение;
		vector<vector<long double>> L(N, vector <long double>(N));
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++) {
				if (i < j)
					L[i][j] = 0;
				else if (i == j) {
					for (int k = 0; k < (i); k++)
						sum += L[i][k]*L[i][k];
					L[i][i] = sqrt(opr[i][i] - sum);
					sum = 0;
				}
				else {
					for (int k = 0; k < (j); k++)
						sum += L[i][k] * L[j][k];
					L[i][j] = (opr[i][j] - sum) / L[j][j];
					sum = 0;
				}
			}
		//инициализация и вычисление обратной матрицы
		vector<vector<long double>> tmp(N, vector <long double>(N));
		
		for (int i = N - 1; i >= 0; i--)
			for (int j = N - 1; j >= 0; j--) {
				if (i == j) {
					for (int k = i+1; k < N; k++)
						sum += L[k][i] * tmp[k][i];
					tmp[i][i] = (1 / L[i][i] - sum) / L[i][i];
					sum = 0;
				}
				else if(i>j) {
					for (int k = j+1; k < N; k++)
						sum += L[k][j] * tmp[k][i];
					tmp[i][j]=tmp[j][i] = -sum / L[j][j];
					sum = 0;
				}
			}
		temp.set(tmp);
		return temp;
	}
	Tmatrix Gauss() { //+
		Tmatrix temp(opr.size(), opr[0].size()); 
		long double ved_el, idk; int N = opr.size();
		vector<vector<long double>> tmp(opr.size(), vector <long double>(opr[0].size()));
		for (int i = 0; i < N; i++)  //создание единичной матрицы
			for (int j = 0; j < N; j++) {
				tmp[i][j] = 0;
				if (i == j)
					tmp[i][j] = 1;
			}
		vector <vector<long double>> par = opr; //начальная матрица
		if (opred(opr) == 0) {
			cout <<endl<< "There is invertible matrix" << endl;
			temp.set(opr);
			return temp;
		}
		for (int k = 0; k < N; k++) {
			if (par[k][k] == 0) {
				ved_el = k;
				while (ved_el < N) {
					if (par[ved_el][k] != 0)
						break;
					ved_el++;
				}
				for (int j = 0; j < N; j++) {
					idk = par[k][j];
					par[k][j] = par[ved_el][j];
					par[ved_el][j] = idk;
					idk = tmp[k][j];
					tmp[k][j] = tmp[ved_el][j];
					tmp[ved_el][j] = idk;
				}
			}
			ved_el = par[k][k];
			for (int j = 0; j < N; j++) {
				par[k][j] /= ved_el;
				tmp[k][j] /= ved_el;
			}
			for (int i = k + 1; i < N; i++) {
				ved_el = par[i][k];
				for (int j = 0; j < N; j++) {
					par[i][j] -= par[k][j] * ved_el;
					tmp[i][j] -= tmp[k][j] * ved_el;
				}
			}
			ved_el = 0;
		}
		for (int k = N - 1; k > 0; k--) {
			for (int i = k - 1; i >= 0; i--) {
				ved_el = par[i][k];
				for (int j = 0; j < N; j++) {
					par[i][j] -= par[k][j] * ved_el;
					tmp[i][j] -= tmp[k][j] * ved_el;
				}
			}
		}
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				if (tmp[i][j] == -0)
					tmp[i][j] = 0;
		temp.set(tmp);
		return temp;
	}
};

class Tquaternion {
protected:
	long double q;
	Tvector Q;
public:
	Tquaternion() {
		Q.resize(3); // M = opr(0) + opr(1)*i1 + opr(2)*i2 + opr(3)*i3, i1^2=i2^2=i3^2=-1
	}
	Tquaternion(long double p0, long double p1, long double p2, long double p3) {
		Q.resize(3); q = p1;
		Q(0) = p1; Q(1) = p2; Q(2) = p3;
	}
	Tquaternion(long double phi, Tvector par) {
		q = cosl(phi / 2);
		Q = par; Q = sinl(phi / 2) * Q;
	}
	Tquaternion(const Tquaternion& par) { *this = par; }
	long double scal(){return q;}
	Tvector vect() { return Q; }

};
Tvector Tvector::operator = (Tmatrix par) {
	if ((par.get()[0].size() == 1)) {
		if (opr.size() != par.get().size())
			resize(par.get().size());
		for (int i = 0; i < opr.size(); i++)
			opr[i] = par(i, 0);
	}
	else if (par.get().size() == 1) {
		if (opr.size() != par.get()[0].size())
			resize(par.get()[0].size());
		for (int i = 0; i < opr.size(); i++)
			opr[i] = par(0, i);
	}
	else if ((par.get().size() == 1) && (par.get()[0].size() == 1)) {
		if (opr.size() != 1)
			resize(1);
		opr[0] = par(0, 0);
	}
	else {
		for (int i = 0; i < opr.size(); i++)
			opr[i] = i*pow(-1,i+1);
	}
	return (*this);
};
Tvector Tvector::operator*(Tmatrix* par) {
	vector<long double> tmp(par->get()[0].size());
	long double sum = 0;
	for (int j = 0; j < tmp.size(); j++) {
		for (int k = 0; k < opr.size(); k++)
			sum += opr[k] * par->get()[k][j];
		tmp[j] = sum; sum = 0;
	}
	Tvector temp(tmp);
	return temp;
}