#include <iostream>
#include <string>
#include "Solveur.h"
#include "Instance.h"
#include "ilcplex/ilocplex.h"
#include "Solution.h"
#include "Client.h"
#include <set>
#include <math.h>
#include <list>
using namespace std;
#define DEBUG

#define epsilon 0.0001

void find_ViolatedCst_INTEGER(IloEnv env, vector<vector<IloNumVar> >& x, vector<vector<int> > sol,
list<IloRange>& L_ViolatedCst,  vector<Client>& clients,float Q){
// get current solution
    int i,j;
    int n = x.size();
//     vector<vector<int> > sol;
    
//     sol.resize(n);
//     for (i=0; i<n; i++){
//         sol[i].resize(n);
//         for (j=0; j<n; j++){
//             if (i==0 && j==0){
//                 continue;
//             }
//             sol[i][j] = getValue(x[i][j]) < epsilon ? 0 : 1;
//         };
//     };
//     sol[0][0] = 0;    
// find all nodes which are not connected with 0 
    set<int> node_not_connected_with_0;
    //init
    for (i=0; i<n; i++){
        node_not_connected_with_0.insert(i);
    };
    //elimination of node_connected with 0
    for (int i=1; i<n; i++){
        if (sol[0][i] == 1){
            int city = i; 
            int nextCity;
            while (city != 0){
                node_not_connected_with_0.erase(city);   //city in vehicle k
                for (int nextCity=0; nextCity<n; nextCity++){
                    if (sol[city][nextCity] == 1){
                        city = nextCity;
                        break;
                    }
                };
            };
        };
    };
    node_not_connected_with_0.erase(0);
// trouver sous tours
    vector<set<int> > sous_tours;
    while (node_not_connected_with_0.empty() == false){
        set<int> tour;
        int city = *(node_not_connected_with_0.begin());
        int nextCity;
        while (node_not_connected_with_0.find(city) != node_not_connected_with_0.end()){
            node_not_connected_with_0.erase(city);   //city in vehicle k
            tour.insert(city);
            for (auto nextCity=node_not_connected_with_0.begin(); nextCity != node_not_connected_with_0.end(); nextCity++){
                if (sol[city][*nextCity] == 1){
                    city = *nextCity;
                    break;
                }
            };
        };
        sous_tours.push_back(tour);
    };

// construction of violated csts
    for (auto it = sous_tours.begin(); it != sous_tours.end(); it++){
        IloExpr expr(env);
        float s=0;
        set<int> tour = *it;
        while (tour.empty()==false){
            int elt = *(tour.begin());
            for (i=0; i<n; i++){
                if ((*it).find(elt) == (*it).end()){
                    expr += x[elt][i];
                }
            }
            s += clients[elt].demande;
            tour.erase(elt);
        }

        // cst
        IloRange newCte = IloRange(expr >= ceil(s/Q));
        L_ViolatedCst.push_back(newCte);
    }
    
};

// Necessary inequalities
ILOLAZYCONSTRAINTCALLBACK2(LazyCutSeparation,Instance *, pinstance, vector<vector<IloNumVar> >&, x){
    cout<<"\n*********** Lazy separation Callback *************\n"<<endl;
    float Q = pinstance->get_Q();
    std::vector<Client> clients = pinstance->get_clients();
// get current solution
    int i,j;
    int n = x.size();
    vector<vector<int> > sol;
    
    sol.resize(n);
    for (i=0; i<n; i++){
        sol[i].resize(n);
        for (j=0; j<n; j++){
            if (i==0 && j==0){
                continue;
            }
            sol[i][j] = getValue(x[i][j]) < epsilon ? 0 : 1;
        };
    };
    sol[0][0] = 0; 


    list<IloRange> L_ViolatedCst;

    /* Separation of .... inequalities */
  L_ViolatedCst.clear();
  find_ViolatedCst_INTEGER(getEnv(), x, sol, L_ViolatedCst,clients,Q);
  
  if (L_ViolatedCst.empty()) cout<<"No Cst found"<<endl;
  
  while (!L_ViolatedCst.empty()){
      cout << "Adding constraint : " << L_ViolatedCst.front() << endl;
    add(L_ViolatedCst.front(),IloCplex::UseCutForce); //UseCutPurge);
    L_ViolatedCst.pop_front();
  }

};

bool Solveur::plne_branch_and_cut(){
// init
    int n = this->pinstance->get_n();  // nb client
    int m = this->pinstance->get_m();   // nb vehicle
    int Q = this->pinstance->get_Q();
    std::vector<Client> clients = this->pinstance->get_clients();
    // NC
    std::set<int> NC;
    for (int i = 1; i<clients.size();i++){
        NC.insert(i);
    };
    // c[i][j]   i,j in {0,...,n-1}     la distance de i a j
    std::vector<std::vector<float> > c;
    c.resize(n);
    for (int i=0; i<n; i++){
        c[i].resize(n);
    };
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            c[i][j] = clients[i].distanceTo(clients[j]);
            std::cout<<c[i][j]<<" ";
        };
        std::cout<<std::endl;
    };

//model
    IloEnv   env;
    IloModel model(env);
//var
    // x[i][j]   i,j in {0,...n-1}  l'arc (i,j) est utilise ou non
    std::vector<std::vector<IloNumVar> > x;
    x.resize(n);  
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


    IloCplex cplex(model);
// ADD CHECK SOLUTION FEASABILITY
    //cplex.use(LazyCutSeparation(env,x,clients,Q));

//Resolutino


    cplex.exportModel("./sortie.lp");
    
    if ( !cplex.solve()){
        std::cout<<"pas de solution"<<std::endl;
       return false;
    };
    std::cout<<" ****************************** solve *****************************"<<std::endl;


//
    std::cout<<"plne branch and cut"<<std::endl;
    return false;





}