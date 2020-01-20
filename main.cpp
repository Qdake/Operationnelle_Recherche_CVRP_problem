#include <iostream>
#include <vector>
#include "Instance.h"
#include "Client.h"
#include "Solution.h"
#include "Solveur.h"
#include <string>
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
    string path;
    //instance.read_file("./test2.data");
    instance.read_file("./instance/Vrp-Set-P/P/P-n19-k2.vrp");
    //instance.read_file("./instance/Vrp-Set-P/P/P-n22-k8.vrp");
    instance.afficher();

////////////////////////////////////
///// test Solveur  MTZ
////////////////////////////////
    // cout<<"\n\n TEST Solveur  MTZ MTZ MTZ  MTZ MTZ MTZ  MTZ MTZ MTZ  MTZ MTZ MTZ  MTZ MTZ MTZ \n\n"<<endl;
    // Solveur solver;
    // solver.set_instance( & instance);
    // bool flag;
    // solver.set_method("plne_MTZ");
    // flag = solver.solve();
    // cout<<"\n********************main \n \n \n"<<endl;
    // if (flag){
    //    solver.psolution->affichage();
    // };
    // solver.psolution->visualisation();
    // solver.psolution->showpng();

////////////////////////////////////
///// test Solveur  Branch and cut
////////////////////////////////
    cout<<" TEST Solveur "<<endl;
    Solveur solver;
    solver.set_instance( & instance);
    bool flag;
    solver.set_method("plne_branch_and_cut");
    cout<<"\n **********************MAIN : plne_branch_and_cut BEGIN \n\n";
     flag = solver.solve();
    cout<<"\n********************main \n \n \n"<<endl;
    if (flag){
       solver.psolution->affichage();
    };
    cout<< "\n branch and cut  fin fin fin \n";
    solver.psolution->visualisation();
    solver.psolution->showpng();
    return 0;
};