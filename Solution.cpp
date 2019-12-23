#include "Solution.h"
#include <vector>
//constructeur par defaut
Solution::Solution(){};
//constructeur
Solution::Solution(int n,int m){
    x.resize(n,std::vector<int>(m));
};
//destructeur
Solution::~Solution(){};