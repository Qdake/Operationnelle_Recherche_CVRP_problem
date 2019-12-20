#include <iostream>
#include <vector>
#include <fstream>
#include "Client.h"
#include "Instance.h"
// constructeur
Instance::Instance(){};
// destructeur
Instance::~Instance(){};

// read file
bool Instance::read_file(std::string path){
    std::ifstream in;
    //in.open("Vrp-Set-tai/tai/tai75a.dat");
    in.open(path);
    if ( ! in.is_open()){
        std::cout<<"Error when opening file"; exit(1);
    };
    // n Q B
    in>>n>>UB>>Q;
    // depot
    float x,y,d;
    int k;
    in>>x>>y;
    clients.push_back(Client(0, 0, x, y));
    // clients
    while (!in.eof()){
        in>>k>>d>>x>>y;
        clients.push_back(Client(k,d,x,y));
    };
    in.close();
    return true;
}

// afficher
void Instance::afficher()const{
    std::cout<<"n  "<<n<<std::endl;
    std::cout<<"UB "<<UB<<std::endl;
    std::cout<<"Q  "<<Q<<std::endl;
    for (int i=0; i<n+1; i++){
        std::cout<<clients[i]<<std::endl;
    };
}
