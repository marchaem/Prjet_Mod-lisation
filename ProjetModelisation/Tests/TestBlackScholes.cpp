/* 
 * File:   TestBlackScholes.cpp
 * Author: pieruccd
 *
 * Created on September 20, 2017, 12:57 PM
 */

#include <cstdlib>
#include <iostream>
#include "../parser.hpp"
#include "../BlackScholesModel.hpp"
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
    PnlMat * path = pnl_mat_create(size,timestep+1);
    testModel->asset(path,T,timestep,rng);
    
    std::cout << "Génération de la trajectoire suivant le modèle" 
            << "de Black Scholes" << std::endl;
    
    std::cout << "Taille de path " << path->m << "*" << path->n << std::endl;
    
    for (int i=0; i<path->m; i++) {
        std::cout << "Sous-jacent n° " << i << std::endl;
        for (int j=0; j<path->n; j++) {
            std::cout << pnl_mat_get(path,i,j);
            std::cout << "->" << std::endl;
        }
        std::cout << std::endl;
    }
    
    std::cout << "Fin du test" << std::endl;
    
    
    return 0;
}

