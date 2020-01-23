#ifndef SOLUTION_H
#define SOLUTION_H
#include <string>
#include <vector>
#include "Instance.h"

class Solution{
    public:
    int m;
    int n;
    Instance * pinstance;
    float objValue;
    std::string status;
    std::vector<std::vector<int> > x;
    std::vector<std::vector<float> > solx;
    //constructeur
    Solution();
    //constructeur
    Solution(int n,int m, Instance * pinstance);
    //destructeur
    ~Solution();
    //affichage
    void affichage()const;
    // write_SVG_tour
    void write_SVG_tour();
    //visualisation
    void visualisation()const;
    //showpng
    void showpng()const;
};

#endif