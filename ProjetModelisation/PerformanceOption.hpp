/* 
 * File:   PerformanceOption.hpp
 * Author: benjellt
 *
 * Created on September 18, 2017, 6:43 PM
 */

#ifndef PERFORMANCEOPTION_HPP
#define	PERFORMANCEOPTION_HPP
#include "Option.hpp"
class PerformanceOption : public Option {
public:
    PerformanceOption();
    PerformanceOption(double maturity, int nbtime, int size);
    PerformanceOption(double maturity, int nbtime, int size,std::vector <double> coefficient);
    PerformanceOption(const PerformanceOption& orig);
    PerformanceOption(Param * P);
    virtual ~PerformanceOption();
    double payoff(const PnlMat *path);
    void toString();
private:
    
};

#endif	/* PERFORMANCEOPTION_HPP */

