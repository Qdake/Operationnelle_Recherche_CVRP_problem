#ifndef INSTANCE_H
#define INSTANCE_H
#include <vector>
#include <string>
#include "Client.h"
class Instance{
    public:
    int n;  // nb clients
    float UB;//upper bound
    float Q; //capacity
    std::vector< Client> clients;
    // constructeur
    Instance();
    // destructeur
    ~Instance();
    // read_file
    bool read_file(std::string path);
    // afficher
    void afficher()const;
};
#endif