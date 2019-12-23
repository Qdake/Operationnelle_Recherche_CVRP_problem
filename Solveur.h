#ifndef SOLVEUR_H
#define SOLVEUR_H
#include <string>
#include "Instance.h"
#include "Configuration.h"
class Solveur{
    public:
    Instance* pinstance;
    Configuration* psolution;
    bool (Solveur::*pmethode)();
    //constructeur
    Solveur(){};
    //destructeur
    ~Solveur(){
        if (psolution != nullptr){
            delete psolution;
        };
        if (pinstance != nullptr){
            delete pinstance;
        }
    };
    // set_instance
    void set_instance(Instance* pinstance);
    // set_method
    void set_method(string methode);
    // solve
    bool solve();
    // bin_plne_heuristique
    bool bin_plne_heuristique();
    // bin_glouton_heuristique
    bool bin_glouton_heuristique();
    // bin_plne_min_heuristique
    bool bin_plne_min_heuristique();
    // plne branch and cut
    bool plne_branch_and_cut();
    // plne MTZ
    bool plne_MTZ();
    // plne flots
    bool plne_flots();
};
#endif