#include "Solution.h"
#include <vector>
//constructeur par defaut
Solution::Solution(){};
//constructeur
Solution::Solution(int n_,int m_):m(m_),n(n_){
    x.resize(n_);
    for (int i=0; i<n; i++){
        x[i].resize(m_);
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
};