#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <vector>

class Configuration{
    public:
    std::vector<std::vector<int>> cars;
    //constructeur
    Configuration();
    //constructeur
    Configuration(int n,int m);
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