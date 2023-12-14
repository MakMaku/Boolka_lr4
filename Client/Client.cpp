#include "TClient.h"
#include "TAircraft.h"
#include <chrono>
#include "Struct.h"

using namespace std;
using namespace chrono;


void thread_try(milliseconds delay, int workTime) {
    double time = 0;

    TClient* INS = new TINS();
    INS->set_port(12346);
    INS->set_ipaddres("127.0.0.1");
    INS->Creat_Socket();

    time_point<steady_clock> Tend = steady_clock::now(), Tbegin = steady_clock::now();
    while (time <= workTime) {
        time = duration<double>(Tend - Tbegin).count();
        
        INS->Working();
        INS->Sending();

        Tend = steady_clock::now();
        this_thread::sleep_for(milliseconds(delay));
    }

    INS->Clean_n_Close();
}

int main(){
    system("title UDP Client");
    
    TAircraft Aircraft(37.2, 55.6, 0, 0, 0, "../Around Moscow.kml", 160, 200);

    Aircraft.StartAircraft();
}
