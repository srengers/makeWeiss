#include "fuzzy.h"

int Fuzzy::_pwmRate = 0;
int Fuzzy::_temperature = analogRead (A0);

Fuzzy::Fuzzy () {


} 

//MUDAR ISSO PRA ALGO DECENTE
void Fuzzy::setTemperature (int temp) {
		if (Fuzzy::_temperature > temp){
			Fuzzy::_temperature -=  3 * fabs ((Fuzzy::_temperature - temp)/temp);
			_pwmRate = 0;
		}
		if (Fuzzy::_temperature < temp){
			Fuzzy::_pwmRate = 30* (temp - Fuzzy::_temperature)/temp;
			Fuzzy::_temperature += 3 * fabs ((temp - Fuzzy::_temperature)/temp);
		}
}

/*int Fuzzy::getTemperature () {
	//RECEBER TEMPERATURA DO TERMOMETRO, SETAR EM _temperature
	return _temperature;
}*/
