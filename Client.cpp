#include <iostream>
#include "Client.h"
#include "math.h"
// default constructor
Client::Client(){};
// constructeur
Client::Client( int numero_, int demande_, int x_, int y_):demande(demande_), numero( numero_),x(x_),y(y_){};
Client::Client( const Client& client):demande(client.demande),numero(client.numero),x(client.x),y(client.y){};
// toString
std::string Client::to_string()const{
    return "[numero = " + std::to_string(numero) + ", demande = " + std::to_string(demande) + ", x = " + std::to_string(x) + ", y = " + std::to_string(y) + "]";
};
// distanceTo
float Client::distanceTo(const Client & client)const{
    float res = sqrt(pow(x-client.x,2)+pow(y-client.y,2));
//    std::cout<<"x0= "<<x<<" y0= "<<y<<" x1= "<<client.x<<" y1= "<<client.y<<" res= "<<res<<std::endl;
    return res;
}
// operator<<
std::ostream & operator<<(std::ostream& out, const Client& Client){
    out<<Client.to_string();
    return out;
};

