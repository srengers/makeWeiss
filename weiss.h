
/*
 * 	CREATED BY CHATUBA DEVELOPS.
 * 
 * 
 *
 * 										*/
 
 
#ifndef WEISS_H
#define WEISS_H
 
#include "fuzzy.h"
#include <string>

int _timeSystem = 0;

/*keep calm and make weiss*/
class WeissPan {
public:
	WeissPan ();
	//~WeissPan ();
	void makeWeiss ();
//	static std::string _systemState;
	
private:
	void ramp (float temperature);
	void keepWarm (int time, int temperature);
	void controlResistence (bool state);
	void coolDown (int time);
	void playBuzzer ();
	void brassagem ();
	void fervura ();
	
	
	
	/*variables*/
	steady_clock::time_point _time;	
	Fuzzy fuzzy;
	
	//BRASSAGEM
	const int PROTEIN_TIME = 30;	//minutes
	const int SACCHARIFICATION_TIME = 45;	//minutes
	const int MASHOUT_TIME = 10;	//minutes
	const int MOSTO_TIME = 15;		//minutes
	const int MAX_TEMP = 100;		//degrees
	const int FIRST_RAMP_TEMP = 50;
	const int SECOND_RAMP_TEMP = 66;
	const int THIRD_RAMP_TEMP = 75;
	
	//FERVURA
	const int HOLD_TIME1 = 30;
	const int HOLD_TIME2 = 25;
	const int HOLD_TIME3 = 5;
	const int CHILLING_TIME = 25;	//minutes
};

static PI_THREAD(blinky) {
  for (;;)  {
    if (Fuzzy::_pwmRate != 0) {
      digitalWrite (3, 1); 
      myDelay (10*Fuzzy::_pwmRate);
      _resistanceState = true;
    }
    else _resistanceState = false;
    //std::cout << "Valor eh: " << Fuzzy::_pwmRate << std::endl;
    digitalWrite (3, 0); 
    myDelay (1000 - 10*Fuzzy::_pwmRate);
  }
}

#endif
