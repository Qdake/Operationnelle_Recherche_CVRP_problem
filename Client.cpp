#include <iostream>
#include "Client.h"
// default constructor
Client::Client(){};
// constructeur
Client::Client( int numero_, float demande_, float x_, float y_):demande(demande_), numero( numero_),x(x_),y(y_){};
// toString
std::string Client::to_string()const{
    return "[" + std::to_string(demande) + "," + std::to_string(numero) + "," + std::to_string(x) + "," + std::to_string(y) + "]";
}
// operator<<
std::ostream & operator<<(std::ostream& out, const Client& Client){
    out<<Client.to_string();
    return out;
};

