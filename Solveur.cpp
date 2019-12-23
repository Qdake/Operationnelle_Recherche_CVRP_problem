#include <iostream>
#include <string>
#include "Solveur.h"
#include "Instance.h"
#include "ilcplex/ilocplex.h"

#define DEBUG

// set_instance
void Solveur::set_instance(Instance* pinstance_){
    pinstance = pinstance_;
};


// set_method
void Solveur::set_method(string methode_){
    if (methode_ == "bin_plne_heuristique"){
        pmethode = & Solveur::bin_plne_heuristique;
        return;
    };
    if (methode_ == "bin_glouton_heuristique"){
        pmethode = & Solveur::bin_glouton_heuristique;
        return;
    };
};


// solve
bool Solveur::solve(){
    bool r = (this->*pmethode)();
    return r;
};

bool Solveur::bin_plne_heuristique(){
////////////////////////
////   INIT
////////////////////////
    int n = this->pinstance->get_n();  // nb client
    int m = this->pinstance->get_m();   // nb vehicle
    int Q = this->pinstance->get_Q();
    std::vector<Client> clients = this->pinstance->get_clients();

    IloEnv env;
////////////////////////////////
///   MODEL
/////////////////////////////////
    IloModel model(env);
    ///////////////////////
    /////  Var
    //////////////////////
    std::vector<std::vector<IloNumVar>> x;
    x.resize(n);               //x[i][j] client i in car j
    for (int i{0}; i<n; i++){
        x[i].resize(m);
        std::ostringstream varname;
        for (int j{0}; j<m; j++){
            x[i][j]=IloNumVar(env, 0.0, 1.0, ILOINT);
            varname.str("");
            varname<<"x["<<i<<"]["<<j<<"]";
            x[i][j].setName(varname.str().c_str());
        };
    };
    /////////////////////////
    //////// CST
    /////////////////////////
    IloRangeArray CC(env);
    int nbcst{0};
    // Cst capacity :   sum(x_i_j,i=1..n-1)<=Q     for j in 0..m-1
    for (int j{0}; j<m; j++){
        IloExpr cst(env);
        for (int i{1}; i<n; i++)
            cst += x[i][j]*clients[i].demande;
        CC.add(cst<=Q);
        
        std::ostringstream cstname;
        cstname.str("");
        cstname<<"Cst_capacity_car_"<<j;
        CC[nbcst].setName(cstname.str().c_str());
        nbcst++;
    };
    // Cst :  sum(x_i_j,j=1..k)==1     for i in 1..n
    for (int i{0}; i<n; i++){
        IloExpr cst(env);
        for (int j{0}; j<m; j++)
            cst += x[i][j];
        CC.add(cst<=1);
        CC.add(cst>=1);

        std::ostringstream cstname;
        cstname.str("");
        cstname<<"Cst_x_"<<i<<"_*";
        CC[nbcst].setName(cstname.str().c_str());
        CC[nbcst+1].setName(cstname.str().c_str());
        nbcst += 2;
    };
    // add Cst
    model.add(CC);

    /////////////////////////////////////
    ////////  Obj
    /////////////////////////////////////
    IloObjective obj=IloAdd(model,IloMaximize(env,0.0));
    for (int i{0}; i<n; i++)
        for (int j{0}; j<m; j++)
            obj.setLinearCoef(x[i][j],0);
//////////////////////////////////
///////  Resolution
/////////////////////////////////
    IloCplex cplex(model);
    ////////////////////////////////////
    //////// get solution
    ////////////////////////////////////
    cplex.exportModel("sortie.lp");
    
    if ( !cplex.solve()){
       return false;
    };

    env.out()<<"Solution status = " <<cplex.getStatus()<<std::endl;
    env.out()<<"Solution value = " <<cplex.getObjValue()<<std::endl;

    std::vector<std::vector<int>> solx;
    solx.resize(n,std::vector<int>(m));
    std::vector<Car> cars;
    cars.resize(m);
    for (int j{0}; j<m; j++){
        cars[j].pclients.resize(n);
//        std::cout<<"cars[j].clients.size() apres clear =" <<cars[j].clients.size()<<" "<<std::endl;
//        #ifdef DEBUG
//        std::cout<<"car "<<j<<": "<<std::endl;
//        #endif
        for (int i{0}; i<n; i++){     
            if (cplex.getValue(x[i][j]) == 1){
                std::cout<<"("<<i<<","<<j<<")"<<std::endl;
                cars[j].add_client(&(clients[i]));
//                #ifdef DEBUG
//                std::cout<<"add success? "<<success<<std::endl;
//                std::cout<<"cars[j].clients.size() apres add_client =" <<cars[j].clients.size()<<" "<<std::endl;
//                std::cout<<clients[i].demand<<" ";
//                #endif
            };  
        };
        #ifdef DEBUG
            std::cout<<std::endl;
        #endif
    };
        
//////////////////////////////////////////
///////  Cplex's ending
//////////////////////////////////////////
    env.end();
/////////////////////////////////////////
//////////   output
//////////////////////////////////////////
    std::cout<<" OUTPUT"<<std::endl;
    return true;
}


// bin_plne_heuristique
// bool Solveur::bin_plne_heuristique(){

//     int n = this->pinstance->get_n();  // nb client
//     int m = this->pinstance->get_m();   // nb vehicle
//     int Q = this->pinstance->get_Q();
//     std::vector<Client> clients = this->pinstance->get_clients();


//     std::cout<<"n "<<n<< " m "<<m<<std::endl;
//     for (int i{0}; i<n; i++)
//         std::cout<<pinstance->clients[i].demande<<std::endl;
        
        

//     ///////////////////
//     /////// Cplex initialization
//     ////////////////////
//     IloEnv env;
//     IloModel model(env);
//     ///////////////////////
//     /////  Var
//     //////////////////////
//     std::vector<std::vector<IloNumVar>> x;
//     x.resize(n);               //x[i][j] client i in car j
//     for (int i{0}; i<n; i++){
//         x[i].resize(m);
//         std::ostringstream varname;
//         for (int j{0}; j<m; j++){
//             x[i][j]=IloNumVar(env, 0.0, 1.0, ILOINT);
//             varname.str("");
//             varname<<"x["<<i<<"]["<<j<<"]";
//             x[i][j].setName(varname.str().c_str());
//         };
//     };
//     /////////////////////////
//     //////// CST
//     /////////////////////////
//     IloRangeArray CC(env);
//     int nbcst{0};

//     // Cst capacity :   sum(x_i_j*client[i].demande,i=1..n)<=Q     for j in 1..m
//     // la somme des demandes des clients d'une voiture <= Q
//     for (int j{0}; j<m; j++){
//     //pour tous les voitures
//         IloExpr cst(env);
//         for (int i{1}; i<n; i++)
//         //la somme des demandes des clients de la voiture j    
//             cst += x[i][j] * this->pinstance->clients[i].demande;
//         CC.add(cst<=pinstance->Q); // < Q        
//         std::ostringstream cstname;
//         cstname.str("");
//         cstname<<"Cst_capacity_car_"<<j;
//         CC[nbcst].setName(cstname.str().c_str());
//         nbcst++;
//     };

//     // // Cst :  sum(x_i_j,j=0..m-1)==1     for i in 0 ..n-1
//     // for (int i{1}; i<n; i++){
//     //     IloExpr cst(env);
//     //     for (int j{0}; j<m; j++)
//     //         cst += x[i][j];
//     //     CC.add(cst<=1);
//     //     //CC.add(cst>=1);

//     //     std::ostringstream cstname;
//     //     cstname.str("");
//     //     cstname<<"Cst_x_"<<i<<"_*";
//     //     CC[nbcst].setName(cstname.str().c_str());
//     //     //CC[nbcst+1].setName(cstname.str().c_str());
//     //     nbcst += 1;
//     // };
//     // add Cst
//     model.add(CC);

//     /////////////////////////////////////
//     ////////  Obj
//     /////////////////////////////////////
//     IloObjective obj=IloAdd(model,IloMaximize(env,0.0));
//     for (int i{0}; i<n; i++)
//         for (int j{0}; j<m; j++)
//             obj.setLinearCoef(x[i][j],1);
//     //////////////////////////////////
//     ///////  Resolution
//     /////////////////////////////////
//     IloCplex cplex(model);

//     ////////////////////////////////////
//     //////// get solution
//     ////////////////////////////////////
//     cplex.exportModel("sortie_Bin_Plne_heuristique.lp");
    
//     if ( !cplex.solve()){
//        return false;
//     };

//     env.out()<<"Solution status = " <<cplex.getStatus()<<std::endl;
//     env.out()<<"Solution value = " <<cplex.getObjValue()<<std::endl;

//     if (psolution != nullptr){
//         delete psolution;
//     }
//     this->psolution = new Configuration();
//     // psolution->cars.resize(m);
//     // for (int j{0}; j<m; j++){
//     //     for (int i{0}; i<n; i++){
//     //         if (cplex.getValue(x[i][j]) == 1){
//     //             //psolution->cars[j].add_client( &(pinstance->clients[i]));
//     //         };
//     //     };
//     // };

//     for (int i{0}; i<n; i++){
//         for (int j{0}; j<m; j++){
//             cplex.getValue(x[i][j]);
//         };
//     };


//     //////////////////////////////////////////
//     ///////  Cplex's ending
//     //////////////////////////////////////////
//     env.end();

//     // TO DO
//     std::cout<<"bin_plne_heuristique"<<std::endl;
//     return true;
// };


bool Solveur::bin_glouton_heuristique(){
    // TO DO
    std::cout<<"bin_glouton_heuristique"<<std::endl;
    return true;
};


bool Solveur::bin_plne_min_heuristique(){
    // TO DO
    std::cout<<"bin_plne_min_heuristique"<<std::endl;
    return true;
};


bool Solveur::plne_branch_and_cut(){
    // TO DO
    std::cout<<"plne branch and cut"<<std::endl;
    return true;
}


bool Solveur::plne_MTZ(){
    // TO DO
    std::cout<<"plne_MTZ"<<std::endl;
    return true;
}


bool Solveur::plne_flots(){
    // TO DO
    std::cout<<"plne_flots"<<std::endl;
    return true;
};