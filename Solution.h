#ifndef SOLUTION_H
#define SOLUTION_H
#include <string>
#include <vector>

class Solution{
    public:
    int m;
    int n;
    float objValue;
    std::string status;
    std::vector<std::vector<int> > x;
    std::vector<std::vector<float> > solx;
    //constructeur
    Solution();
    //constructeur
    Solution(int n,int m);
    //destructeur
    ~Solution();
    //affichage
    void affichage()const;
    //visualisation
    void visualisation()const;
    //showpng
    void showpng()const;
};

#endif