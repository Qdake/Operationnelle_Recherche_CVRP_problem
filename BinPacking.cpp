#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <string>
#include <sstream>
#include "ilcplex/ilocplex.h"


#define DEBUG
class Client{
    public:
    static int objectCount;
    int number;
    float demand;
    Client(float demand_):demand(demand_){
        objectCount++;
        this->number = objectCount;
    };
    ~Client(){
        objectCount = objectCount - 1;
    };
    //print
    std::ostream& print(std::ostream& out)const{
        out<<this->demand;
        return out;
    };
};
int Client::objectCount{0};
std::ostream& operator<<(std::ostream& out, const Client& Client){
    Client.print(out);
    return out;
} 
class Car{
    public:
    static int objectCount;
    int number;
    float Q; //capacite
    float demand;
    std::vector<Client> clients;
    //constructor
    Car(float Q_):Q(Q_),demand(0){
        objectCount++;
        number = objectCount;
    };
    //destructor
    ~Car(){
        objectCount = objectCount - 1;
    };
    //add_client
    bool add_client(const Client& client){
        if (client.demand + this->demand > this->Q)
            return false;
        this->demand += client.demand;
        this->clients.push_back(client);
        return true;
    };
    // clear_clients
    void clear_clients(){
        this->clients.clear();
        this->demand = 0;
    }
    //print
    std::ostream& print(std::ostream& out)const{
        out<<"car "<<this->number<<" :";
        for (const Client& client:this->clients)
            out<<client<<" ";
        return out;
    }
};
int Car::objectCount{0};
std::ostream& operator<<(std::ostream& out, const Car& car){
    car.print(out);
    return out;
}
bool comparator(const Client& client1,const Client& client2){
    return (client1.demand>client2.demand)?true:false;
}
bool first_fit_decreasing(std::vector<Car>& cars,std::vector<Client>& clients){
    /* Sorting the clients into decreasing order and then taking the items in the order given,
     placing each item in the first available bin, starting with Bin 1 every time.*/
    
    // sort
    std::sort(clients.begin(),clients.end(),comparator);

    #ifdef DEBUG
        for (auto i:clients)
            std::cout<<i<<" ";
        std::cout<<std::endl;
    #endif

    // first fit
    for (Client& client:clients){
        bool success{false};
        for (Car& car:cars){
            success = car.add_client(client);
            if (success)
                break;
        };
        if (!success)
            return false;
    };
    return true;
}
bool plne(std::vector<Car>& cars, std::vector<Client>& clients){
    /* utiliser plne pour trouver un bin packing exact;
        true s'il existe une solution, false sinon;*/

    int n = Client::objectCount;  // nb client
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
    // Cst capacity :   sum( x_i_j * di,i=1..n)<=Q     for j in 1..k
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
        std::cout<<"cars[j].clients.size() apres clear =" <<cars[j].clients.size()<<" "<<std::endl;
        #ifdef DEBUG
        std::cout<<"car "<<j<<": "<<std::endl;
        #endif
        for (int i{0}; i<n; i++){     
            if (cplex.getValue(x[i][j]) == 1){
                bool success = cars[j].add_client(clients[i]);
                #ifdef DEBUG
                std::cout<<"add success? "<<success<<std::endl;
                std::cout<<"cars[j].clients.size() apres add_client =" <<cars[j].clients.size()<<" "<<std::endl;
                std::cout<<clients[i].demand<<" ";
                #endif
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
    std::cout<<"test 1"<<std::endl;
    for (auto car:cars)
        std::cout<<car<<std::endl;
    return true;
};


bool plne_min(std::vector<Car>& cars, std::vector<Client>& clients){
    /* utiliser plne pour trouver un bin packing exact;
        true s'il existe une solution, false sinon;*/

    int n = Client::objectCount;  // nb client
    int k = Car::objectCount;   // nb vehicle
///////////////////
/////// Cplex initialization
////////////////////
    IloEnv env;
    IloModel model(env);
///////////////////////
/////  Var
//////////////////////
    // X
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
    // Y
    std::vector<IloNumVar> y;
    y.resize(k);
    for (int i{0}; i<Car::objectCount; i++){
        y[i] = IloNumVar(env, 0.0, 1.0, ILOFLOAT);
    }
/////////////////////////
//////// CST
/////////////////////////
    IloRangeArray CC(env);
    int nbcst{0};
    // Cst capacity :   sum( x_i_j * di,i=1..n)<=Q     for j in 1..k
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
    // Cst : y[j] >= x[i][j]  for i in [1...n]  for j in [1...m]
    for (int j{0}; j<k; j++)
        for (int i{0}; i<n; i++){
            IloExpr cst(env);
            cst += y[j];
            cts += -x[i][j];
            CC.add(cst >= 0);
        }
    // Cst : sum(x_i_j, i=1...n) >= y[j]   for j in [1..k]
    for (int j{0}; j<k; j++){
        IloExpr cst(env);
        for (int i{0}; i<n; i++)
            cst += x[i][j];
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
    for (int i{0}; i<n; i++)
        for (int j{0}; j<k; j++)
            obj.setLinearCoef(x[i][j],0);
    // y 
    for (int j{0}; j<k; j++)
        obj.setLinearCoef(y[j],1);
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
        std::cout<<"cars[j].clients.size() apres clear =" <<cars[j].clients.size()<<" "<<std::endl;
        #ifdef DEBUG
        std::cout<<"car "<<j<<": "<<std::endl;
        #endif
        for (int i{0}; i<n; i++){     
            if (cplex.getValue(x[i][j]) == 1){
                bool success = cars[j].add_client(clients[i]);
                #ifdef DEBUG
                std::cout<<"add success? "<<success<<std::endl;
                std::cout<<"cars[j].clients.size() apres add_client =" <<cars[j].clients.size()<<" "<<std::endl;
                std::cout<<clients[i].demand<<" ";
                #endif
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
    std::cout<<"test 1"<<std::endl;
    std::cout<<"";
    for (auto car:cars)
        std::cout<<car<<std::endl;
    return true;
};

bool binPacking(std::vector<Car>& cars, std::vector<Client>& clients){
    //initialization
    bool success = first_fit_decreasing(cars,clients);
    if (success){
        #ifdef DEBUG
        std::cout<<"first_fit_decreasing found a solution of binPacking problem"<<std::endl;
        #endif
        return true;
    };
    return plne(cars,clients);
};   
int main(){s

    std::vector<float> demands{2,2,2,3,3,4};
    float Q = 8;
    int k = 2;

    // std::vector<float> demands{3,6,2,3,2,7};
    // float Q = 10;
    // int k = 3;
    std::vector<Client> clients;
    for (auto demand:demands)
        clients.push_back(Client(demand));
    std::vector<Car> cars;
    for (int i{0};i<k;i++)
        cars.push_back(Car(Q));

    std::cout<<"first fit decreasing ** *********"<<std::endl;
    bool success = first_fit_decreasing(cars,clients);
    std::cout<<"found a solution? "<< success<<std::endl;
    for (auto car:cars)
        std::cout<<car<<std::endl;
    std::cout<<"cars.size = "<<cars.size()<<std::endl;

    std::cout<<"plne ** *********"<<std::endl;
    for (auto car:cars)
        car.clear_clients();
    success = plne(cars,clients);
    std::cout<<"found a solution? "<< success<<std::endl;
    std::cout<<"test 2"<<std::endl;
    for (auto car:cars)
        std::cout<<car<<std::endl;
    std::cout<<"cars.size = "<<cars.size()<<std::endl;    

    // plne_min
    std::cout<<"plne_min *********"<<std::endl;
    for (auto car:cars)
        car.clear_clients();
    success = plne(cars,clients);
    std::cout<<"found a solution? "<< success<<std::endl;
    std::cout<<"test 3"<<std::endl;
    for (auto car:cars)
        std::cout<<car<<std::endl;
    std::cout<<"cars.size = "<<cars.size()<<std::endl;    

    // std::vector<float> demands{8,5,7,5};
    // float capacity = 10;
    // int k = 2;
    // std::vector<Client> clients;
    // for (auto demand:demands)
    //     clients.push_back(Client(demand));
    // std::vector<Car> cars;
    // for (int i{0};i<k;i++)
    //     cars.push_back(Car(capacity));
    // bool success = first_fit_decreasing(cars,clients);
    // std::cout<<"found a solution? "<< success<<std::endl;
    // for (auto car:cars)
    //     std::cout<<car<<std::endl;
    // std::cout<<"cars.size = "<<cars.size()<<std::endl;


    return 0;
};