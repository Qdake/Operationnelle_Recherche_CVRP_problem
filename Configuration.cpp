#include "Configuration.h"
#include <string>
#include <vector>
//constructeur
Configuration::Configuration(){};
//destructeur
Configuration::~Configuration(){};
//constructeur de copie
Configuration::Configuration(const Configuration & configuration):cars(configuration.cars){};
// to_string
std::string Configuration::to_string()const{
    string s = "Configuration\n";
    for (const Car & car: cars){
        s += car.to_string() + "\n";
    };
    return s;
};
// add_car
void Configuration::add_car(Car & car){
    cars.push_back(car);
};