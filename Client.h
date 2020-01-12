#ifndef Client_H
#define Client_H
#include <string>
class Client{
    public:
    int numero;
    int demande;
    int x;
    int y;
    // default constructor
    Client();
    // constructeur
    Client( int numero, int demande, int x, int y);
    // copy
    Client(const Client &);
    //toString
    std::string to_string()const;
    // distanceTo
    float distanceTo(const Client &)const;
};

// operator<<
std::ostream & operator<<(std::ostream & out, const Client & client);
#endif 