#ifndef FUZZY_H
#define FUZZY_H

#include <iostream>
#include <chrono>
#include <wiringPi.h>
#include <pcf8591.h>
#include <cmath> 	

static const u_int32_t Address = 0x48;

static const int BASE = 64;
static const int A0 = BASE+0;
static const int A1 = BASE+1;
static const int A2 = BASE+2;
static const int A3 = BASE+3;

static const int BUZZER_PIN = 4;	//GPIO4, PIN16  //LED VERDE, MOSTRA O BUZZER
static const int RESISTENCE_PIN = 1; 	//GPIO1, PIN12	//LED AMARELO. MOSTRA RESISTENCIA LIGADA OU NAO
static const int S_PWM_PIN = 3;	//GPIO3, PIN15	//LED VERMELHO. MOSTRA O CONTROLE DA RESISTENCIA. 
const int BUTTON_PIN = 0;		//GPIO0 - BOTAO DO USUARIO	


static bool _buttonState = false;
static bool _buzzer = false;
static bool _resistanceState = false;

using namespace std::chrono;

class Fuzzy {
private:

public:
	Fuzzy ();
	void setTemperature (int temp);
	//int getTemperature ();
	static int _pwmRate;
	static int _temperature;
};

static void myDelay(int value) {
	int begin = millis();
	while(millis() - begin < value) {}
}

static void myInterrupt (void) {	
    _buzzer = false;
    if (_buttonState == true) return;
   _buttonState = true;
   _resistanceState = true;
}

//TOCA O BUZZER QUANDO buzzer == true
static PI_THREAD(buzzer) {
  for (;;) {
    if (_buzzer == false) digitalWrite (BUZZER_PIN, 0); 
    else {
      digitalWrite (BUZZER_PIN, 1);
    }
  }
}

static PI_THREAD (invariant) {

  for (;;) {
    Fuzzy::_temperature = analogRead (A0);
    if (Fuzzy::_temperature < 15) {
      std::cout << "ALGO DE ERRADO COM O SENSOR DE TEMPERATURA!!!" << std::endl;
      exit (1);
    }
  }
}

static PI_THREAD(resistence) {
  for (;;) {
    if (_resistanceState == false) digitalWrite (RESISTENCE_PIN, 0); 
    else {
      digitalWrite (RESISTENCE_PIN, 1);
    }
  }
}

#endif


