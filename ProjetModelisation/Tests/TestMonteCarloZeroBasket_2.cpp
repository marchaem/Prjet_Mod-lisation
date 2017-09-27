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

    char *infile = "data/basket_2.dat";
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
    std::cout << "Prix attendu : 9.238710 " << std::endl;
    std::cout << "Calcul du delta en zero " << std::endl;
    PnlVect *deltas = pnl_vect_create(size);
    mc->calcDelta0(deltas);
    vector<double> error(0);
    double deltasDat[] = {0.0162094, 0.01621, 0.01622, 0.0162203, 0.0162145,
        0.0162044, 0.016215, 0.0162163, 0.0161995, 0.0161993, 0.0162085,
        0.0161999, 0.0161848, 0.0161824, 0.0162018, 0.0161994, 0.0161971,
        0.0162066, 0.0162119, 0.0162086, 0.0162088, 0.0162014, 0.0162149,
        0.0162236, 0.0162311, 0.0162138, 0.0162064, 0.0162129, 0.0162166,
        0.0162183, 0.0161925, 0.0162276, 0.016189, 0.0161958, 0.0162017,
        0.0162142, 0.0162028, 0.0162092, 0.0162104, 0.0162025};
    for (int i = 0; i < size; i++) {
        /*std::cout << "Valeur Obtenue : " << pnl_vect_get(deltas,i) 
                << " Valeur Attendue : " << deltasDat[i] 
                << " Difference = " << pnl_vect_get(deltas,i) - deltasDat[i] 
                << std::endl;*/
        error.push_back(std::abs(pnl_vect_get(deltas, i) - deltasDat[i]));
    }
    std::sort(error.begin(), error.end());
    std::cout << "Erreur Min = " << error[0] << std::endl;
    std::cout << "Erreur Max = " << error[size - 1] << std::endl;


    return 0;
}

