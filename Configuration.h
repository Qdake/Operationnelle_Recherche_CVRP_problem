#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <vector>
#include "Car.h"
using namespace std;

class Configuration{
    public:
    vector<Car> cars;
    //constructeur
    Configuration();
    //destructeur
    ~Configuration();
    // constructeur de copie
    Configuration(const Configuration & configuratin);
    // to_string
    std::string to_string()const;
    // add_car
    void add_car(Car & car);
};

#endif