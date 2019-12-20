#include <iostream>
#include <string>
#include "Solveur.h"
#include "Instance.h"
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
// bin_plne_heuristique
bool Solveur::bin_plne_heuristique(){

 
    int n = instance->n;  // nb client
    int k = Car::objectCount;   // nb vehicle
    ///////////////////
    /////// Cplex initialization
    ////////////////////
    IloEnv env;
    IloModel model(env);
    ///////////////////////
    /////  Var
    //////////////////////
    std::vector<std::vector<IloNumVar>> x;
    x.resize(n);               //x[i][j] client i in car j
    for (int i{0}; i<Client::objectCount; i++){
        x[i].resize(k);
        std::ostringstream varname;
        for (int j{0}; j<k; j++){
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
    // Cst capacity :   sum(x_i_j,i=1..n)<=Q     for j in 1..k
    for (int j{0}; j<k; j++){
        IloExpr cst(env);
        for (int i{0}; i<n; i++)
            cst += x[i][j]*clients[i].demand;
        CC.add(cst<=cars[j].Q);
        
        std::ostringstream cstname;
        cstname.str("");
        cstname<<"Cst_capacity_car_"<<j;
        CC[nbcst].setName(cstname.str().c_str());
        nbcst++;
    };
    // Cst :  sum(x_i_j,j=1..k)==1     for i in 1..n
    for (int i{0}; i<n; i++){
        IloExpr cst(env);
        for (int j{0}; j<k; j++)
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
        for (int j{0}; j<k; j++)
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
    solx.resize(n,std::vector<int>(k));
    for (int j{0}; j<k; j++){
        cars[j].clear_clients();
        for (int i{0}; i<n; i++){     
            if (cplex.getValue(x[i][j]) == 1){
                bool success = cars[j].add_client(clients[i]);
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

    // TO DO
    std::cout<<"bin_plne_heuristique"<<std::endl;
    return true;
};
bool Solveur::bin_glouton_heuristique(){
    // TO DO
    std::cout<<"bin_glouton_heuristique"<<std::endl;
    return true;
};
bool bin_plne_min_heuristique(){
    // TO DO
    std::cout<<"bin_plne_min_heuristique"<<std::endl;
    return true;
};
bool plne_branch_and_cut(){
    // TO DO
    std::cout<<"plne branch and cut"<<std::endl;
    return true;
}
bool plne_MTZ(){
    // TO DO
    std::cout<<"plne_MTZ"<<std::endl;
    return true;
}
bool plne_flots(){
    // TO DO
    std::cout<<"plne_flots"<<std::endl;
    return true;
}