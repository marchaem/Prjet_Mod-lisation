#include <iostream>
#include <string>
#include <time.h>
#include "../src/parser.hpp"
#include "../src/BlackScholesModel.hpp"
#include "../src/MonteCarlo.hpp"
#include "../src/AsianOption.hpp"
#include "../src/BasketOption.hpp"
#include "../src/Hedge.hpp"

using namespace std;
int parametreSelection(int argc,char **argv){
    int i = 1;
    if (argc>2){
        if (argc>4){
            cout << "trop d argument "<< endl;
        }
        if (strcmp(argv[1],"-c")==0){
            i = 3;
        }else if (strcmp(argv[2],"-c")==0){
            cout << "mauvais ordre des paramètres mais problème résolut "<< endl;
        }else {
            cout << "option non reconu"<< endl;
        }
    }
    return i ;
}

int main(int argc, char **argv)
{   
    int i =parametreSelection(argc,argv);
    // plus haut la selection de quel partie de programe executé
   
    const clock_t begin_time = clock();
  
    double T, r, strike;
    PnlVect *spot, *sigma, *divid;
    string type;
    int size;
    size_t n_samples;

    char *infile = argv[i];
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
    
    //BlackScholesModel *testModel = new BlackScholesModel(P);
    MonteCarlo *mc =new MonteCarlo(P);
    double prix;
    double ic;
    mc->price(prix,ic);
    cout<<prix<<endl;
       cout<<ic<<endl ;
    
    //fabrication de l option
    /*
    if (type=="asian"){
        AsianOption op= new AsianOption(P);
    }else if (type=="basket"){
        BasketOption op= new BasketOption(P);
    }else if (type=="performance"){
        performanceOption op=new performanceOption(P);
    }else{
        cout << "type d option non reconu " << endl;
    }
    */
    
    /*
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
    */
    
    /*BlackScholesModel *testModel = new BlackScholesModel(P);
    double prix,mc;
    vector<double> vect (size,1.0/size);
    BasketOption *basket =new BasketOption(P);
    AsianOption *asian=new AsianOption(P);
    MonteCarlo *mt1 =new MonteCarlo(testModel,basket,0.00001,n_samples);

    MonteCarlo *mt2 =new MonteCarlo(testModel,asian,0.00001,n_samples);
    PnlVect * delta=pnl_vect_create(size);
    PnlMat * mat=pnl_mat_create(size,1);
    pnl_mat_set_col(mat,testModel->spot_,1);
    pnl_mat_print(mat);
    cout << "go delta"<<endl;
    mt1->delta(mat,0.0,delta);
    mt1->price(prix,mc);
    cout <<"delta fini"<<endl;
    pnl_vect_print(delta);
    cout <<"fini"<<endl;
    
    
    pnl_vect_free(&spot);
    pnl_vect_free(&sigma);
    pnl_vect_free(&divid);
    delete P;

    exit(0);
     */
    /*char * file= "data/market-data/simul_asian.dat";
    Hedge portefeuille = new Hedge(P,file);
    portefeuille.Majall();
    double pl = portefeuille.getPandL();
    cout<< "l'error de tracking est de : "<< pl<< endl;
     */
    
    
    cout << float( clock () - begin_time ) << endl;
 
}
