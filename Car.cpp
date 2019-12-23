#include <vector>
#include "Car.h"
//constructeur
Car::Car(){};
//destructeur
Car::~Car(){};
//constructeur de copie
Car::Car(const Car& car){
        demande = car.demande;
        capacite = car.capacite;
        pclients = car.pclients;
};
// toString
std::string Car::to_string()const{
    std::string s = "(capacite: " + std::to_string(capacite) + ", demande: " + std::to_string(demande) + ")\n";
    for (const Client * pclient : pclients){
        s += pclient->to_string() + "\n";
    };
    return s;
}
//
bool Car::add_client(Client * pclient){
    if (demande + pclient->demande <= capacite){ 
        pclients.push_back(pclient);
        demande += pclient->demande;
        return true;
    };
    return false;
}
// operator<<
std::ostream & operator<<(std::ostream& out, const Car& car){
    out<<car.to_string();
    return out;
};