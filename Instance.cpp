#include <boost/algorithm/string.hpp>
#include <assert.h>
#include <iostream>
#include <vector>
#include <fstream>
#include "Client.h"
#include "Instance.h"
// constructeur
Instance::Instance(){};
// destructeur
Instance::~Instance(){};
// get_m
int Instance::get_m()const{
    return m;
};
//get_n
int Instance::get_n() const{
    return n;
};
//get_Q
int Instance::get_Q() const{
    return Q;
};
//get_clients()const;
std::vector<Client> Instance::get_clients()const{
    return clients;
};


// read file
bool Instance::read_file(std::string path){

    std::vector<std::string> string_list;
    std::string line;
    std::ifstream infile;
    //infile.open("path");
    infile.open(path);
    if ( ! infile.is_open()){
        std::cout<<"Error when opening file"; exit(1);
    };
    // instance name
    std::getline(infile,line);
    //std::cout<<line<<std::endl;
    boost::algorithm::split(string_list,line,boost::algorithm::is_any_of("-nk"));
    this->n = std::stoi(string_list[2]);
    this->m = std::stoi(string_list[4]);
    //std::cout<<"n="<<n<<"  m="<<m<<std::endl;
    // No of trucks 
    std::getline(infile,line);
    // TYPE 
    std::getline(infile,line);
    // DIMENSION
    std::getline(infile,line);
    boost::algorithm::split(string_list,line,boost::algorithm::is_any_of(" "));
    // EDGE_WEIGHT_TYPE
    std::getline(infile,line);
    // CAPACITY
    std::getline(infile,line);
    boost::algorithm::split(string_list, line, boost::algorithm::is_any_of(" "));
    Q = std::stoi(string_list[string_list.size()-1]);
    //std::cout<<"capacity ="<<Q<<std::endl;
    // NODE_COORD_SECTION
    std::getline(infile,line);
    for (int i=0; i<n; i++){
        clients.push_back(Client());
        infile>>clients[i].numero>>clients[i].x>>clients[i].y;
    };
    //std::cout<<"n="<<n<<std::endl;
    // DEMANDE_SECTION
    std::getline(infile,line);
    std::getline(infile,line);
    for (int i=0; i<n; i++){
        //std::cout<<i<<" : ";
        infile>>clients[i].numero>>clients[i].demande;
        //std::cout<<clients[i].demande;
    };

///////////////////////////////
////   cout entre villes
/////////////////////////////
    distance.resize(n);
    for (int i=0; i<n; i++){
        distance[i].resize(n);
    };
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            distance[i][j] = clients[i].distanceTo(clients[j]);
            //std::cout<<distance[i][j]<<" ";
        };
        std::cout<<std::endl;
    };


    return true;
}

// afficher
void Instance::afficher()const{
    std::cout<<"n = "<<n<<"  m = "<<m<<std::endl;
    std::cout<<"UB = "<<UB<<std::endl;
    std::cout<<"Q = "<<Q<<std::endl;
    for (int i=0; i<n; i++){
        std::cout<<clients[i]<<std::endl;
    };
}
