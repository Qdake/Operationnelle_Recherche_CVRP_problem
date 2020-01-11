#include <iostream>
#include <vector>
#include "Instance.h"
#include "Client.h"
#include "Solution.h"
//#include "Solveur.h"
using namespace std;
void afficher(vector<Client>& clients){
    for (int i=0; i<clients.size();i++){
        cout<<clients[i]<<endl;
        //cout<<"("<<clients[i].numero<<","<<clients[i].demande<<")"<<endl;
    };
};
int main(){
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
    // cout<<" TEST Solveur "<<endl;
    // Solveur solver;
    // solver.set_instance( & instance);
    // solver.set_method("bin_plne_min_heuristique");
    // solver.solve();
    // printf("\n********************main\n");
    // solver.psolution->affichage();
    return 0;
};