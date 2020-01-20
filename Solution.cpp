#include "Solution.h"
#include <fstream>
#include <string>
#include <vector>
//constructeur par defaut
Solution::Solution(){};
//constructeur
Solution::Solution(int n_,int m_):m(m_),n(n_){
    x.resize(n_);
    for (int i=0; i<n; i++){
        x[i].resize(m_);
    };

    solx.resize(n);
    for (int i=0; i<n; i++){
        solx[i].resize(n);
    };
};
//destructeur
Solution::~Solution(){};
//affichage
void Solution::affichage()const{
    for (int j=0; j<m; j++){
        printf("Route #%d:",j+1);
        for (int i=1; i<n; i++){
            (x[i][j] == 1)? printf(" %d",i):true;      
        };
        printf("\n");
    };
    printf("cost %f \n", this->objValue);
};
void Solution::visualisation()const{
    std::ofstream file;
    std::string dot;
    int i,j;
    dot = "digraph cvrp{\n";
    for (i=0; i< n; i++){
         dot += std::to_string(i+1);
         dot += ";\n";
    }
    for (i=0; i<n; i++){
        for (j=0; j<n; j++){
            if (solx[i][j]==1){
                dot += std::to_string(i+1);
                dot += "->";
                dot += std::to_string(j+1);
                dot += ";";
            }
        }
    }
    dot += "}";
    file.open("cvrp.dot",std::fstream::out);
    file<<dot;
    file.close();
    system("dot -Tpng cvrp.dot -o cvrp.png");
}
void Solution::showpng()const{
    system("eog cvrp.png");
}    
