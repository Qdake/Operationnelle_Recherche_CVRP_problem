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
#include <map>
#include <iomanip>
using namespace std;
#define epsilon 0.00001
//find_ViolatedCst_INTEGER_sous_tour
void find_ViolatedCst_INTEGER_sous_tour(IloEnv env, vector<vector<IloNumVar> >& x, vector<vector<int> > sol,
list<IloRange>& L_ViolatedCst,  vector<Client>& clients,float Q,int m){
//init
    bool DEBUG = true;

    int i,j;
    int n = x.size();

// cst coupe cvrp
    set<int> nodes;
    for (i=0; i<n; i++){
        nodes.insert(i);
    };

    // tours
    vector<set<int> > tours;
    while (nodes.empty() == false){
        set<int> tour;
        int city = *(nodes.begin());
        int nextCity;
        while (nodes.find(city) != nodes.end()){
            nodes.erase(city);   //city in vehicle k
            tour.insert(city);
            for (auto nextCity=nodes.begin(); nextCity != nodes.end(); nextCity++){
                if (sol[city][*nextCity]==1){
                    city = *nextCity;
                    break;
                }
            };
        };
        tours.push_back(tour);
    }; 

    for (set<int>& tour:tours){
        for (int j:tour){
            if (sol[0][j] == 1){
                tour.insert(0);
                break;
            }
        }
    }  
    // pour chaque tour
    for (set<int>& tour:tours){
        float somme_demande = 0;
        for (int i:tour){
            somme_demande += clients[i].demande;
        }
        // si c'est un sous tour 
        if ( tour.find(0) == tour.end()){
            tour.erase(0);
            // construire CST violee
            IloExpr expr(env);
            for (int i:tour){
                for (int j=0; j<n; j++){
                    // si j est dans tour
                    if (tour.find(j) != tour.end()){
                        continue;
                    };
                    // sinon
                    expr += x[i][j];
                }
            }
            IloRange newCte = IloRange(expr >= ceil(float(somme_demande)/float(Q)));
            L_ViolatedCst.push_back(newCte);
            //return;
            //**************************************
        };
    };
};

// find ViolatedCst_INTEGER_capacite
void find_ViolatedCst_INTEGER_capacite(IloEnv env, vector<vector<IloNumVar> >& x, vector<vector<int> > sol,
list<IloRange>& L_ViolatedCst,  vector<Client>& clients,float Q,int m){
//init
    bool DEBUG = false;

    int i,j,k;
    int n = x.size();

    for (i=1; i<n; i++){
        if (sol[0][i] == 0) continue;
        set<int> w;
        set<int> others;
        int somme_demande = 0;
        j = i;
        while (j != 0){
            w.insert(j);
            somme_demande += clients[j].demande;
            for (int k=0; k<n; k++){
                if (sol[j][k] == 1){
                    j = k;
                    break;
                }
            }
        }
        // si les contraintes de CAPACITE ne sont pas violees
        if (somme_demande <= Q) continue;
        // IloExpr
        IloExpr expr(env);
        for (j = 0; j<n; j++){
            if (w.find(j) == w.end()){
                others.insert(j);
            }
        }
        //constr CST
        for (int j:w){
            for (int k:others){
                expr += x[j][k];
            };
        };
        IloRange newCte = IloRange(expr >= ceil(float(somme_demande)/float(Q)));
        L_ViolatedCst.push_back(newCte);
        //return;
    };

};

void find_ViolatedCst(IloEnv env, vector<vector<IloNumVar> >& x, vector<vector<float> > sol,
list<IloRange>& L_ViolatedCst,  vector<Client>& clients,float Q,int m){
//init
    bool DEBUG = false;
    // get current solution
    int i,j;
    int n = x.size();
    map<int, float> sommex;
    int u,w;
    float s_demande,h,hx;
//  Greedy randomized algorithm
    if (DEBUG) cout<<"*********************** Greedy randomized algorithm  BEGIN **********************"<<endl;
    set<int> s,v;
    for (i=1; i<n; i++) v.insert(i);
    // ajouter les noueds de v dans s un par un 
    while ( v.size() != 0){
        if (DEBUG) cout<<"*********************** add nodes in S  BEGIN **********************"<<endl;
        if (DEBUG){
            cout<<" while ing: "<<endl;
            cout<<"S={"; for (auto it = s.begin(); it!=s.end();it++) cout<<*it<<" "; cout<<"}"<<endl;
            cout<<"V={"; for (auto it = v.begin(); it!=v.end();it++) cout<<*it<<" "; cout<<"}"<<endl;
        }
        sommex.clear();
        for (auto it = v.begin(); it != v.end(); it++){
            h = 0;
            for (auto ij = s.begin(); ij != s.end(); ij++){
                //cout<<"sol[*it][*ij]= "<<sol[*it][*ij]; 
                h += sol[*it][*ij];
            };
            if (s.empty()){
                for (auto ij = v.begin(); ij != v.end(); ij++){
                    if (DEBUG) cout<<"sol[*it][*ij]= "<<sol[*it][*ij]; 
                    h += sol[*it][*ij];
            };    
            }
            sommex[*it] = h;
        }

        if (DEBUG){
            for (auto it = v.begin(); it != v.end(); it++){
                cout<<"<"<<*it<<","<<sommex.find(*it)->second<<">  ";
            }
            cout<<endl;
        }

        // find max h
        if (DEBUG) cout<<"*********************** find best node to add in S  BEGIN **********************"<<endl;
        u = *(v.begin());
        h = sommex.find(u)->second;
        for (auto it = v.begin(); it != v.end(); it++){
            if (sommex.find(*it)->second < h){
                u = *it;
                h = sommex.find(*it)->second;
            }
        }
        if (DEBUG) cout<<"*********************** find best node to add in S  END **********************"<<endl;
        // maj
        if (DEBUG) cout<<"*********************** MAJ  S and V   BEGIN **********************"<<endl;
        s.insert(u);
        v.erase(u);
        if (DEBUG) cout<<"added node : "<<u<<endl;
        if (DEBUG){
            cout<<"S={"; for (auto it = s.begin(); it!=s.end();it++) cout<<*it<<" "; cout<<"}"<<endl;
            cout<<"V={"; for (auto it = v.begin(); it!=v.end();it++) cout<<*it<<" "; cout<<"}"<<endl;
        }
        if (DEBUG) cout<<"*********************** MAJ  S and V   END **********************"<<endl;
        s_demande = 0;
        hx = 0;
        for (auto ij=s.begin(); ij != s.end(); ij++){
            s_demande += clients[*ij].demande;
            for (auto it=v.begin(); it != v.end(); it++){
                hx += sol[*ij][*it];    
            };
        }
        // ??? hx>=h/Q
        if (hx < h/Q){
            if (DEBUG){
                cout<<"S ={ ";
                for (auto it = s.begin(); it!= s.end();it++){
                    cout<<*it<<"  ";
                }
                cout<<"}"<<endl;
            }
            // construire IloExpression
            IloExpr expr(env);
            if (DEBUG){
                cout<<"ILoExpr = ";
            }
            for (auto ij=s.begin(); ij != s.end(); ij++){
                for (auto it=v.begin(); it != v.end(); it++){
                    expr += x[*ij][*it];    
                    if (DEBUG){
                        cout<<"x["<<*ij<<"]["<<*it<<"] + ";
                    }
                };
                expr += x[*ij][0];
                if (DEBUG){
                    cout<<"x["<<*ij<<"]["<< 0 <<"] + ";
                }
            };
            if (DEBUG){
                cout<<endl;
            }
            // construir CST
            IloRange newCte = IloRange(expr >= h/Q);
            L_ViolatedCst.push_back(newCte);
            break;
        };
    };
    if (DEBUG) cout<<"*********************** Greedy randomized algorithm  END **********************"<<endl;

    if (DEBUG) cout<<"*********************** Ilo user cut  END **********************"<<endl;
};


// Necessary inequalities
ILOLAZYCONSTRAINTCALLBACK2(LazyCutSeparation,Instance *, pinstance, vector<vector<IloNumVar> >&, x){
// init

    bool DEBUG =false;
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
            if (getValue(x[i][j])<epsilon){
                sol[i][j] = 0;
            }else{
                sol[i][j] = 1;
            }
        };
    };
    sol[0][0] = 0; 


    list<IloRange> L_ViolatedCst;

// Separation of .... inequalities 
    L_ViolatedCst.clear();
    find_ViolatedCst_INTEGER_sous_tour(getEnv(), x, sol, L_ViolatedCst,clients,Q,m);
    find_ViolatedCst_INTEGER_capacite(getEnv(), x, sol, L_ViolatedCst,clients,Q,m);

    if (DEBUG) if (L_ViolatedCst.empty()) cout<<"No Cst found"<<endl;
    
    while (!L_ViolatedCst.empty()){
        if (DEBUG) cout << "Adding constraint : " << L_ViolatedCst.front() << endl;
        add(L_ViolatedCst.front(),IloCplex::UseCutForce); //
        L_ViolatedCst.pop_front();
    };
    
};


//Usefull inequalities (here are the same as the necessary ones
ILOUSERCUTCALLBACK2(UserCutSeparation,Instance *, pinstance, vector<vector<IloNumVar> >&, x){
//init
    bool DEBUG = false;
    if (DEBUG) cout<<"\n******************* User Separation Callback   BEGIN*************\n"<<endl;
    float Q = pinstance->get_Q();
    int m = pinstance->m;
    std::vector<Client> clients = pinstance->get_clients();
    // get current solution
    int i,j;
    int n = x.size();
    vector<vector<float> > sol;
    
    sol.resize(n);
    for (i=0; i<n; i++){
        sol[i].resize(n);
        for (j=0; j<n; j++){
                sol[i][j] = getValue(x[i][j]);
        };
    };
    sol[0][0] = 0; 

// gen cst
    list<IloRange> L_ViolatedCst;

    /* Separation of .... inequalities */
  L_ViolatedCst.clear();
  find_ViolatedCst(getEnv(), x, sol, L_ViolatedCst,clients,Q,m);
  
  if (L_ViolatedCst.empty()) cout<<"No Cst found"<<endl;
  
  while (!L_ViolatedCst.empty()){
    if (DEBUG)  cout << "Adding constraint : " << L_ViolatedCst.front() << endl;
    if (DEBUG)  add(L_ViolatedCst.front(),IloCplex::UseCutForce); //UseCutPurge);
    L_ViolatedCst.pop_front();
  }

    if (DEBUG) cout<<"\n******************* User Separation Callback   END*************\n"<<endl;

}


bool Solveur::plne_branch_and_cut(){
// init
    bool DEBUG = true;

    if (DEBUG) {std::cout<<" **********************BRANCH AND CUT: init begin *****************************"<<std::endl;}
    int n = this->pinstance->get_n();  // nb client
    int m = this->pinstance->get_m();   // nb vehicle
    int Q = this->pinstance->get_Q();
    std::vector<Client> clients = this->pinstance->get_clients();
    std::vector<std::vector<float> > c = this->pinstance->distance;
    // NC
    std::set<int> NC;
    for (int i = 1; i<clients.size();i++){
        NC.insert(i);
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


    // Cst 0 
    for (int it = 0; it < n; it++){
        IloExpr cst(env);
        cst += x[it][it];
        CC.add(cst == 0);
        nbcst += 1;
    };

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
 

    // CST 6
    for (int i=1; i<n; i++){
        for (int j=1; j<n; j++){
            IloExpr cst(env);
            cst += x[i][j];
            cst += x[j][i];
            CC.add(cst<=1);
            nbcst ++;
        }
    }

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
    cplex.use(UserCutSeparation(env,pinstance,x));
//Resolutino

    if (DEBUG) std::cout<<" **********************BRANCH AND CUT: solve begin *****************************"<<std::endl;
    cplex.exportModel("./sortie.lp");
    
    if ( !cplex.solve()){
        std::cout<<"pas de solution"<<std::endl;
       return false;
    };

    if (DEBUG) std::cout<<" **********************BRANCH AND CUT: solve end *****************************"<<std::endl;

// get solution
    if (DEBUG) std::cout<<" **********************BRANCH AND CUT: get solution begin *****************************"<<std::endl;
    env.out()<<"Solution status = " <<cplex.getStatus()<<std::endl;
    env.out()<<"Solution value = " <<cplex.getObjValue()<<std::endl;

    this->psolution = new Solution(n,m,pinstance);
    this->psolution->objValue = cplex.getObjValue();
    this->psolution->status = cplex.getStatus();
    std::vector<int> startCities; 
    // pinstance->x
    for (int i=0; i<n; i++){
        for (int j=0; j<m; j++){
            this->psolution->x[i][j] = 0;
        }
    }
    int k = -1;
    for (int i=1; i<n; i++){
        if (cplex.getValue(x[0][i]) >= 1-epsilon){
            std::cout<<std::endl;
            k++;
            int city = i; 
            int nextCity;
            while (city != 0){
                this->psolution->x[city][k] = 1;   //city in vehicle k
                for (int nextCity=0; nextCity<n; nextCity++){
                    if (cplex.getValue(x[city][nextCity]) >= 1-epsilon){
                        city = nextCity;
                        break;
                    }
                };
            };
        };
    };

    // pinstance->solx
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            this->psolution->solx[i][j] = 0;
        }
    }
    if (DEBUG) cout<<" ******************* slox **********************************************************"<<endl;
    if (DEBUG){
        cout<<left<<setw(7)<<"l";
        for (int i=0; i<n; i++) cout<<setw(7)<<"c"+to_string(i);
        cout<<endl;
    }
    for (int i=0; i<n; i++){
            cout<<setprecision(1)<<fixed;
            cout<<setw(7)<<i;
        for (int j=0;j<n; j++){

            if (cplex.getValue(x[i][j]) <epsilon)  this->psolution->solx[i][j] = 0 ;
            if (cplex.getValue(x[i][j]) > 1- epsilon)  this->psolution->solx[i][j] = 1 ;
            cout<<setw(7) <<cplex.getValue(x[i][j]);
            // cout<<setw(7) <<psolution->solx[i][j];   
        }
        cout<<endl;
    }   
    
    {
        cout<<" 0 3 : "<<cplex.getValue(x[0][3])<<endl;
        cout<<" 12 3 : "<<cplex.getValue(x[12][3])<<endl;
        cout<<" 3 0 : "<<cplex.getValue(x[3][0])<<endl;
        cout<<" 3 12 : "<<cplex.getValue(x[3][12])<<endl;
    }
    if (DEBUG) std::cout<<" **********************BRANCH AND CUT: get solution begin *****************************"<<std::endl; 
// end
    std::cout<<"plne branch and cut  SOLVER FIN \n \n"<<std::endl;
    return true;





}