/* 
 * File:   TestMonteCarloZeroBasket.cpp
 * Author: pieruccd
 *
 * Created on September 25, 2017, 4:03 PM
 */

#include <cstdlib>
#include <iostream>
#include "../parser.hpp"
#include "../BlackScholesModel.hpp"
#include "../BasketOption.hpp"
#include "../MonteCarlo.hpp"
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    double T, r, strike;
    PnlVect *spot, *sigma, *divid;
    string type;
    int size;
    size_t n_samples;
    int timestep;

    char *infile = "data/basket.dat";
    Param *P = new Parser(infile);

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
    P->extract("timestep number", timestep);
    P->print();

    BlackScholesModel *testModel = new BlackScholesModel(P);

    PnlMat * path = pnl_mat_create(size, timestep + 1);

    BasketOption *basketOption = new BasketOption(P);

    double prix;
    double ic;

    MonteCarlo *mc = new MonteCarlo(testModel, basketOption, timestep, n_samples);

    mc->price(prix, ic);

    std::cout << "Prix de l'option en zero = " << prix << std::endl;
    std::cout << "Prix attendu : 13,627 " << std::endl;

    return 0;
}

