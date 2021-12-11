#include <simlib.h>
#include <iostream>
#include "ims-project.hpp"

using namespace std;

Facility StartYear("StartYear");
Facility EndYear("EndYear");
int yearCnt = 2018;

unsigned long electric_cars = DEFAULT_ELECTRIC_CARS_NUM;
unsigned long ev_consumption = ONE_YEAR_EV_CONSUMPTION;
unsigned long year_ev_consumption = 0;

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
	year_ev_consumption = electric_cars * ev_consumption;     
        for (unsigned long i = 0; i < electric_cars; i++) {
            if (too_old_car(TOO_OLD_ELECTRIC_CAR)) {	// stare auto = kupa noveho
                electric_cars--;
 
                for (int j = 0; j < 2; j++) {
                        electric_cars++;
                }
            } else {
                if (this->too_old_battery()) {
			year_ev_consumption += 5130; 	// vymena baterii v aute - 1kwh = 57kwh of energy, EV = 90kwh batteries, 90 * 57 = 5130kwh for manufacturing of batteries
		}
            }
        }
	cout << "- year EV consumption:    " << year_ev_consumption << " kWh" << endl;
        Passivate();
        
        goto newYearElectric;
    }


    bool too_old_battery() {
    if (Random() < TOO_OLD_BATTERY) {	// SIMLIB funkcia Random = normalne rozdelenie, hodnoty <0,1)
        return true;
    } else {
        return false;
    }
}
        

};




class YearTimer: public Event {

public:
    ElectricCar* e_car = new ElectricCar;

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

    return 0;
}