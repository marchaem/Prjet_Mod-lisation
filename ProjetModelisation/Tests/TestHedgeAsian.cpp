/* 
 * File:   TestHedgeAsian.cpp
 * Author: pieruccd
 *
 * Created on September 26, 2017, 12:16 PM
 */

#include <iostream>
#include <string>
#include "../parser.hpp"
#include "../BlackScholesModel.hpp"
#include "../MonteCarlo.hpp"
#include "../AsianOption.hpp"
#include "../BasketOption.hpp"
#include "../PerformanceOption.hpp"
#include <time.h>
#include "../Hedge.hpp"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    std::cout << "Debut du test" << std::endl;

    double T, r, strike;
    PnlVect *spot, *sigma, *divid;
    string type;
    int size;
    size_t n_samples;

    char *infile = "data/basket.dat";
    Param *P = new Parser(infile);

    std::cout << "Fichier d'entrée : " << "data/basket.dat" << std::endl;

    P->extract("option type", type);
    P->extract("maturity", T);
    P->extract("option size", size);
    P->extract("spot", spot, size);
    P->extract("volatility", sigma, size);
    P->extract("interest rate", r);
    if (P->extract("dividend rate", divid, size) == false) {
        divid = pnl_vect_create_from_zero(size);
    }
    P->extract("strike", strike);
    P->extract("sample number", n_samples);
    int timestep;
    P->extract("timestep number", timestep);
    P->print();
    
    Hedge *testHedge = new Hedge(P,"data/market-data/simul_asian.dat");
    //testHedge->computePNL();
    
    return 0;
}

