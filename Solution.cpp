#include "Solution.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <climits>
using namespace::std;
#define epsilon 0.00001
//constructeur par defaut
Solution::Solution(){};
//constructeur
Solution::Solution(int n_,int m_, Instance * pinstance_):m(m_),n(n_),pinstance(pinstance_){
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

    printf("Q = %f\n", pinstance->Q);
    for (int j=0; j<m; j++){
        int s = 0;
        printf("Route #%d:",j+1);
        cout<<" somme_demande = ";
        for (int i=1; i<n; i++){
            if (x[i][j] == 1){    
                s+=pinstance->clients[i].demande;
                printf(" + x[%d][%d].demande(%d, %d) ",i,j,pinstance->clients[i].demande,s);
            };
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
            if (solx[i][j] > epsilon){
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
void Solution::write_SVG_tour(){

    int i;
    ostringstream FileName; 
    FileName.str("");
    FileName <<"G_tour.svg";

    float dimx=600;
    float dimy=600;
    int minx=10000;
    int miny=10000;
    int maxy=-100000;
    int maxx=-100000;
    for (int i=0; i<n;i++){
        if (maxx < this->pinstance->clients[i].x) maxx = this->pinstance->clients[i].x;
        if (maxy < this->pinstance->clients[i].y) maxy = this->pinstance->clients[i].y;
        if (minx > this->pinstance->clients[i].x) minx = this->pinstance->clients[i].x;
        if (miny > this->pinstance->clients[i].y) miny = this->pinstance->clients[i].y;
    };  
    ofstream fic(FileName.str().c_str());

  fic<<"<html>\n <body>\n <svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.2\"";
  fic<<" width=\"600\" height=\"600\"";
  fic<<" viewBox=\""<<-2.0<<" "<<-2.0<<" "<<dimx+7.0<<" "<<dimy+7.0<<"\"";
  fic<<" preserveAspectRatio=\"yes\">"<<endl;
  fic<<"<g>"<<endl<<endl;


  for (i=0;i<n;i++){

        fic<<"<circle cx=\""<<dimx*(pinstance->clients[i].x-minx)/(maxx-minx)<<"\" cy=\""<<dimy*(pinstance->clients[i].y-miny)/(maxy-miny)<<"\" r=\"2\" stroke=\"0\" stroke-width=\"2\" fill=\"0\" />"<<endl;

  }

  for (int i=0; i<n; i++)
    for (int j=0;j<n; j++){
        if (solx[i][j] < epsilon) continue; 
//    for (it=sol.begin();it!=sol.end();it++){
        fic<<"<line x1=\""<<dimx*(pinstance->clients[i].x-minx)/(maxx-minx)<<"\"";
        fic<<" y1=\""<<dimy*(pinstance->clients[i].y-miny)/(maxy-miny)<<"\"";
        fic<<" x2=\""<<dimx*(pinstance->clients[j].x-minx)/(maxx-minx)<<"\"";
        fic<<" y2=\""<<dimy*(pinstance->clients[j].y-miny)/(maxy-miny)<<"\"";
        fic<<" style=\"stroke:rgb(255,0,0);stroke-width:1\" />"<<endl;
    }

  fic<<endl<<endl<<"</g></svg></body></html>"<<endl;
  fic.close();

}
//showpng
void Solution::showpng()const{
    system("eog cvrp.png");
}    
