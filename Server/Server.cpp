#include "TServer.h"
#include <fstream>
#include <chrono>
#include "../P_INFO.h"
#include <map>

using namespace std;
using namespace chrono;

int main(){
    system("title UDP Server");

    TServer server;
    server.set_port(12346);
    server.Creat_Socket();

    ofstream log("../Log_from_Server.txt");
    BNR Message;
    
    P_INFO temp, time_UTC;
    int bincode = 0;
    StatucWordINS SW_INS;
    SRNS SW_SNS;
    Date _Date;
    time_point<steady_clock> Tend = steady_clock::now(), Tbegin = steady_clock::now();

    map<int, P_INFO> adress_names = {
        {0, P_INFO("NaN", 0, 0)},
        {200, P_INFO("INS:Latitude", 20, 90)},                          {201, P_INFO("INS:Longitude", 20, 90)},                 {241, P_INFO("INS:Height", 19, 19975.3728)},
        {204, P_INFO("INS:True_course", 16, 90)},                       {212, P_INFO("INS:Pitch", 16, 90)},                     {213, P_INFO("INS:Roll", 16, 90)},
        {246, P_INFO("INS:Velocity north", 19, 1053.5822)},             {247, P_INFO("INS:Velocity east", 19, 1053.5822)},      {245, P_INFO("INS:Velocity vertical", 19, 83.2307)},
        {217, P_INFO("INS:Acceleration X", 12, 19.62)},                 {218, P_INFO("INS:Acceleration Z", 12, 19.62)},         {219, P_INFO("INS:Acceleration Y", 12, 19.62)},
        {62, P_INFO("SNS:Height", 20, 65536)},                          {65, P_INFO("SNS:HDOP", 15, 512)},                      {66, P_INFO("SNS:VDOP", 15, 512)},
        {67, P_INFO("SNS:WayAngle", 15, 90)},                           {72, P_INFO("SNS:Latitude", 20, 90)},                   {80, P_INFO("SNS:Latitude_tchn", 11, 0.00085830)},
        {73, P_INFO("SNS:Longitude", 20, 90)},                          {81, P_INFO("SNS:Longitude_tchn", 11, 0.00085830)},     {75, P_INFO("SNS:Zadergka", 20, 512)},
        {96, P_INFO("SNS:UTC_ml", 20, 512)},                            {117, P_INFO("SNS:Vh", 16, 16384)}
    };
    while (true){

        Message = server.Receive();
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        Tend = steady_clock::now();
        memcpy(&bincode, &Message, sizeof(Message));
        cout << "Adress: " << Message.Value.Label << endl;

        switch (Message.Value.Label){
        case 184:   //SW_INS
            log << "\nТекущее время: " << asctime(localtime(&in_time_t));
            memset(&SW_INS, 0, sizeof(StatucWordINS));
            memcpy(&SW_INS, &Message, sizeof(Message));
            log << "Слово состояния ИНС:\n";
            log << "SDI: " << SW_INS.Value.SDI << endl;
            log << "Подготовка по ЗК: " << SW_INS.Value.Preparation_ZK << endl;
            log << "Контроль: " << SW_INS.Value.Control << endl;
            log << "Навигация: " << SW_INS.Value.Navigation << endl;
            log << "Гирокомпассирование: " << SW_INS.Value.Gyrocompassing << endl;
            log << "Повторный запуск: " << SW_INS.Value.Restart << endl;
            log << "Шкала подготовки: " << SW_INS.Value.PreparationScale << endl;
            log << "Исправность обогрева: " << SW_INS.Value.ServiceabilityHeating << endl;
            log << "Термостатирование: " << SW_INS.Value.TemperatureControl << endl;
            log << "Нет начальных данных: " << SW_INS.Value.NoInitialData << endl;
            log << "Нет приёма Habs: " << SW_INS.Value.NoHeightReception << endl;
            log << "Исправность ИНС: " << SW_INS.Value.ServiceabilityINS << endl;
            log << "Готовность ускоренная: " << SW_INS.Value.ReadyAcceleration << endl;
            log << "Готовность: " << SW_INS.Value.Ready << endl;
            break;
        case 176:   //Date
            memset(&_Date, 0, sizeof(Date));
            memcpy(&_Date, &Message, sizeof(Message));
            log << "Слово 'ДАТА':\n";
            log << "День: " << _Date.Value.Day_de * 10 + _Date.Value.Day_ed << endl;
            log << "Месяц: " << _Date.Value.Month_de * 10 + _Date.Value.Month_ed << endl;
            log << "Год: " << _Date.Value.Year_de * 10 + _Date.Value.Year_ed << endl;
            break;
        case 187:   //SW_SNS
            log << "\nТекущее время: " << asctime(localtime(&in_time_t));
            memset(&SW_SNS, 0, sizeof(SRNS));
            memcpy(&SW_SNS, &Message, sizeof(Message));
            log << "Слово состояния СНС:\n";
            log << "Запрос начальных данных: " << SW_SNS.Value.RequestInitalData << endl;
            log << "Тип рабочей СРНС: " << SW_SNS.Value.TypeSRNS << endl;
            log << "Альманах GPS: " << SW_SNS.Value.Almanac_GPS << endl;
            log << "Альманах ГЛОНАСС: " << SW_SNS.Value.Almanac_GLONASS << endl;
            log << "Режим работы: " << SW_SNS.Value.OperatingMode << endl;
            log << "Подрежим работы: " << SW_SNS.Value.OperatingSubMode << endl;
            log << "Признак времени: " << SW_SNS.Value.TimeInd << endl;
            log << "Дифф. режим измерений: " << SW_SNS.Value.DiffMode << endl;
            log << "Отказ изделия: " << SW_SNS.Value.Failure << endl;
            log << "Порог сигнализации: " << SW_SNS.Value.AlamThreshold << endl;
            log << "Система координат: " << SW_SNS.Value.ReferenceFrame << endl;
            break;
        case 104:   //UTC_st
            bincode = Message.Value.D;
            temp = P_INFO("Chas", 5, 16); temp.set_binValue(bincode >> 12);
            log << "SNS:UTC_st: " << temp.get_Value();
            temp = P_INFO("Minuta", 6, 32); temp.set_binValue((bincode >> 6) & 63);
            log << ":" << temp.get_Value();
            temp = P_INFO("Secunda", 6, 32); temp.set_binValue(bincode & 63);
            log << ":" << temp.get_Value() << endl;
            break;
        default:
            temp = (adress_names.find(Message.Value.Label)->second);
            temp.set_bincode(bincode);
            log << temp.get_name() << ": " << temp.get_Value() << endl;
            break;
        }

    }
    log.close();
    server.Clean_n_Close();
}

