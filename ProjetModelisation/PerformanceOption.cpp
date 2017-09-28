/* 
 * File:   PerformanceOption.cpp
 * Author: benjellt
 * 
 * Created on September 18, 2017, 6:43 PM
 */

#include "PerformanceOption.hpp"

PerformanceOption::PerformanceOption(): Option() {
}
PerformanceOption::PerformanceOption(double maturity, int nbtime, int size): Option(maturity, nbtime,  size) {
}
PerformanceOption::PerformanceOption(double maturity, int nbtime, int size, std::vector<double> coefficient):Option(maturity, nbtime,  size, coefficient) {
}
PerformanceOption::PerformanceOption(const PerformanceOption& orig):Option(orig) {
}
PerformanceOption::PerformanceOption(Param* P):Option(P){
    
}
PerformanceOption::~PerformanceOption() {
}
double PerformanceOption::payoff(const PnlMat *path){ 
    
    double payoff =0.0;
    double tmp1=0.0;
    double tmp2=0.0;

    for (int i =1 ; i<this->getnbTimeSteps()+1;i++){
        tmp1=0.0;
        tmp2=0.0;
        for (int d =0; d<this->getsize();d++){          
            tmp1+=this->getCoefficient(d)*MGET(path,d,i);          
            tmp2+=this->getCoefficient(d)*MGET(path,d,i-1);           
        }
        
        if( tmp1/tmp2 >1 )
        payoff+= (tmp1/tmp2-1);  
    } 
    return (1.0+payoff);
}
void PerformanceOption::toString(){
    
    std::cout << "Il s'agit d'une option performance de maturité : " << this->getMaturity()<< endl;
    std::cout << "avec   " << this->getsize()<< " sous jacents"<< endl;
    std::cout << "le nombre de pas étudié pour cette option est : " << this->getnbTimeSteps()<< endl;
    std::cout<< "enfin les coefficient de poid de chaque sous jacents sont  dans l'odre : " << endl;
    for (int i =0 ; i< this->getsize();i++)
        std::cout<< "" << this->getCoefficient(i)<< endl;
   
} 

