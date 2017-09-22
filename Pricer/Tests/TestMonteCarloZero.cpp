/* 
 * File:   TestMonteCarloZero.cpp
 * Author: pieruccd
 *
 * Created on September 20, 2017, 1:32 PM
 */

#include <cstdlib>
#include <iostream>
#include "../parser.hpp"
#include "../BlackScholesModel.hpp"
#include "../AsianOption.hpp"
#include "../MonteCarlo.hpp"
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"


using namespace std;

/*
 * 
 */

/*
int main(int argc, char** argv) {
    
    double T, r, strike;
    PnlVect *spot, *sigma, *divid;
    string type;
    int size;
    size_t n_samples;

    char *infile = argv[1];
    Param *P = new Parser(infile);

    P->extract("option type", type);
    P->extract("maturity", T);
    P->extract("option size", size);
    P->extract("spot", spot, size);
    P->extract("volatility", sigma, size);
    P->extract("interest rate", r);
    if (P->extract("dividend rate", divid, size) == false)
    {
        divid = pnl_vect_create_from_zero(size);
    }
    P->extract("strike", strike);
    P->extract("sample number", n_samples);
    int timestep;
    P->extract("timestep number",timestep);
    P->print();
    
    BlackScholesModel *testModel = new BlackScholesModel(P);
    
    PnlRng * rng = pnl_rng_create(0);
    PnlMat * path = pnl_mat_create(size,timestep);
    
    AsianOption *asianOption = new AsianOption(P);
    asianOption->setCoefficient(0.5,1);
    
    double prix;
    double ic;
    
    MonteCarlo *mc = new MonteCarlo(testModel, asianOption, timestep , n_samples); // A changer peut -être 
    mc->price(prix,ic);

    std::cout << "Prix de l'option = " << prix << " ic = " << ic << std::endl;
    
    return 0;
}
 * */

