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
    int H;
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
    P->extract("hedging dates number",H);
    cout<<"timestep "<<timestep<<endl;
    
   
    
    
 



    char * file= "data/market-data/simul_perf.dat";
    
    //mt->price(prix,ic);
    //mt->delta(past,0.0,delt);
    Hedge * portefeuille = new Hedge(P,file);
    
    portefeuille->Majall();
  
    
    double pl = portefeuille->getPandL();
    cout<< "l'error de tracking est de : "<< pl<< endl;
     
    
}
