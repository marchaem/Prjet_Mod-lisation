/* 
 * File:   TestDeltaZero.cpp
 * Author: pieruccd
 *
 * Created on September 21, 2017, 11:05 AM
 */

#include <cstdlib>

#include <cstdlib>
#include <iostream>
#include "../parser.hpp"
#include "../BlackScholesModel.hpp"
#include "../AsianOption.hpp"
#include "../MonteCarlo.hpp"
#include "../BasketOption.hpp"
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

    P->print();
    cout << endl;
    cout << "option type " << type << endl;
    cout << "maturity " << T << endl;
    cout << "strike " << strike << endl;
    cout << "option size " << size << endl;
    cout << "interest rate " << r << endl;
    cout << "dividend rate ";
    pnl_vect_print_asrow(divid);
    cout << "spot ";
    pnl_vect_print_asrow(spot);
    cout << "volatility ";
    pnl_vect_print_asrow(sigma);
    cout << "Number of samples " << n_samples << endl;
    int timestep;
    P->extract("timestep number",timestep);
    
    BlackScholesModel *testModel = new BlackScholesModel(P);
    double prix,ic;
    vector<double> vect (size,1.0/size);
    BasketOption *basket =new BasketOption(P);
    AsianOption *asian=new AsianOption(P);
    MonteCarlo *mt1 =new MonteCarlo(testModel,basket,0.00001,n_samples);
    MonteCarlo *mt2 =new MonteCarlo(testModel,asian,0.00001,n_samples);
    PnlVect * delta=pnl_vect_create(size);
    PnlMat * mat=pnl_mat_create(size,1);
    pnl_mat_set_col(mat,testModel->spot_,1);
    //pnl_mat_print(mat);
    cout << "go delta"<<endl;
    mt1->delta(mat,0.0,delta);
    mt1->price(prix,ic);
    cout <<"delta fini"<<endl;
    pnl_vect_print(delta);
    cout <<"fini"<<endl;
    
    
    pnl_vect_free(&spot);
    pnl_vect_free(&sigma);
    pnl_vect_free(&divid);
    delete P;
    
    return 0;
}

