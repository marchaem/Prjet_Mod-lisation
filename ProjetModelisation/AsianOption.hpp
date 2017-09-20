/* 
 * File:   AsianOption.hpp
 * Author: benjellt
 *
 * Created on September 18, 2017, 10:10 AM
 */

#ifndef ASIANOPTION_HPP
#define	ASIANOPTION_HPP
#include "Option.hpp"
#include "parser.hpp"

class AsianOption : public Option {
public:
    AsianOption();
    AsianOption(Param *P);
    AsianOption(double maturity, int nbtime, int size,double strike);
    AsianOption(double maturity, int nbtime, int size,double strike,std::vector <double> coefficient);
    AsianOption(const AsianOption& orig);
    virtual ~AsianOption();

    double getStrike();
    void setStrike(double strike);
    double payoff(const PnlMat *path);
    void toString();
private:
    double Strike_;
};

#endif	/* ASIANOPTION_HPP */

