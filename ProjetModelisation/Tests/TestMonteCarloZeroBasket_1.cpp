/* 
 * File:   TestMonteCarloZeroBasket_1.cpp
 * Author: pieruccd
 *
 * Created on September 25, 2017, 4:16 PM
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

    char *infile = "data/basket_1.dat";
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
    std::cout << "Prix attendu : 13.616294 " << std::endl;
    
    std::cout << "Calcul du delta en zero " << std::endl;
    PnlVect *deltas = pnl_vect_create(size);
    vector<double> error(0);
    mc->calcDelta0(deltas);
    double deltasDat[] = {0.0248956, 0.0249324, 0.0249503, 0.0250191, 0.0249342, 0.024961, 0.0249808, 
            0.0248986, 0.0249032, 0.0249421, 0.0249554, 0.0249416, 0.0249236, 0.0249541, 
            0.0249515, 0.0250078, 0.0248439, 0.0248783, 0.0249421, 0.0249178, 0.0249168, 
            0.0249511, 0.0249393, 0.0250007, 0.0249344, 0.0248973, 0.0249136, 0.0249047, 
            0.024927, 0.0248953, 0.0249739, 0.024962, 0.0249467, 0.0249322, 0.0249571, 
            0.0249858, 0.0248853, 0.024914, 0.024861, 0.0250143};
    for (int i=0; i<size; i++) {
        /*std::cout << "Valeur Obtenue : " << pnl_vect_get(deltas,i) 
                << " Valeur Attendue : " << deltasDat[i] 
                << " Difference = " << pnl_vect_get(deltas,i) - deltasDat[i] 
                << std::endl;*/
        error.push_back(std::abs(pnl_vect_get(deltas,i) - deltasDat[i]));
    }
    std::sort(error.begin(),error.end());
    std::cout << "Erreur Min = " << error[0] << std::endl;
    std::cout << "Erreur Max = " << error[size-1] << std::endl;

    return 0;
}

