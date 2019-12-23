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
    //toString
    std::string to_string()const;
};

// operator<<
std::ostream & operator<<(std::ostream & out, const Client & client);

#endif 