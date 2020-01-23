#include <iostream>
#include <string>
#include "Solveur.h"
#include "Instance.h"
#include "ilcplex/ilocplex.h"
#include "Solution.h"
#include "Client.h"
#include <set>
using namespace std;
#define epsilon 0.00001
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
    if (methode_ == "bin_plne_min_heuristique"){
        pmethode = & Solveur::bin_plne_min_heuristique;
        return;
    };
    if (methode_ == "plne_branch_and_cut"){
        pmethode = & Solveur::plne_branch_and_cut;
        return;
    };
    if (methode_ == "plne_flots"){
        pmethode = & Solveur::plne_flots;
        return;
    };    
    if (methode_ == "plne_MTZ"){
        pmethode = & Solveur::plne_MTZ;
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
    std::vector<std::vector<IloNumVar> > x;
    x.resize(n);               //x[i][j] client i in car j
    for (int i=0; i<n; i++){
        x[i].resize(m);
        std::ostringstream varname;
        for (int j=0; j<m; j++){
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
    int nbcst=0;
    // Cst capacity :   sum(x_i_j,i=1..n-1)<=Q     for j in 0..m-1
    for (int j=0; j<m; j++){
        IloExpr cst(env);
        for (int i=1; i<n; i++)
            cst += x[i][j]*clients[i].demande;
        CC.add(cst<=Q);
        
        std::ostringstream cstname;
        cstname.str("");
        cstname<<"Cst_capacity_car_"<<j;
        CC[nbcst].setName(cstname.str().c_str());
        nbcst++;
    };
    // Cst :  sum(x_i_j,j=1..k)==1     for i in 1..n
    for (int i=0; i<n; i++){
        IloExpr cst(env);
        for (int j=0; j<m; j++)
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
    for (int i=0; i<n; i++)
        for (int j=0; j<m; j++)
            obj.setLinearCoef(x[i][j],0);


//////////////////////////////////
///////  Resolution
/////////////////////////////////
    IloCplex cplex(model);

    cplex.exportModel("sortie.lp");
    
    if ( !cplex.solve()){
       return false;
    };

////////////////////////////////////
//////// get solution
////////////////////////////////////
    env.out()<<"Solution status = " <<cplex.getStatus()<<std::endl;
    env.out()<<"Solution value = " <<cplex.getObjValue()<<std::endl;

    this->psolution = new Solution(n,m,pinstance);
    this->psolution->objValue = cplex.getObjValue();
    this->psolution->status = cplex.getStatus();
    for (int j=0; j<m; j++){
        for (int i=1; i<n; i++){  
            this->psolution->x[i][j] = cplex.getValue(x[i][j]);  
            if (this->psolution->x[i][j] == 1){
                printf("(%d,%d) ",i,j);
            }
        };
    };

////////////////////////////////
///// cplex end
///////////////////////////////

    env.end();


///////////////////////////////////////////////////

 //    TO DO : HEURISTIQUE



    return true;
};

bool comparator(const Client& client1,const Client& client2){
    return (client1.demande>client2.demande)?true:false;
};

bool first_fit_decreasing(){
    // TO DO: DEBUG

//     int n = this->pinstance->get_n();  // nb client
//     int m = this->pinstance->get_m();   // nb vehicle
//     int Q = this->pinstance->get_Q();
//     std::vector<Client> clients = this->pinstance->get_clients();
//     std::vector<int> cars_load;

//     cars_load.resize(m,0);
//     std::sort(clients.begin(),clients.end(),comparator);
// TO DO TO DO TO DO
//         #ifdef DEBUG
//             for (auto i:clients)
//                 std::cout<<i<<" ";
//             std::cout<<std::endl;
//         #endif

//         for (Client& client:clients){
//             bool success{false};
//             for (Car& car:cars){
//                 success = car.add_client(client);
//                 if (success)
//                     break;
//             };
//             if (!success)
//                 return false;
//         };
//         return true;
//     }
    return true;
};

bool Solveur::bin_glouton_heuristique(){
    // bool success = first_fit_decreasing(cars,clients);
    // if (success){
    //     #ifdef DEBUG
    //     std::cout<<"first_fit_decreasing found a solution of binPacking problem"<<std::endl;
    //     #endif
    //     return true;
    // };
    // return plne(cars,clients);


//////////////////////////////////////////////////////////////

    // TO DO: HEURISTIQUE

    return true;
};


bool Solveur::bin_plne_min_heuristique(){
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
    std::vector<std::vector<IloNumVar> > x;
    x.resize(n);               //x[i][j] client i in car j
    for (int i=0; i<n; i++){
        x[i].resize(m);
        std::ostringstream varname;
        for (int j=0; j<m; j++){
            x[i][j]=IloNumVar(env, 0.0, 1.0, ILOINT);
            varname.str("");
            varname<<"x["<<i<<"]["<<j<<"]";
            x[i][j].setName(varname.str().c_str());
        };
    };

    // Y
    std::vector<IloNumVar> y;
    y.resize(m);
    for (int i=0; i<m; i++){
        y[i] = IloNumVar(env, 0.0, 1.0, ILOFLOAT);
    }

    /////////////////////////
    //////// CST
    /////////////////////////
    IloRangeArray CC(env);
    int nbcst=0;
    // Cst capacity :   sum(x_i_j,i=1..n-1)<=Q     for j in 0..m-1
    for (int j=0; j<m; j++){
        IloExpr cst(env);
        for (int i=1; i<n; i++)
            cst += x[i][j]*clients[i].demande;
        CC.add(cst<=Q);
        
        std::ostringstream cstname;
        cstname.str("");
        cstname<<"Cst_capacity_car_"<<j;
        CC[nbcst].setName(cstname.str().c_str());
        nbcst++;
    };
    // Cst :  sum(x_i_j,j=1..k)==1     for i in 1..n
    for (int i=0; i<n; i++){
        IloExpr cst(env);
        for (int j=0; j<m; j++)
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
    // Cst : y[j] >= x[i][j]  for i in [1...n]  for j in [1...m]
    for (int j=0; j<m; j++)
        for (int i=0; i<n; i++){
            IloExpr cst(env);
            cst += y[j];
            cst += -x[i][j];
            CC.add(cst >= 0);
        }
    // Cst : sum(x_i_j, i=1...n) >= y[j]   for j in [1..k]
    for (int j=0; j<m; j++){
        IloExpr cst(env);
        for (int i=0; i<n; i++){
            cst += x[i][j];
        };
        cst += -y[j];
        CC.add(cst>=0);
        
        std::ostringstream cstname;
        cstname.str("");
        cstname<<"Cst_capacity_car_"<<j;
        CC[nbcst].setName(cstname.str().c_str());
        nbcst++;
    }; 

    // add Cst
    model.add(CC);

    /////////////////////////////////////
    ////////  Obj
    /////////////////////////////////////
    // x
    IloObjective obj=IloAdd(model,IloMaximize(env,0.0));
    for (int i=0; i<n; i++)
        for (int j=0; j<m; j++)
            obj.setLinearCoef(x[i][j],0);

    // y 
    for (int j=0; j<m; j++)
        obj.setLinearCoef(y[j],1);

//////////////////////////////////
///////  Resolution
/////////////////////////////////
    IloCplex cplex(model);

    cplex.exportModel("sortie.lp");
    
    if ( !cplex.solve()){
       return false;
    };

//////////////////////////////////
////// get solution
//////////////////////////////////
    env.out()<<"Solution status = " <<cplex.getStatus()<<std::endl;
    env.out()<<"Solution value = " <<cplex.getObjValue()<<std::endl;

    this->psolution = new Solution(n,m,pinstance);
    this->psolution->objValue = cplex.getObjValue();
    this->psolution->status = cplex.getStatus();
    for (int j=0; j<m; j++){
        for (int i=0; i<n; i++){     
            if (cplex.getValue(x[i][j])== 1){
                printf(" (%d,%d) ",i,j);
            };
            this->psolution->x[i][j] = cplex.getValue(x[i][j]);  
        };
    };
    

////////////////////////////////
///// cplex end
///////////////////////////////
    env.end();


//////////////////////////////////////////////////////////////

    // TO DO: HEURISTIQUE
    return true;
};


// bool Solveur::plne_branch_and_cut(){
//     // TO DO
//     std::cout<<"plne branch and cut"<<std::endl;
//     return true;
// }






#include "math.h"

bool Solveur::plne_MTZ(){
// init
    int n = this->pinstance->get_n();  // nb client
    int m = this->pinstance->get_m();   // nb vehicle
    int Q = this->pinstance->get_Q();
    std::vector<std::vector<float> > c = this->pinstance->distance;
    std::vector<Client> clients = this->pinstance->get_clients();
    // NC
    std::set<int> NC;
    for (int i = 1; i<clients.size();i++){
        NC.insert(i);
    };

//model
    IloEnv   env;
    IloModel model(env);
//var
    // x[i][j]   i,j in {0,...n-1}  l'arc (i,j) est utilise ou non
    std::vector<std::vector<IloNumVar> > x;
    x.resize(n);  

    // for (int i=0; i<n; i++){
    //     x[i].resize(n);
    // }            
    // for (int i=0; i<n; i++){
    //     std::ostringstream varname;
    //     for (int j=i; j<n; j++){
    //         x[i][j]=IloNumVar(env, 0.0, 1.0, ILOINT);
    //         varname.str("");
    //         varname<<"x["<<i<<"]["<<j<<"]";
    //         x[i][j].setName(varname.str().c_str());
    //         x[j][i]=x[i][j];
    //     };
    // };
    for (int i=0; i<n; i++){
        x[i].resize(n);
        std::ostringstream varname;
        for (int j=0; j<n; j++){
            x[i][j]=IloNumVar(env, 0.0, 1.0, ILOINT);
            varname.str("");
            varname<<"x["<<i<<"]["<<j<<"]";
            x[i][j].setName(varname.str().c_str());
        };
    };

    // w[i]  i in {0,...,n-1}   la quantite charge dans le vehicule apres le passage d'un vehicule au client i
    std::vector<IloNumVar> w;
    w.resize(n);
    for (int i = 0; i < n; i++){
        w[i] = IloNumVar(env, 0.0, Q, ILOFLOAT);
        std::ostringstream varname;
        varname.str("");
        varname<<"w["<<i<<"]";
        w[i].setName(varname.str().c_str());
    };

    //std::cout<<"partie var correcte"<<std::endl;

//cst
    IloRangeArray CC(env);
    int nbcst=0;

    // Cst 1   Arc_sortant_de_depot

    {
        IloExpr cst(env);
        for (int j=1; j<n; j++){
            cst += x[0][j];
        };
        CC.add(cst==m);
        std::ostringstream cstname;
        cstname.str("");
        cstname<<"Arc_sortant_de_depot";
        CC[nbcst].setName(cstname.str().c_str());
        nbcst += 1;
    };

    // {
    //     IloExpr cst(env);
    //     for (int j=1; j<n; j++){
    //         cst += x[0][j];
    //     };
    //     CC.add(cst<=m);
    //     CC.add(cst>=m);        
    //     std::ostringstream cstname;
    //     cstname.str("");
    //     cstname<<"Arc_sortant_de_depot";
    //     CC[nbcst+1].setName(cstname.str().c_str());
    //     CC[nbcst].setName(cstname.str().c_str());
    //     nbcst += 2;
    // };

    // Cst 2    Arc_entrant_a_depot
    {
        IloExpr cst(env);
        for (int i=1; i<n; i++){
            cst += x[i][0];
        };
        CC.add(cst==m);
        std::ostringstream cstname;
        cstname.str("");
        cstname<<"Arc_entrant_a_depot";
        CC[nbcst].setName(cstname.str().c_str());
        nbcst += 1;
    };
    // {
    //     IloExpr cst(env);
    //     for (int i=1; i<n; i++){
    //         cst += x[i][0];
    //     };
    //     CC.add(cst<=m);
    //     CC.add(cst>=m);
    //     std::ostringstream cstname;
    //     cstname.str("");
    //     cstname<<"Arc_entrant_a_depot";
    //     CC[nbcst].setName(cstname.str().c_str());
    //     CC[nbcst+1].setName(cstname.str().c_str());
    //     nbcst += 2;
    // };

    // Cst 3  Arc_sortant_de_i   i in NC
    for (int it = 1; it < n; it++){
        IloExpr cst(env);
        for (int jt = 0; jt < n; jt++){
            cst += x[it][jt];
        };
        CC.add(cst == 1);
        std::ostringstream cstname;
        cstname.str("");
        cstname<<"Arc_sortant_de_"<<it;
        CC[nbcst].setName(cstname.str().c_str());
        nbcst += 1;
    };
    // for (int it = 1; it < n; it++){
    //     IloExpr cst(env);
    //     for (int jt = 0; jt < n; jt++){
    //         cst += x[it][jt];
    //     };
    //     CC.add(cst <= 1);
    //     CC.add(cst >= 1);
    //     std::ostringstream cstname;
    //     cstname.str("");
    //     cstname<<"Arc_sortant_de_"<<it;
    //     CC[nbcst].setName(cstname.str().c_str());
    //     CC[nbcst+1].setName(cstname.str().c_str());
    //     nbcst += 2;
    // };

    // Cst 4  Arc_entrant_a_j   j in NC
    for (int jt = 1; jt < n ; jt++){
        IloExpr cst(env);
        for (auto it = 0; it<n; it++){
            cst += x[it][jt];
        };
        CC.add(cst == 1);
        std::ostringstream cstname;
        cstname.str("");
        cstname<<"Arc_entrant_a_"<<jt;
        CC[nbcst].setName(cstname.str().c_str());
        nbcst += 1;
    };

    //Cst 5 MTZ
    for (int i = 0; i < n; i++){
        for (int jt = 1; jt < n; jt++){
            IloExpr cst(env);
            float d = clients[jt].demande;
            cst = cst + w[i] - w[jt] - d + (Q + d)*(1- x[i][jt]);
            CC.add(cst >= 0);
            std::ostringstream cstname;
            cstname.str("");
            cstname<<"MTZ_"<<i<<"_"<<jt;
            CC[nbcst].setName(cstname.str().c_str());
            nbcst ++;
        };
    };
    
    // add Cst
    model.add(CC);

//obj

    // x
    IloObjective obj=IloAdd(model,IloMinimize(env,0.0));
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            obj.setLinearCoef(x[i][j],c[i][j]);
        };
    };


    std::cout<<" **********************set obj *****************************"<<std::endl;
//Resolutino
    IloCplex cplex(model);

    cplex.exportModel("./sortie.lp");
    
    if ( !cplex.solve()){
        std::cout<<"pas de solution"<<std::endl;
       return false;
    };
    std::cout<<" ****************************** solve *****************************"<<std::endl;
// Get solution

    env.out()<<"Solution status = " <<cplex.getStatus()<<std::endl;
    env.out()<<"Solution value = " <<cplex.getObjValue()<<std::endl;

    this->psolution = new Solution(n,m,pinstance);
    this->psolution->objValue = cplex.getObjValue();
    this->psolution->status = cplex.getStatus();
    std::vector<int> startCities;
    std::cout<<" **********************   traduction de lasolution phase init *****************************"<<std::endl;
//    std::cout<<"x.size() = "<<x.size()<<" x[0].size() = "<<x[0].size()<<std::endl;
    
    ////TO DEBUG : pourquoi ne peut pas afficher x[0][0]

    for (int i=0; i<n; i++){
        for (int j=0; j<m; j++){
            this->psolution->x[i][j] = 0;
        }
    }
    int k = -1;
    for (int i=1; i<n; i++){
        if (cplex.getValue(x[0][i]) == 1){
            std::cout<<std::endl;
            k++;
//            std::cout<<k <<": ";
            int city = i; 
            int nextCity;
            while (city != 0){
//                std::cout<<city<<"  ";
                this->psolution->x[city][k] = 1;   //city in vehicle k
                for (int nextCity=0; nextCity<n; nextCity++){
                    if (cplex.getValue(x[city][nextCity]) == 1){
                        city = nextCity;
                        break;
                    }
                };
            };
        };
    };

    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            this->psolution->solx[i][j] = 0;
        }
    }

    for (int i=0; i<n; i++){
        for (int j=0;j<n; j++){
            if (i==0 && j==0){
                continue;
            }
            if (cplex.getValue(x[i][j]) > 1-epsilon){
                psolution->solx[i][j] = 1;
            }
        }
    }   


    std::cout<<" **********************   traduction de lasolution *****************************"<<std::endl;
// cplex end
     env.end();

    std::cout<<" //////////////////////////////////////////////////////// fin //////////////////////////////////"<<std::endl;
    return true;
}


bool Solveur::plne_flots(){
    // TO DO
    std::cout<<"plne_flots"<<std::endl;
    return true;
};