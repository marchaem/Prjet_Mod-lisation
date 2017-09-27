/* 
 * File:   TestPricingTempsQcq.cpp
 * Author: pieruccd
 *
 * Created on September 25, 2017, 5:06 PM
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
int main(int argc, char** argv) {

    double T, r, strike;
    PnlVect *spot, *sigma, *divid;
    string type;
    int size;
    size_t n_samples;
    int timestep;

    char *infile = "data/asian.dat";
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

    AsianOption *asianOption = new AsianOption(P);

    double prix;
    double ic;
    
    double t = 0.705;

    PnlMat * past = pnl_mat_create_from_file("data/market-data/simul_asian_71dates.dat");
    past = pnl_mat_transpose(past);

    MonteCarlo *mc = new MonteCarlo(testModel, asianOption, timestep, n_samples);

    mc->price(past, t, prix, ic);

    std::cout << "Prix de l'option en t = " << t << " : " << prix << std::endl;
    
    /*Section a débugger encore ...*/
    PnlMat * vraieTrajectoire = pnl_mat_create_from_file("data/market-data/simul_asian.dat");
    vraieTrajectoire = pnl_mat_transpose(vraieTrajectoire);
    double PrixTh = asianOption->payoff(vraieTrajectoire);  
    std::cout << "Prix attendu : " << PrixTh * exp(-(testModel->r_)*(T-t)) << std::endl;

}

