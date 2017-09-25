#include <iostream>
#include <string>
#include "parser.hpp"
#include "BlackScholesModel.hpp"
#include "MonteCarlo.hpp"
#include "AsianOption.hpp"
#include "BasketOption.hpp"
#include "PerformanceOption.hpp"
#include <time.h>
#include "Hedge.hpp"


using namespace std;

int main(int argc, char **argv)
{
    double T, r, strike;
    PnlVect *spot, *sigma, *divid;
    string type;
    int size;
    int timestep;
    size_t n_samples;
    double prix,ic;
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
  //  P->extract("sample number", n_samples);

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
    P->extract("timestep number",timestep);
    cout<<"timestep "<<timestep<<endl;
    
    
   /* BlackScholesModel *testModel = new BlackScholesModel(P);
    double prix,mc;
    MonteCarlo *mt =new MonteCarlo(P);

    PnlRng * rng=pnl_rng_create(0);
    pnl_rng_sseed(rng,time(NULL));
    PnlVect * delta=pnl_vect_create(size);
    PnlMat * past=pnl_mat_create_from_file("data/market-data/manu.dat");
    past=pnl_mat_transpose(past);
    PnlMat * path=pnl_mat_create(size,timestep+1);
   // mt2->price(prix,mc);
    mt->price(past,0.0,prix,mc);
    cout <<"fini"<<endl;
    
    
    pnl_vect_free(&spot);
    pnl_vect_free(&sigma);
    pnl_vect_free(&divid);
    delete P;

    exit(0);*/
    PnlMat * past=pnl_mat_create(size,timestep);
    PnlVect *delt=pnl_vect_create(size);
    char * file= "data/market-data/simul_basket.dat";
   /* MonteCarlo *mt =new MonteCarlo(P);
    mt->price(prix,ic);
    mt->delta(past,0.0,delt);*/
    Hedge * portefeuille = new Hedge(P,file);
    portefeuille->Majall();
    cout << "on sort de Majall"<<endl;
    double pl = portefeuille->getPandL();
    cout<< "l'error de tracking est de : "<< pl<< endl;
     
    
}
