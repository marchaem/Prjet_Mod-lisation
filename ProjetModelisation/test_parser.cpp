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
    char *infile = "data/basket.dat";
    Param *P = new Parser(infile);

    char * file= "data/market-data/simul_basket.dat";
    
    Hedge * portefeuille = new Hedge(P,file);
  
    portefeuille->Majall();
  
    double pl = portefeuille->getPandL();
    cout<< "l'error de tracking est de : "<< pl<< endl;
    
    return 0;
     
    
}
