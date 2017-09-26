/* 
 * File:   TestBlackScholesTempsQcq.cpp
 * Author: pieruccd
 *
 * Created on September 25, 2017, 5:29 PM
 */

#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <functional>
#include "../parser.hpp"
#include "../BlackScholesModel.hpp"
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <time.h>
#include <numeric>
#include <vector>

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

    char *infile = "data/asian.dat";
    Param *P = new Parser(infile);

    std::cout << "Fichier d'entrée : " << "data/asian.dat" << std::endl;

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

    BlackScholesModel *testModel = new BlackScholesModel(P);
    double t = 0.7;

    PnlRng * rng = pnl_rng_create(0);
    pnl_rng_sseed(rng, time(NULL));
    PnlMat * path = pnl_mat_create(size, timestep + 1);
    PnlMat * past = pnl_mat_create_from_file("data/market-data/simul_asian_70dates.dat");
    past = pnl_mat_transpose(past);
    testModel->asset(path, t, T, timestep, rng, past);

    std::cout << "Génération de la trajectoire suivant le modèle"
            << "de Black Scholes" 
            << "à la date " 
            << t << std::endl;

    std::cout << "Taille de la trajectoire " << path->m << "*" << path->n << std::endl;

    double mean;
    double sum;
    double sq_sum;
    double stdev;

    vector<double> trajectoire(0);
    for (int i = 0; i < path->m; i++) {
        std::cout << "Sous-jacent n° " << i << std::endl;
        for (int j = 0; j < path->n; j++) {
            std::cout << pnl_mat_get(path,i,j);
            std::cout << "->" << std::endl;
            trajectoire.push_back(pnl_mat_get(path, i, j));
            sum = std::accumulate(trajectoire.begin(), trajectoire.end(), 0.0);
            mean = sum / trajectoire.size();
            sq_sum = std::inner_product(trajectoire.begin(), trajectoire.end(), trajectoire.begin(), 0.0);
            stdev = std::sqrt(sq_sum / trajectoire.size() - mean * mean);
        }
        std::cout << "Moyenne de la trajectoire du SJ n°" << i << " = " << mean << std::endl;
        std::cout << "Standard Deviation du SJ n°" << i << " = " << stdev << std::endl;
        trajectoire.clear();
    }

    std::cout << "Fin du test" << std::endl;

    return 0;
}

