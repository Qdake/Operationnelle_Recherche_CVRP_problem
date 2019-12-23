#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <assert.h>
#include "Client.cpp"
int main(){
    std::vector<Client> clients;


    std::vector<std::string> string_list;
    std::string line;
    std::ifstream infile;
    //infile.open("test.data");
    in.open(path,std::fstream::in);
    if ( ! infile.is_open()){
        std::cout<<"Error when opening file"; exit(1);
    };
    // instance name
    std::getline(infile,line);
    std::cout<<line<<std::endl;
    boost::algorithm::split(string_list,line,boost::algorithm::is_any_of("-nk"));
    int n = std::stoi(string_list[2]);
    int m = std::stoi(string_list[4]);
    std::cout<<"n="<<n<<"  m="<<m<<std::endl;
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
    int Q = std::stoi(string_list[string_list.size()-1]);
    std::cout<<"capacity ="<<Q<<std::endl;
    // NODE_COORD_SECTION
    std::getline(infile,line);
    for (int i=0; i<n; i++){
        clients.push_back(Client());
        infile>>clients[i].numero>>clients[i].x>>clients[i].y;
    };
    std::cout<<"n="<<n<<std::endl;
    // DEMANDE_SECTION
    std::getline(infile,line);
    std::getline(infile,line);
    for (int i=0; i<n; i++){
        std::cout<<i<<" : ";
        infile>>clients[i].numero>>clients[i].demande;
        std::cout<<clients[i].demande;
    };
    // afficher
    std::cout<<n<<std::endl;
    for (int i=0; i<n; i++){
        std::cout<<clients[i]<<std::endl;
    };

    infile.close();
    return 0;
};