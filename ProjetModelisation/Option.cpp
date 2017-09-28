using namespace std;
#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "Option.hpp"
/// \brief Classe Option abstraite

    
Option::Option(): Maturity_(1),nbTimeSteps_(1) , size_(1),coefficient_(pnl_vect_create(1)){
    
}
Option::Option(double T, int nbTimeSteps, int size): Maturity_(T),nbTimeSteps_(nbTimeSteps),size_(size),coefficient_(pnl_vect_create(size)){
    
}
    
Option::Option(double T, int nbTimeSteps, int size,std::vector <double> coefficient) {
    Maturity_=T;
    nbTimeSteps_= nbTimeSteps;
    size_=size;
    this->coefficient_= pnl_vect_create(size_);
    for (int i=0; i<size_; i++) {
        pnl_vect_set(this->coefficient_,i,coefficient[i]);
    }
}
    
Option::Option(Option const& autre): Maturity_(autre.Maturity_), nbTimeSteps_(autre.nbTimeSteps_), size_(autre.size_),coefficient_(autre.coefficient_){
    
}
Option::Option(Param *P){
    P->extract("maturity", this->Maturity_);
    P->extract("option size", this->size_);
    std::vector<double> second (size_);
    P->extract("payoff coefficients", second, this->size_);
    this->coefficient_= pnl_vect_create(size_);
    for (int i=0; i<size_; i++) {
        pnl_vect_set(this->coefficient_,i,second[0]);
    }
    P->extract("timestep number", this->nbTimeSteps_);
    if (Maturity_<=0){
        throw string ("Attention la maturité est négative ou nulle");
    }
    if (size_==0){
        throw string(" Attention il n y a pas de sous-jacents");
    }
    if ( size_*this->getCoefficient(0) !=1 ){
        throw string ("Attention les coefficents sont mal répartis entre les actions");
    }
}
Option::~Option(){
    pnl_vect_free(&this->coefficient_);
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
    return pnl_vect_get(this->coefficient_,i);
}
void Option::setCoefficient(double d, int i){
    pnl_vect_set(this->coefficient_,i,d);
}

void Option::print() {
    cout << "Maturity = " << Maturity_ << endl;
    cout << "Option Size = " << size_ << endl;
    cout << "Payoff Coefficients" << endl;
    pnl_vect_print(this->coefficient_);
    cout << "TimeStep Number = " << nbTimeSteps_ << endl;
}


