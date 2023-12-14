#pragma once

#define adr_SW_INS 184
#define adr_Lat 200
#define adr_Lng 201
#define adr_H 241
#define adr_C_True 204
#define adr_Pitch 212
#define adr_Roll 213
#define adr_Vn 246
#define adr_Ve 247
#define adr_Vy 245
#define adr_Ax 217
#define adr_Ay 218
#define adr_Az 219

#pragma pack(push, 1)
union BNR {
	struct data{
		unsigned Label: 8;
		unsigned SDI: 2;
		unsigned D: 19;
		unsigned SSM: 2;
		unsigned P: 1;
	} Value;
};
#pragma pack(pop)

#pragma pack(push, 1)
union BCD {
	struct data {
		unsigned Label : 8;
		unsigned SDI : 2;
		unsigned Digit5 : 4;
		unsigned Digit4 : 4;
		unsigned Digit3 : 4;
		unsigned Digit2 : 4;
		unsigned Digit1 : 3;
		unsigned SSM : 2;
		unsigned P : 1;
	} Value;
};
#pragma pack(pop)

#pragma pack(push, 1)
union StatucWordINS {
	struct data {
		unsigned Label : 8;						//Адрес
		unsigned SDI : 2;
		unsigned Preparation_ZK : 1;			//Подготовка по ЗК
		unsigned Control : 1;					//Контроль
		unsigned Navigation : 1;				//Навигация
		unsigned Gyrocompassing : 1;			//Гирокомпассирование
		unsigned RSVD1 : 1;
		unsigned Restart : 1;					//Повторный запуск
		unsigned PreparationScale : 3;			//Шкала подготовки (начальной выставки)
		unsigned ServiceabilityHeating : 1;		//Исправность обогрева
		unsigned TemperatureControl : 1;		//Термостатирование
		unsigned NoInitialData : 1;				//Нет начальный данных
		unsigned NoHeightReception : 1;			//Нет приёма абсолютной высоты
		unsigned ServiceabilityINS : 1;			//Исправность ИНС
		unsigned ReadyAcceleration : 1;			//Готовность ускорения
		unsigned Ready : 1;
		unsigned RSVD2 : 3;
		unsigned SM : 2;
		unsigned P : 1;
	} Value;
};
#pragma pack(pop)

#pragma pack(push, 1)
union Date {
	struct data {
		unsigned Label : 8;
		unsigned RSVD : 2;			//10
		unsigned Year_ed : 4;		//14
		unsigned Year_de : 4;		//18
		unsigned Month_ed : 4;		//22
		unsigned Month_de : 1;		//23
		unsigned Day_ed : 4;		//27
		unsigned Day_de : 2;		//29
		unsigned StatusMatrix : 2;	//31
		unsigned P : 1;				//32
	} Value;
};
#pragma pack(pop)

#pragma pack(push, 1)
union SRNS {
	struct data {
		unsigned Label : 8;
		unsigned RequestInitalData : 1;
		unsigned TypeSRNS : 3;
		unsigned Almanac_GPS : 1;
		unsigned Almanac_GLONASS : 1;
		unsigned OperatingMode : 2;
		unsigned OperatingSubMode : 1;
		unsigned TimeInd : 1;
		unsigned DiffMode : 1;
		unsigned Failure : 1;
		unsigned AlamThreshold : 1;
		unsigned ReferenceFrame : 2;
		unsigned StatusMatrix : 2;
		unsigned P : 1;
	} Value;
};
#pragma pack(pop)

//doesn't used
struct TMessage {
	double timelabel;
	double longitude;
	double latitude;
	int H;
	int Vput;
	double pitch;
	double roll;
	double yaw;
};