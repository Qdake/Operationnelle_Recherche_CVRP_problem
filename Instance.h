#ifndef INSTANCE_H
#define INSTANCE_H
#include <vector>
#include <string>
#include "Client.h"
class Instance{
    public:
    int m;   // no of trucks
    int n;  // nb clients
    float UB;//upper bound
    float Q; //capacity
    std::vector< Client> clients;
    // constructeur
    Instance();
    // destructeur
    ~Instance();
    // get_m
    int get_m()const;
    //get_n
    int get_n() const;
    //get_Q
    int get_Q() const;
    //get_clients()const;
    std::vector<Client> get_clients()const;
    // read_file
    bool read_file(std::string path);
    // afficher
    void afficher()const;
};
#endif