#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "BinPacking.cpp"
using namespace std;

#define DEBUG
class Instance{
    public:
    int n;  // nb clients
    float UB;//upper bound
    float Q; //capacity
    Client depot;
    vector<Client> clients;
    Instance(){};
    void print(){
        for (int i{0}; i<n; i++){
            cout<<clients[i]<<endl;
        };
    };
};
int main(){
    Instance instance;
    ifstream in;
    in.open("Vrp-Set-tai/tai/tai75a.dat");
    if ( ! in.is_open()){
        std::cout<<"Error when opening file"; exit(1);
    };
    std::string s;
    in>>instance.n>>instance.UB>>instance.Q;
    #ifdef DEBUG
        cout<<"n  "<<instance.n<<endl;
        cout<<"UB "<<instance.UB<<endl;
        cout<<"Q  "<<instance.Q<<endl;
    #endif
    float x,y,d;
    int n;
    in>>x>>y;
    instance.depot = Client(0,x,y);
    while (!in.eof()){
        in>>n>>d>>x>>y;
        instance.clients.push_back(Client(n,d,x,y));
    };
    in.close();
    #ifdef DEBUG
    instance.print();
    #endif

    return 0;
}