#include <iostream>
#include <string>
#include "parser.hpp"
#include "BlackScholesModel.hpp"
#include "MonteCarlo.hpp"
#include "AsianOption.hpp"

using namespace std;

int main(int argc, char **argv)
{
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
    
    BlackScholesModel *testModel = new BlackScholesModel(P);
    int timestep;
    P->extract("timestep number",timestep);
    PnlRng *rng = pnl_rng_create(0);
    vector<double> vect (size,1.0/size);
    cout << size << " " <<n_samples <<endl;
    AsianOption *manu =new AsianOption(T,n_samples,size,strike,vect);
    manu->toString();
    cout << "les coef valent" << manu->getCoefficient(1)<<endl;
    MonteCarlo *mt =new MonteCarlo(testModel,manu,timestep,n_samples);
    double prix=0.0;
    double ic=0.0;
    mt->price(prix,ic);
    
    pnl_vect_free(&spot);
    pnl_vect_free(&sigma);
    pnl_vect_free(&divid);
    delete P;

    exit(0);
}
