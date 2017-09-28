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
    Hedge(Param * P, char *  file);   // données  simulés par nos évaluateurs 
    Hedge(Param* P); // données simulé par nous
    ~Hedge();   
    PnlMat* getdelta();
    void setDelta(PnlMat* delta);
    void Maj(double t, const PnlMat* past);
    void MajZero();
    int getIndice(double t);
    void Majall();
    PnlMat * getHisto(double t);
    double getPandL();
    PnlMat * getPast();
    MonteCarlo * getMt();
    int Getnbtreb();
    PnlMat * GetGethisto();
private:
    MonteCarlo * mt_;
    PnlMat* delta;
    PnlMat* past;
    double profit_and_loss;
    int NbtreRebalencement;
    PnlMat * gethisto;
    
};

#endif	/* HEDGE_HPP */

