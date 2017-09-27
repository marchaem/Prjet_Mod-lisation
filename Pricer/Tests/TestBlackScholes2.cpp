/* 
 * File:   TestBlackScholes2.cpp
 * Author: kerboult
 *
 * Created on September 21, 2017, 11:58 AM
 */

#include <cstdlib>
#include <iostream>
#include "../src/parser.hpp"
#include "../src/BlackScholesModel.hpp"
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
//#include <time.h>

using namespace std;
/*
 * 
 */

int main(int argc, char **argv)
{

    int nbTimeSteps=1;
    int size=1;
    PnlMat *path= pnl_mat_create_from_zero(size,nbTimeSteps+1);
    double T = 5;
    PnlRng *rng= pnl_rng_create(0);
    char *infile = argv[1];
    Param *P = new Parser(infile);
    BlackScholesModel *testModel = new BlackScholesModel(P);
    
    std::cout << "Génération de la trajectoire suivant le modèle" 
            << "de Black Scholes" << std::endl;
    
    std::cout << "test du premier asset"<< std::endl;
    // test asset 1
    // normal
    testModel->asset(path, T, nbTimeSteps,rng);
    //  T date jusqu'à laquelle on connait la trajectoire.
    // T=0
    testModel->asset(path, 0, nbTimeSteps,rng);
    // T negatif
    testModel->asset(path, -1, nbTimeSteps,rng);

    //nbTimeSteps nombre de pas de constatation
    // nbTimeSteps=0 ( division par 0)
    testModel->asset(path, T, 0,rng);
    // nbTimeSteps negatif
    testModel->asset(path, T, -1,rng);

    // past trajectoire réalisée jusqu'a la date t
    // past null
    PnlMat *pathNULL= NULL;
    testModel->asset(pathNULL, T, nbTimeSteps,rng);
    // past vide
    PnlMat *pathVide= new PnlMat();
    testModel->asset(pathVide, T, nbTimeSteps,rng);


    std::cout << "test du 2eme asset"<< std::endl;
    // test asset 2
    PnlMat *past = pnl_mat_create_from_zero(size,nbTimeSteps+1);
    double deb=1;
    // normal
    testModel->asset(path, deb,T, nbTimeSteps,rng, past);
    // deb date jusqu'à laquelle on connait la trajectoire.
    //deb =0
    testModel->asset(path, 0,T, nbTimeSteps,rng, past);
    // deb negatif
    testModel->asset(path, -1,T, nbTimeSteps,rng, past);
    // deb superieur a T
    testModel->asset(path, T+1,T, nbTimeSteps,rng, past);

    //  T date jusqu'à laquelle on connait la trajectoire.
    // T=0
    testModel->asset(path, deb, 0, nbTimeSteps,rng , past);
    // T negatif
    testModel->asset(path, deb, -1, nbTimeSteps,rng , past);

    //nbTimeSteps nombre de pas de constatation
    // nbTimeSteps=0 ( division par 0)
    testModel->asset(path, deb, T, 0,rng , past);
    // nbTimeSteps negatif
    testModel->asset(path, deb, T, -1,rng , past);

    // path contient une trajectoire du modèle.
    // path null
    testModel->asset(pathNULL, deb, T, nbTimeSteps,rng , past);
    // path vide
    testModel->asset(pathVide, deb, T, nbTimeSteps,rng , past);

    //past trajectoire réalisée jusqu'a la date t
    //past est NULL
    PnlMat *pastNULL= NULL;
    testModel->asset(path, deb,T, nbTimeSteps,rng, pastNULL);
    //past est vide
    PnlMat *pastVide= new PnlMat();
    testModel->asset(path, deb,T, nbTimeSteps,rng, pastVide);



    std::cout << "test de shift asset"<< std::endl;
    //Shift d'une trajectoire du sous-jacent
    //normal
    PnlMat *shift_path= pnl_mat_create_from_zero(size,nbTimeSteps+1);
    int d = 1;
    double  h = 1;
    double  t = 1;
    double  timestep = 1;
    testModel->shiftAsset(shift_path, path, d, h, t, timestep);

    //shift_path
    //NULL
    PnlMat *shift_pastNULL= NULL;
    testModel->shiftAsset(shift_pastNULL, path, d, h, t, timestep);
    //Vide
    PnlMat *shift_pastVide= new PnlMat();
    testModel->shiftAsset(shift_pastVide, path, d, h, t, timestep);

    //path
    //NULL
    testModel->shiftAsset(shift_path, pathNULL, d, h, t, timestep);
    //Vide
    testModel->shiftAsset(shift_path, pathVide, d, h, t, timestep);

    //d indice du sous-jacent à shifter
    //=0
    testModel->shiftAsset(shift_path, path, 0, h, t, timestep);
    //<0
    testModel->shiftAsset(shift_path, path, -1, h, t, timestep);

    //h h pas de différences finies
    //=0
    testModel->shiftAsset(shift_path, path, d, 0, t, timestep);
    //<0
    testModel->shiftAsset(shift_path, path, d, -1, t, timestep);

    //t date à partir de laquelle on shift
    //=0
    testModel->shiftAsset(shift_path, path, d, h, 0, timestep);
    //<0
    testModel->shiftAsset(shift_path, path, d, h, -1, timestep);

    //timestep
    //=0
    testModel->shiftAsset(shift_path, path, d, h, t, 0);
    //<0
    testModel->shiftAsset(shift_path, path, d, h, t, -1);

    return 0;
}



