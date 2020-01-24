#include <iostream>
#include <vector>
#include "Instance.h"
#include "Client.h"
#include "Solution.h"
#include "Solveur.h"
#include <string>
#include <iomanip>
using namespace std;
void afficher(vector<Client>& clients){
    for (int i=0; i<clients.size();i++){
        cout<<clients[i]<<endl;
        //cout<<"("<<clients[i].numero<<","<<clients[i].demande<<")"<<endl;
    };
};
int main(int argc, char** argv){
    bool DEBUG = false;
/////////////////////////
//// test Instance
/////////////////////////
    cout<<" TEST Instance "<<endl;
    Instance instance;
    Solveur solver;
    string path = argv[2];
    string method = argv[1];
    instance.read_file(path);
    instance.afficher();

////////////////////////////////////
///// test Solveur  bin_plne_gloton
////////////////////////////////
    if (method == "bin_plne_glouton"){
        cout<<"\n\n TEST Solveur  bin_plne_gloton   bin_plne_gloton   bin_plne_gloton \n\n"<<endl;
        time_t start,end;
        time(&start);
        solver.set_instance( & instance);
        bool flag;
        solver.set_method("bin_plne_heuristique");
        flag = solver.solve();
        cout<<"\n********************main \n \n \n"<<endl;
        if (flag){
            solver.psolution->affichage();
        };      
        time(&end);
        cout<< "TIME : "<<setprecision(6)<<double(end - start)<<endl;
    };
////////////////////////////////////
///// test Solveur  MTZ
////////////////////////////////
    if (method == "MTZ"){
        cout<<"\n\n TEST Solveur  MTZ MTZ MTZ  MTZ MTZ MTZ  MTZ MTZ MTZ  MTZ MTZ MTZ  MTZ MTZ MTZ \n\n"<<endl;
        solver.set_instance( & instance);
        bool flag;
        solver.set_method("plne_MTZ");
        flag = solver.solve();
        cout<<"\n********************main \n \n \n"<<endl;
        if (flag){
        solver.psolution->affichage();
        };
    };

////////////////////////////////////
///// test Solveur  Branch and cut
////////////////////////////////
    if (method == "branch_and_cut"){
        cout<<" TEST Solveur "<<endl;
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
    };
    
    
///////////////////////  VISUALISATION /////////////
    solver.psolution->write_SVG_tour();
    return 0;
};