/* 
 * File:   Hedge.hpp
 * Author: benjellt
 *
 * Created on September 21, 2017, 11:24 AM
 */

#ifndef HEDGE_HPP
#define	HEDGE_HPP

#pragma once

#include "Option.hpp"
#include "MonteCarlo.hpp"



class Hedge {
public:    
    Hedge(Param * P, char *  file);    
    Hedge(const Hedge& orig);
    virtual ~Hedge();   
    PnlMat* getdelta();
    void setDelta(PnlMat* delta);
    void Maj(double t, const PnlMat* past);
    int getIndice(double t);
    void Majall();
    PnlMat * getHisto(double t);
    double getPandL();
    PnlMat * GetTrajectoire();
    PnlMat * getPast();
private:
    MonteCarlo * mt_;
    PnlMat* delta;
    PnlMat* past;
    double profit_and_lost;
    int NbtreRebalencement;
};

#endif	/* HEDGE_HPP */

