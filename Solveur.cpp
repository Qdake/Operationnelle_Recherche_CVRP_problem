#include <iostream>
#include <string>
#include "Solveur.h"
#include "Instance.h"
#include "ilcplex/ilocplex.h"
#include "Solution.h"
using namespace std;
#define DEBUG

//destructeur
Solveur::~Solveur(){};

// set_instance
void Solveur::set_instance(Instance* pinstance_){
    pinstance = pinstance_;
};


// set_method
void Solveur::set_method(std::string methode_){
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

    this->psolution = new Solution(n,m);
    this->psolution->objValue = cplex.getObjValue();
    this->psolution->status = cplex.getStatus();
    for (int j{0}; j<m; j++){
        for (int i{0}; i<n; i++){     
            this->psolution->x[i][j] = cplex.getValue(x[i][j]);  
        };
    };

//////////////////////////////////////////
///////  Cplex's ending
//////////////////////////////////////////
    env.end();
}


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