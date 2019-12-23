#ifndef SOLUTION_H
#define SOLUTION_H
#include <string>
#include <vector>

class Solution{
    public:
    float objValue;
    std::string status;
    std::vector<std::vector<int>> x;
    //constructeur
    Solution();
    //constructeur
    Solution(int n,int m);
    //destructeur
    ~Solution();
};

#endif