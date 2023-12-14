#pragma once
#include <iostream>

using namespace std;

class P_INFO{
protected:
	string ParName;			//�������� ���������
	double Value;			//�������� ���������
	int binValue;	//�������� ������������� �������� ���������, ���������� ��� - 19
	//��������� ��� - ���� (0 = ����/�����/������/�����, 1 = �����/��/�����/����), ����������� ���� - ��������
	int NumBit;				//����� �������� ���
	double Max;				//���� �������� �������
	int Label = 0;			//��������� ������� Label, 0<=Label<=255
	int SDI = 0;			//��������� �������� SDI � ������ ���������� ������������� ���������� �������� ���, ����� � ������ SDI �������� �������� ��� 20
	int SSM = 0;			//��������� �������� SSM - ������� ���������, 0 - ���������� ������
	int P = 0;				//��� ��������
	
	//Additional functions
	void ConvertToBin();
	void ConvertToNotBin();
public:
	//Constructors
	P_INFO();
	P_INFO(string inParName, int inNumBit, double inMax, double inValue = NULL, int inbinValue = -1, int inLabel = 0, int inSDI = 0, int inSSM = 0, int inP = 0);
	
	//Set
	void set_ParName(string inParName) { this->ParName = inParName; }
	void set_Value(double inValue) { this->Value = inValue; }
	void set_NumBit(int inNumBit) { this->NumBit = inNumBit; }
	void set_Max(double inMax) { this->Max = inMax; }
	void set_Label(int inLabel) { this->Label = inLabel; }
	void set_SDI(int inSDI) { this->SDI = inSDI; }
	void set_SSM(int inSSM) { this->SSM = inSSM; }
	void set_P(int inP) { this->P = inP; }
	void set_bincode(int binCode);
	void set_binValue(int binValue);

	//Get
	string get_name() { return ParName; }
	double get_Value() { return Value; }
	int get_binValue() { return binValue; }
	int get_NumBit() { return NumBit; }
	double get_Max() { return Max; }
	int get_Label() { return Label; }
	int get_SDI() { return SDI; }
	int get_SSM() { return SSM; }
	int get_P();
	int get_bincode();
	
	//Math operations
	P_INFO operator =(double inValue);
	P_INFO operator =(P_INFO inPar);
	double operator *(double par);
	double operator +(double par);
};

