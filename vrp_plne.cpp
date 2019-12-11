#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <string>
#include <sstream>
#include "ilcplex/ilocplex.h"

bool plne_mtz(std::vector<Car>& cars, std::vector<Client>& clients, std::vector<std::vector<float>> c){
    /* utiliser plne pour trouver un bin packing exact;
        true s'il existe une solution, false sinon;*/

    int n = clients.size();  // nb client
    int m = cars.size();   // nb vehicle
    float Q = cars[0].Q;
///////////////////
/////// Cplex initialization
////////////////////
    IloEnv env;
    IloModel model(env);
///////////////////////
/////  Var
//////////////////////
    // x
    std::vector<std::vector<IloBoolVar>> x;
    x.resize(n);               //x[i][j] edge {i,j} is used 
    for (int i{0}; i<n; i++){
        x[i].resize(n);
        std::ostringstream varname;
        for (int j{0}; j<n; j++){
            x[i][j]=IloNumVar(env, 0.0, 1.0, ILOBOOL);
            varname.str("");
            varname<<"x["<<i<<"]["<<j<<"]";
            x[i][j].setName(varname.str().c_str());
        };
    };
    // w
    std::vector<IloNumVar>> w;
    w.resize(n);
    for (int i{1}; i<n; i++){
        w[i]=IloNumVar(env, 0.0, 1.0, ILOFLOAT);
        varname.str("");
        varname<<"x["<<i<<"]["<<j<<"]";
        x[i][j].setName(varname.str().c_str());
    }
/////////////////////////
//////// CST
/////////////////////////

///// il y a exactement un arc entrant et un arc sortant pur chaque revendeurs, alors que le depot peut avoir jusqu'a m arcs entrants et m arcs sortants
    IloRangeArray CC(env);
    int nbcst{0};
    // cst (1) nb car out 0:      sum(j={1..n},x0j)<=m
    IloExpr cst1(env);
    for (int j{1}; j<=n; j++){
        cst1 += x[0][j];
    };
    CC.add( cst <= m );
    std::ostringstream cstname;
    cstname.str("");
    cstname<<"Cst_nb_car_out_"<<0;
    CC[nbcst].setName(cstname.str().c_str());
    nbcst++;

    // cst (2) nb car in 0:      sum(i={1..n},xi0)<=m
    IloExpr cst2(env);
    for (int j{1}; j<=n; j++){
        cst2 += x[i][0];
    };
    CC.add( cst <= m );
    std::ostringstream cstname;
    cstname.str("");
    cstname<<"Cst_nb_car_in_"<<0;
    CC[nbcst].setName(cstname.str().c_str());
    nbcst++;

    // csts (3) nb car out:
    for (int i{1}; i <= n; i++){
        // csts (3) nb car out i:      sum(j={0..n},x0j)=1
        IloExpr cst(env);
        for (int j{0}; j<=n; j++){
            cst += x[i][j];
        };
        CC.add(cst<=1);
        CC.add(cst>=1);

        std::ostringstream cstname;
        cstname.str("");
        cstname<<"Cst_nb_car_out_"<<i;
        CC[nbcst].setName(cstname.str().c_str());
        CC[nbcst+1].setName(cstname.str().c_str());
        nbcst += 2;
    };

    // csts (4) nb car in:
    for (int j{1}; j <= n; j++){
        // csts (3) nb car in j:      sum(i={0..n},x0j)=1
        IloExpr cst(env);
        for (int i{0}; i<=n; i++){
            cst += x[i][j];
        };
        CC.add(cst<=1);
        CC.add(cst>=1);

        std::ostringstream cstname;
        cstname.str("");
        cstname<<"Cst_nb_car_in_"<<j;
        CC[nbcst].setName(cstname.str().c_str());
        CC[nbcst+1].setName(cstname.str().c_str());
        nbcst += 2;
    };

// MTZ

    // cst (5) ：
    for (int i{1}; i<n; i++){
        for(int j{1}; j<n; j++){
            IloExpr cst(env);
            cst += w[i] - w[j] - clients[i].demand + (Q + clients[i].demand )( 1 - x[i][j] );
            CC.add( cst >= 0 )
        };
    };
    // Cst （6） :
    for (int i{0}; i<n; i++){
        for (int j{0}; j<n; j++){
            IloExpr cst(env);
            cst += x[i][j];
            cst += x[j][i];
            CC.add( cst<=1)
        }
    }

    // add Cst
    model.add(CC);

/////////////////////////////////////
////////  Obj
/////////////////////////////////////
    IloObjective obj=IloAdd(model,IloMinimize(env,0.0));
    for (int i{0}; i<n; i++)
        for (int j{0}; j<k; j++)
            obj.setLinearCoef(x[i][j],c[i][j]);
//////////////////////////////////
///////  Resolution
/////////////////////////////////
    IloCplex cplex(model);

    cplex.exportModel("sortie.lp");
    
    if ( !cplex.solve()){
       return false;
    };

    env.out()<<"Solution status = " <<cplex.getStatus()<<std::endl;
    env.out()<<"Solution value = " <<cplex.getObjValue()<<std::endl;
////////////////////////////////////
//////// get solution
////////////////////////////////////
    
    
    std::map<int,int> solution;
    for (int i{1}; i<n ; i++){
        for (int j{0}; j<n ; j++){
            if (cplex.getValue(x[i][j] == 1)){
                solution[i] = j;
            }
        }
    }

    int count{0};
    for (int i{1}; i<n; i++){
        if (cplex.getValue(x[0][i] == 1)){
            if (cars[count].empty()){
                cars[count].add_client(clients[i]);
                count++;
            }
        }
    }

    for (Car& car:cars){
        int c = car.clients[0].nb??
        while (c != 0){
            c = solution[0];
            car.add_client(clients[c]);
        }
    }        

//////////////////////////////////////////
///////  Cplex's ending
//////////////////////////////////////////
    env.end();
/////////////////////////////////////////
//////////   output
//////////////////////////////////////////

    for (auto car:cars)
        std::cout<<car<<std::endl;
    return true;
};
