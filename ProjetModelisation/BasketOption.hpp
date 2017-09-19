/* 
 * File:   BasketOption.hpp
 * Author: benjellt
 *
 * Created on September 14, 2017, 1:29 PM
 */

#ifndef BASKETOPTION_HPP
#define	BASKETOPTION_HPP
#include "Option.hpp"

class BasketOption : public Option {
public:
    BasketOption();
    BasketOption(double maturity, int nbtime, int size,double strike);
    BasketOption(const BasketOption& orig);
    BasketOption(double maturity, int nbtime, int size,double strike,std::vector <double> coefficient);
    virtual ~BasketOption();
    
    double getStrike();
    void setStrike(double strike);
    double payoff(const PnlMat *path);
    void toString();
    
private:
    double Strike_;
};

#endif	/* BASKETOPTION_HPP */

