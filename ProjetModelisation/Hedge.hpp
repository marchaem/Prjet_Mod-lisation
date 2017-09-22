/* 
 * File:   Hedge.hpp
 * Author: benjellt
 *
 * Created on September 21, 2017, 11:24 AM
 */

#ifndef HEDGE_HPP
#define	HEDGE_HPP

#include "Option.hpp"



class Hedge {
public:
    Hedge();
    Hedge(Option option);
    
    Hedge(const Hedge& orig);
    virtual ~Hedge();
    Option getOption();
    void setOption(Option option);
    PnlMat* getdelta();
    void setDelta(PnlMat* delta);
private:
    Option option_;
    PnlMat* delta;
};

#endif	/* HEDGE_HPP */

