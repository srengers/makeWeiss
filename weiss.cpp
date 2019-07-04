#include "weiss.h"

/*public functions*/

//std::string _systemState = "\0";

std::string _systemState = "Aperte o botao para comecar!";

int main () {
	
	WeissPan weiss;
	
	weiss.makeWeiss ();
	
	return 0;
}

static PI_THREAD (feedback) {
  for (;;)  {
	std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << std::endl;
	std::cout << _systemState << std::endl;
	if (_timeSystem == 0) std::cout << "Esse processo não depende do tempo!" << std::endl;
	else std::cout << _timeSystem << std::endl;
	std::cout << "TEMPERATURA: " <<  Fuzzy::_temperature << std::endl;
	std::cout << "BUTTON: " <<  _buttonState << std::endl;
	std::cout << "RESISTENCIA: " <<  _resistanceState << std::endl;
	std::cout << "BUZZER: " <<  _buzzer << std::endl;
	std::cout << "PWM VALUE: " <<  Fuzzy::_pwmRate << std::endl;
	
    myDelay (250);
  }	
}

WeissPan::WeissPan (): _time (steady_clock::now ()) {
	wiringPiSetup ();
				
	pcf8591Setup(BASE, Address);
		
	wiringPiISR (BUTTON_PIN, INT_EDGE_RISING, &myInterrupt);

	
	pinMode (RESISTENCE_PIN, OUTPUT);	//GPIO1, PIN12	//LED AMARELO. MOSTRA RESISTENCIA LIGADA OU NAO
	pinMode (S_PWM_PIN, OUTPUT);	//GPIO3, PIN15	//LED VERMELHO. MOSTRA O CONTROLE DA RESISTENCIA. 
	pinMode (BUZZER_PIN, OUTPUT);	//GPIO4, PIN16  //LED VERDE, MOSTRA O BUZZER
	
	piThreadCreate(buzzer);
    piThreadCreate(blinky);
	piThreadCreate(resistence);
	piThreadCreate(feedback);
	piThreadCreate(invariant);
}

//UNICA FUNCAO CHAMADA NA MAIN
void WeissPan::makeWeiss () {
	
	brassagem();
	_buttonState = false;
	fervura();
	
	return;
}

/*private functions*/

//FUNCAO QUE MODELA A BRASSAGEM.
void WeissPan::brassagem () {
	
	while (!_buttonState) {}	
	
	_systemState = "O processo de brassagem comecou!";	
		
	controlResistence (true);
	
	_systemState = "Primeira Rampa! 50 graus";
		
	ramp (FIRST_RAMP_TEMP);
	
	playBuzzer ();
	
	//adicionar malte
	
	_systemState = "Manter temperatura a 50 graus!";
	
	keepWarm (PROTEIN_TIME, FIRST_RAMP_TEMP);	
	
	_systemState = "Segunda rampa a 66 graus!";

	ramp (SECOND_RAMP_TEMP);
	
	_systemState = "Manter temperatura a 66 graus!";
	
	keepWarm (SACCHARIFICATION_TIME, SECOND_RAMP_TEMP);
	
	_systemState = "Terceira rampa a 75 graus!";
	
	ramp (THIRD_RAMP_TEMP);
	
	_systemState = "Manter a temperatura a 75 graus!";
	
	keepWarm (MASHOUT_TIME, THIRD_RAMP_TEMP);
	
	controlResistence (false);

	_systemState = "Apenas mexendo o mosto!";

	coolDown (MOSTO_TIME);
}

//FUNCAO QUE MODELA A FERVURA
void WeissPan::fervura () {
	
	_systemState = "Aperte o botao para comecar a fervura!";	
	
	while (!_buttonState) {}
	
	_systemState = "Rampa até ferver! 100 graus";	
	
	ramp (MAX_TEMP);
	
	_systemState = "Manter fervendo! 100 graus";	
	
	keepWarm (HOLD_TIME1, MAX_TEMP);
	
	playBuzzer ();
	
	//adicionar lupulo 10g
	
	_systemState = "Adicionar 10g Lupulo e manter fervendo! 100 graus";
	
	keepWarm (HOLD_TIME2, MAX_TEMP);
	
	playBuzzer ();
	
	_systemState = "Adicionar 20g Lupulo e manter fervendo! 100 graus";
	
	//adicionar lupulo 20g

	keepWarm (HOLD_TIME3, MAX_TEMP);
	
	_systemState = "Manter apenas mexendo ate esfriar!";
	
	controlResistence (false);
	
	coolDown (CHILLING_TIME);
}

//APOS A RESISTENCIA DESLIGADA, CONTINUA CONTANDO UM TEMPO PARA FICAR MEXENDO A PA DA PANELA.
//TROCAR O TEMPO DE SEGUNDOS (PARA TESTE) PARA MINUTOS
void WeissPan::coolDown (int time) {
	
	steady_clock::time_point initialTime = steady_clock::now ();
	
	_timeSystem = time - duration_cast <seconds> (steady_clock::now () - initialTime).count();
	
	while (_timeSystem >= 0) {
		_timeSystem = time - duration_cast <seconds> (steady_clock::now () - initialTime).count();
		Fuzzy::_temperature = analogRead (A0);
	}
}

//FAZ A RAMPA PARA A TEMPERATURA DESEJADA
void WeissPan::ramp (float temperature) {
	
	_timeSystem = 0;
	
	while (Fuzzy::_temperature < temperature) {
		//Fuzzy::_temperature = analogRead (A0);
		fuzzy.setTemperature (temperature);
	}
}

//MANTEM A TEMPERATURA PASSADA POR PARAMETRO
//TROCAR O TEMPO DE SEGUNDOS (PARA TESTE) PARA MINUTOS
void WeissPan::keepWarm (int time, int temperature) {
	
	steady_clock::time_point initialTime = steady_clock::now ();
	
	_timeSystem = time - duration_cast <seconds> (steady_clock::now () - initialTime).count();
	
	while (_timeSystem >= 0) {
		//Fuzzy::_temperature = analogRead (A0);
		_timeSystem = time - duration_cast <seconds> (steady_clock::now () - initialTime).count();
		if (Fuzzy::_temperature != temperature) {
			fuzzy.setTemperature (temperature);		
		}		
	}
	
}

//DESLIGA E LIGA A RESISTENCIA
void WeissPan::controlResistence (bool state) {
	_resistanceState = state;
}

//TOCA O BUZZER
void WeissPan::playBuzzer () {
	_buzzer = true;
}

