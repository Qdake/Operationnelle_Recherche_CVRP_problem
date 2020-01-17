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
#define DEBUG true
#define epsilon 0.00001

void find_ViolatedCst_INTEGER(IloEnv env, vector<vector<IloNumVar> >& x, vector<vector<int> > sol,
list<IloRange>& L_ViolatedCst,  vector<Client>& clients,float Q,int m){
// get current solution
    int i,j;
    int n = x.size();

// find all nodes which are not connected with 0 
    set<int> node_not_connected_with_0;
    //init
    if (DEBUG) cout<<" \n**********************find_ViolatedCst_Integer:  INIT  begin**********************\n";
    for (i=0; i<n; i++){
        node_not_connected_with_0.insert(i);
    };
    if (DEBUG){
        cout<<"set: ";
        for (auto it=node_not_connected_with_0.begin(); it != node_not_connected_with_0.end(); it++){
            cout<<*it <<" ";
        }
        cout<<endl;
        if (DEBUG) cout<<" \n**********************find_ViolatedCst_Integer:  INIT  end**********************\n";
    };
    if (DEBUG) cout<<" \n**********************find_ViolatedCst_Integer:   ELININATION  begin**********************\n";
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
    if (DEBUG){
        cout<<"set: ";
        for (auto it=node_not_connected_with_0.begin(); it != node_not_connected_with_0.end(); it++){
            cout<<*it <<" ";
        }
        cout<<endl;
        if (DEBUG) cout<<" \n**********************find_ViolatedCst_Integer:   ELININATION  end**********************\n";
    };
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

    if (DEBUG){
        for (i=0;i<sous_tours.size();i++){
            cout<<"sous_tour: ";
            for (auto jt = sous_tours[i].begin(); jt != sous_tours[i].end(); jt++){
                for (auto kt = sous_tours[i].begin(); kt != sous_tours[i].end();kt++){
                    if (sol[*jt][*kt] == 1){
                        cout<< *jt<<"-->"<<*kt<< " ";
                    }
                }
            };
        };

        cout<<" \n**********************find_ViolatedCst_Integer:   ELININATION  end**********************\n";
    }
// construction of violated csts

    if (DEBUG) cout<<"Q = "<<Q<<endl;
    for (i=0;i<sous_tours.size();i++){
        if (DEBUG) cout<<"IloExpr: ";
        IloExpr expr(env);
        float s = 0;
    
    //calcul S
    cout<<"S = ";
    for (auto jt = sous_tours[i].begin(); jt != sous_tours[i].end(); jt++){
        s += clients[*jt].demande;
        cout<<"node("<<*jt<<")(demande "<<clients[*jt].demande<<")";
    }; 
    // cacul IloExpr
    for (auto jt = sous_tours[i].begin(); jt != sous_tours[i].end(); jt++){
        // W = V(C) = sous_tours[i]
        // for all node in W
            // for all kt in {0,...,n} \ W
            for (int kt = 0; kt<n; kt++){
                if (sous_tours[i].find(kt) == sous_tours[i].end()){
                    if (DEBUG) cout<< "+ x[" << (*jt)<<"]["<<(kt)<<"]";
                    expr += x[*jt][kt];
                };
            };
        };
        // cst
        float minimum_car;
        minimum_car = min(float(m),ceil(s/Q));
        cout<<"s= "<<s<<"  Q = "<<Q<<"   s/Q = "<<s/Q<< "   ceil(s/Q) = "<<ceil(s/Q)<<" min ="<<minimum_car<<endl;

        IloRange newCte = IloRange(expr >= minimum_car);
        if (DEBUG) cout<<" >= "<<minimum_car<<"\n";
        L_ViolatedCst.push_back(newCte);
    }
    
};

// Necessary inequalities
ILOLAZYCONSTRAINTCALLBACK2(LazyCutSeparation,Instance *, pinstance, vector<vector<IloNumVar> >&, x){
    cout<<"\n*********** Lazy separation Callback *************\n"<<endl;
    float Q = pinstance->get_Q();
    int m = pinstance->m;
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
            if (getValue(x[i][j])<epsilon){
                sol[i][j] = 0;
            }else{
                sol[i][j] = 1;
            }
        };
    };
    sol[0][0] = 0; 


    list<IloRange> L_ViolatedCst;

    /* Separation of .... inequalities */
  L_ViolatedCst.clear();
  find_ViolatedCst_INTEGER(getEnv(), x, sol, L_ViolatedCst,clients,Q,m);
  
  if (L_ViolatedCst.empty()) cout<<"No Cst found"<<endl;
  
  while (!L_ViolatedCst.empty()){
      cout << "Adding constraint : " << L_ViolatedCst.front() << endl;
    add(L_ViolatedCst.front(),IloCplex::UseCutForce); //UseCutPurge);
    L_ViolatedCst.pop_front();
  }

};

bool Solveur::plne_branch_and_cut(){
// init
    if (DEBUG) {std::cout<<" **********************BRANCH AND CUT: init begin *****************************"<<std::endl;}
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
        };
    };
    if (DEBUG) std::cout<<" **********************BRANCH AND CUT: init end*****************************"<<std::endl;
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

    if (DEBUG) std::cout<<" **********************BRANCH AND CUT: var end *****************************"<<std::endl;

//cst
    IloRangeArray CC(env);
    int nbcst=0;
    if (DEBUG) std::cout<<" **********************BRANCH AND CUT: add csts begin *****************************"<<std::endl;

    //cst 0 x[i][i] = 0
    for( int i = 0; i<n; i++){
        IloExpr cst(env);
        cst += x[i][i];
        CC.add(cst == 0);
        nbcst ++;
    }

    // Cst 1   Arc_sortant_de_depot

    {
        IloExpr cst(env);
        for (int j=0; j<n; j++){
            cst += x[0][j];
        };
        CC.add(cst<=m); 
        std::ostringstream cstname;
        cstname.str("");
        cstname<<"Arc_sortant_de_depot";
        CC[nbcst].setName(cstname.str().c_str());
        nbcst += 1;
    };

    // Cst 2    Arc_entrant_a_depot
    {
        IloExpr cst(env);
        for (int i=0; i<n; i++){
            cst += x[i][0];
        };
        CC.add(cst<=m);
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
    if (DEBUG) std::cout<<" **********************BRANCH AND CUT: add csts end *****************************"<<std::endl;
//obj
    if (DEBUG) std::cout<<" **********************BRANCH AND CUT: set objet begin*****************************"<<std::endl;
    // x
    IloObjective obj=IloAdd(model,IloMinimize(env,0.0));
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            obj.setLinearCoef(x[i][j],c[i][j]);
        };
    };


    if (DEBUG) std::cout<<" **********************BRANCH AND CUT: set objet end*****************************"<<std::endl;


    IloCplex cplex(model);
// ADD CHECK SOLUTION FEASABILITY
    cplex.use(LazyCutSeparation(env,pinstance,x));

//Resolutino

    if (DEBUG) std::cout<<" **********************BRANCH AND CUT: solve begin *****************************"<<std::endl;
    cplex.exportModel("./sortie.lp");
    
    if ( !cplex.solve()){
        std::cout<<"pas de solution"<<std::endl;
       return false;
    };

    if (DEBUG) std::cout<<" **********************BRANCH AND CUT: solve end *****************************"<<std::endl;

//solve
    if (DEBUG) std::cout<<" **********************BRANCH AND CUT: get solution begin *****************************"<<std::endl;

    env.out()<<"Solution status = " <<cplex.getStatus()<<std::endl;
    env.out()<<"Solution value = " <<cplex.getObjValue()<<std::endl;

    if (DEBUG) std::cout<<" **********************BRANCH AND CUT: get solution begin *****************************"<<std::endl; 
//
    std::cout<<"plne branch and cut  SOLVER FIN \n \n"<<std::endl;
    return false;





}