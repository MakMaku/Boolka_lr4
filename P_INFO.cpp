#include "P_INFO.h"

P_INFO::P_INFO() {
	this->ParName = "NaN";
	this->Value = 0;
	this->NumBit = 1;
	this->Max = 1;
}

P_INFO::P_INFO(string inParName, int inNumBit, double inMax, double inValue, int inbinValue, int inLabel, int inSDI, int inSSM, int inP) {
	this->ParName = inParName;
	this->NumBit = inNumBit;
	this->Max = inMax;
	this->Label = inLabel;
	this->SDI = inSDI;
	this->SSM = inSSM;
	this->P = inP;
	//Было задано двоичное число
	if ((inValue == NULL) && (inbinValue != -1)) {
		this->binValue = inbinValue;
		ConvertToNotBin();
	}
	//Было задано десятично число
	else if ((inValue != NULL) && (inbinValue == -1)) {
		this->Value = inValue;
		ConvertToBin();
	}
	//Было задано ничего
	//else
		//cout << endl << ParName << "\t was created without value\n";
}

void P_INFO::ConvertToBin() {
	int bin = (NumBit > 19) ? ((NumBit == 20) ? 20 : 21) : (NumBit);
	int bincode = 0;
	double temp = 0, _Val;
	if (Value >= 0) {
		_Val = Value;
	}
	else{
		_Val = abs(Value);
		bincode = 1;
		bincode = bincode << 1;
	}
	for (int i = 0; i < (bin-1); i++) {
		temp += (Max / pow(2, i));
		if (temp <= _Val)
			bincode += 1;
		else
			temp -= (Max / pow(2, i));
		bincode = bincode << 1;
	}
	bincode = bincode >> 1;
	if (bin == 21) {
		SDI = (bincode & 3);	//Считываются только два крайних правых (3 _10 = 1 1 _2)
		binValue = bincode >> 2;
	}
	else if (bin == 20){
		SDI = (bincode & 1);	//Считывается последний бит
		binValue = bincode >> 1;
	}
	else {
		binValue = bincode;
	}
}

void P_INFO::ConvertToNotBin() {
	int bincode = binValue, mask = 0, num = (NumBit > 19) ? ((NumBit == 20) ? 20 : 21) : (NumBit);
	if (NumBit == 20) {
		bincode = (bincode << 1) + SDI;
	}
	else if (NumBit >= 21) {
		bincode = (bincode << 2) + SDI;
	}
	else
		bincode = bincode;
	Value = 0;

	for (int count = 1; count <= (num - 1); count++) {
		mask = pow(2, count - 1);
		mask = bincode & mask;
		if(mask != 0)
			Value += (Max / pow(2, num - count - 1));
	}

	bincode = bincode >> num - 1;

	if (bincode)
		Value *= -1;
	return;
}

P_INFO P_INFO::operator =(double inValue) {
	this->Value = inValue;
	ConvertToBin();
	return (*this);
}

P_INFO P_INFO::operator =(P_INFO inPar) {
	this->ParName = inPar.get_name();
	this->NumBit = inPar.get_NumBit();
	this->Max = inPar.get_Max();
	this->Label = inPar.get_Label();
	this->SDI = inPar.get_SDI();
	this->SSM = inPar.get_SSM();
	this->P = inPar.get_P();
	//Было задано двоичное число
	if ((inPar.get_Value() == NULL) && (inPar.get_binValue() != -1)) {
		this->binValue = inPar.get_binValue();
		ConvertToNotBin();
	}
	//Было задано десятично число
	else if ((inPar.get_Value() != NULL) && (inPar.get_binValue() == -1)) {
		this->Value = inPar.get_Value();
		ConvertToBin();
	}
	//Было задано ничего
	//else
	//	cout << endl << ParName << "\t was created without value\n";
	return (*this);
}

double P_INFO::operator *(double par) {
	return Value * par;
};
double P_INFO::operator +(double par) {
	return Value + par;
};

void P_INFO::set_bincode(int binCode) {
	int temp = binCode;
	Label = (temp & 255);
	temp = temp >> 8;
	SDI = (temp & 3);
	temp = temp >> 2;
	binValue = (temp & 524287);
	temp = temp >> 19;
	SSM = (temp & 3);
	temp = temp >> 2;
	P = abs(temp);
	ConvertToNotBin();
};

int P_INFO::get_bincode() {
	int full_code = P;
	full_code = full_code << 2;		full_code += SSM;
	full_code = full_code << 19;	full_code += binValue;
	full_code = full_code << 2;		full_code += SDI;
	full_code = full_code << 8;		full_code += Label;
	return full_code;
}

int P_INFO::get_P() {
	int full_code = SSM;									//SSM
	full_code = full_code << 19;	full_code += binValue;	//SSM_Data
	full_code = full_code << 2;		full_code += SDI;		//SSM_Data_SDI
	full_code = full_code << 8;		full_code += Label;		//SSM_Data_SDI_Label
	int count_1 = 0;
	for (int i = 0; i < 32; i++) {
		if ((full_code & 1) == 1)
			count_1++;
	}
	P = (count_1 % 2) == 0 ? 1 : 0;
	return P;
}

void P_INFO::set_binValue(int binValue) {
	this->binValue = binValue;
	ConvertToNotBin();
}