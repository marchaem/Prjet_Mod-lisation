/* 
 * File:   BasketOption.cpp
 * Author: benjellt
 * 
 * Created on September 14, 2017, 1:29 PM
 */
#include <vector>
#include "BasketOption.hpp"
#include "Option.hpp"
#include <algorithm>


using namespace std;

BasketOption::BasketOption() : Option(), Strike_(1){
    
}


BasketOption::BasketOption(double maturity, int nbtime, int size,double strike) : Option(maturity,nbtime,size), Strike_(strike){
    
}
BasketOption::BasketOption(double maturity, int nbtime, int size,double strike,std::vector <double> coefficient):Option(maturity,nbtime,size,coefficient),Strike_(strike){
    
}


BasketOption::BasketOption(const BasketOption& orig): Option(orig), Strike_(orig.Strike_){
}

BasketOption::~BasketOption() {
}

double BasketOption::getStrike() {
    return Strike_;
}

void BasketOption::setStrike(double strike) {
    this->Strike_ = strike;
}

double BasketOption::payoff(const PnlMat *path){

    PnlMat * transp = pnl_mat_transpose(path);
    PnlVect * V = pnl_vect_create(this->getsize());
    pnl_mat_get_col(V,transp,this->getnbTimeSteps()-1);
    double payoff = 0.0;
    for (int i = 0 ; i< this->getsize() ; i++)
        payoff+=this->getCoefficient(i)*GET(V,i);
    payoff-=this->getStrike();
    pnl_vect_free(&V);
    pnl_mat_free(&transp);
    if ( payoff> 0)
        return payoff;
    return 0.0;
    
}
void BasketOption::toString(){
    
    std::cout << "Il s'agit d'une option basket de maturité : " << this->getMaturity()<< endl;
    std::cout << "avec   " << this->getsize()<< " sous jacents"<< endl;
    std::cout << "le nombre de pas étudié pour cette option est : " << this->getnbTimeSteps()<< endl;
    std::cout<< "enfin les coefficient de poid de chaque sous jacents sont  dans l'odre : " << endl;
    for (int i =0 ; i< this->getsize();i++)
        std::cout<< "" << this->getCoefficient(i)<< endl;
    
}
