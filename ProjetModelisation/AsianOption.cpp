/* 
 * File:   AsianOption.cpp
 * Author: benjellt
 * 
 * Created on September 18, 2017, 10:10 AM
 */
using namespace std;
#include "AsianOption.hpp"

AsianOption::AsianOption(): Option(), Strike_(1) {
}
AsianOption::AsianOption(double maturity, int nbtime, int size,double strike) : Option(maturity,nbtime,size), Strike_(strike){
    
}
AsianOption::AsianOption(double maturity, int nbtime, int size,double strike,std::vector <double> coefficient) : Option(maturity,nbtime,size,coefficient),Strike_(strike){
    
}


AsianOption::AsianOption(const AsianOption& orig): Option(orig), Strike_(orig.Strike_){
}


AsianOption::~AsianOption() {
}
double AsianOption::payoff(const PnlMat *path){
    cout <<"asian " <<endl;
    PnlMat * transp = pnl_mat_transpose(path);
    double payoff =0.0;
    double tmp=0.0;
    for (int d =0; d<this->getsize();d++){
        tmp=0.0;
        for (int i =0; i<this->getnbTimeSteps()+1;i++){          
            tmp+=MGET(transp,i,d);
        }
        cout <<"coef vaut "<<this->getCoefficient(d)<<endl;
        payoff+=this->getCoefficient(d)*tmp/(getnbTimeSteps()+1);   
    }
    cout <<"vrai payoff vaut "<<payoff<<endl;
    pnl_mat_free(&transp);
    if (payoff > this->Strike_)
        return (payoff-this->Strike_);
    return 0.0;  
}

void AsianOption::toString(){
    
    std::cout << "Il s'agit d'une option asiatique de maturité : " << this->getMaturity()<< endl;
    std::cout << "avec   " << this->getsize()<< " sous jacents"<< endl;
    std::cout << "le nombre de pas étudié pour cette option est : " << this->getnbTimeSteps()<< endl;
    std::cout<< "enfin les coefficient de poid de chaque sous jacents sont  dans l'odre : " << endl;
    for (int i =0 ; i< this->getsize();i++)
        std::cout<< "" << this->getCoefficient(i)<< endl;
    
}

