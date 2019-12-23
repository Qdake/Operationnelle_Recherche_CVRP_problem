#include <iostream>
#include <vector>
#include "Instance.h"
#include "Client.h"
#include "Car.h"
#include "Configuration.h"
#include "Solveur.h"
using namespace std;
void afficher(vector<Client>& clients){
    for (int i=0; i<clients.size();i++){
        cout<<clients[i]<<endl;
        //cout<<"("<<clients[i].numero<<","<<clients[i].demande<<")"<<endl;
    };
};
int main(){
/////////////////////
//// test  Client  Car Configuration
//////////////////////

    // int demandes[]={ 2, 2, 2, 3, 3, 4};
    // int Q = 50;
    // int k = 2;
    // vector<Client> clients;
    // for (int i=0; i<6; i++){
    //     clients.push_back(Client(demandes[i],i,i,i));
    // };
    // afficher(clients);

    // Car car(Q);
    // car.add_client(&clients[0]);
    // car.add_client(&clients[1]);
    // car.add_client(&clients[2]);
    // cout<<car.add_client(&clients[3])<<endl;
    // car.add_client(&clients[4]);
    // cout<<car.add_client(&clients[5])<<endl;
    // cout<<car.to_string();
    // Car car1(8.0);
    // car1.add_client(&clients[0]);
    // car1.add_client(&clients[3]);
    
    // Configuration config;
    // config.add_car(car1);
    // config.add_car(car);
    // cout<<"***";
    // cout<<config.to_string()<<endl;

/////////////////////////
//// test Instance
/////////////////////////
    cout<<" TEST Instance "<<endl;
    Instance instance;
    instance.read_file("./test.data");
    instance.afficher();
    
    cout<<" debut Instance"<<endl;

////////////////////////////////////
///// test Solveur
////////////////////////////////
     cout<<" TEST Solveur "<<endl;
     Solveur solver;
    solver.set_instance( & instance);
     solver.set_method("bin_plne_heuristique");
    solver.solve();
    return 0;
};