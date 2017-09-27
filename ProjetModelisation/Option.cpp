using namespace std;
#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "Option.hpp"
/// \brief Classe Option abstraite

    
Option::Option(): Maturity_(1),nbTimeSteps_(1) , size_(1),coefficient_(1){
    
}
Option::Option(double T, int nbTimeSteps, int size): Maturity_(T),nbTimeSteps_(nbTimeSteps),size_(size),coefficient_(size){
    
}
    
Option::Option(double T, int nbTimeSteps, int size,std::vector <double> coefficient): coefficient_(coefficient){
    Maturity_=T;
    nbTimeSteps_= nbTimeSteps;
    size_=size;
    
    
    
    
}
    
Option::Option(Option const& autre): Maturity_(autre.Maturity_), nbTimeSteps_(autre.nbTimeSteps_), size_(autre.size_),coefficient_(autre.coefficient_){
    
}
Option::Option(Param *P){
    P->extract("maturity", this->Maturity_);
    P->extract("option size", this->size_);
    P->extract("payoff coefficients", this->coefficient_, this->size_);
    std::vector<double> second (size_,coefficient_[0]);
    this->coefficient_=second;
    P->extract("timestep number", this->nbTimeSteps_);
    if (Maturity_==0){
        throw string ("Attention la maturité est nulle");
    }
    if (size_==0){
        throw string(" Attention il n y a pas de sous-jacents");
    }
    if ( size_*coefficient_[0] !=1 ){
        throw string ("Attention les coefficents sont mal répartis entre les actions");
    }
}
Option::~Option(){
    coefficient_.~vector();
}
double Option::getMaturity() {  
    return this->Maturity_;
}


void Option::setMaturity(double d) {
    this->Maturity_=d;
}
int Option::getnbTimeSteps() {
    return this->nbTimeSteps_;
}
void Option::setnbTimeSteps(int n){
    this->nbTimeSteps_=n;
}
int Option::getsize(){
    return this->size_;
}
void Option::setsize(int size){
    this->size_= size;
}
double Option::getCoefficient(int i){
    return this->coefficient_[i];
}
void Option::setCoefficient(double d, int i){
    this->coefficient_[i]=d;
}


