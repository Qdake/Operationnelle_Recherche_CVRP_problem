#ifndef CAR_H
#define CAR_H
#include <string>
#include <vector>
#include "Client.h"
using namespace std;
class Car{
    public:
    vector<Client* > pclients;
    float demande;
    float capacite;
    //constructeur
    Car(float capacite);
    //destructeur
    ~Car();
    //constructeur de copie
    Car(const Car& car);
    // toString
    std::string to_string()const;
    // add_client
    bool add_client(Client * pclient);
};
#endif