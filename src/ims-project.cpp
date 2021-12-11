#include <simlib.h>
#include <getopt.h>
#include <iostream>
#include "ims-project.hpp"

using namespace std;

Facility StartYear("StartYear");
Facility EndYear("EndYear");
int yearCnt = 2018;

unsigned long electric_cars = DEFAULT_ELECTRIC_CARS_NUM;

bool too_old_car(double type) {
    if (Random() > type) {
        return true;
    } else {
        return false;
    }
}

class ElectricCar: public Process {

public:
    void Behavior() {
        Priority = 2;

        newYearElectric:
        
        for (unsigned long i = 0; i < electric_cars; i++) {
            if (too_old_car(TOO_OLD_ELECTRIC_CAR)) {
                electric_cars--;

                for (int j = 0; j < 2; j++) {
                        electric_cars++;
                }
            } else {
                if (this->too_old_battery()) {
			// zvysenie energie na vyrobu novej baterie pre nove auto
                }
            }
        }

        Passivate();
        
        goto newYearElectric;
    }


    bool too_old_battery() {
    if (Random() < TOO_OLD_BATTERY) {
        return true;
    } else {
        return false;
    }
}
        

};




class YearTimer: public Event {

public:
    ElectricCar* e_car = new ElectricCar;
    Stat* CO2Stat = new Stat("Year CO2");

    void Behavior() {
        e_car->Activate();

        
        cout << "-------------------------------------------------------------" << endl;
        cout << "---                  Year " << yearCnt++ << "                            ---" << endl;
        cout << "- Number of electric cars:    " << electric_cars << endl;
        
        Activate(Time + NEXT_YEAR);
    }

private:
    const double NEXT_YEAR = 1.0;
};


int main(){
    
    double length_od_sim = DEFAULT_LENGTH;

    Init(SIM_START, length_od_sim);
    (new YearTimer)->Activate();  
    Run();

    SIMLIB_statistics.Output();

    return 0;
}