#ifndef CAR_H
#define CAR_H
#include <string>
#include <vector>
#include "Client.h"
using namespace std;
class Car{
    public:
    vector<Client* > pclients;
    int demande;
    int capacite;
    //constructeur par defaut
    Car(){};
    //constructeur
    Car(int capacite);
    //destructeur
    ~Car();
    //constructeur de copie
    Car(const Car& car);
    // toString
    std::string to_string()const;
    // add_client
    bool add_client(Client * pclient);
};
// operator<<
std::ostream & operator<<(std::ostream& out, const Car& car);

#endif