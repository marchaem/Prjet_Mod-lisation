/* 
 * File:   main.cpp
 * Author: kerboult
 *
 * Created on September 28, 2017, 9:58 AM
 */

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
/*
 * 
 */
bool avecOption = FALSE ; 
char * file;
char *infile;
int parametreSelection(int argc,char **argv){
    int i = 1;
    if (argc>2){
        if (argc>4){
            cout << "trop d argument "<< endl;
        }
        if (strcmp(argv[1],"-c")==0){
            file=argv[2];
            infile=argv[3];
            avecOption=TRUE;
            i = 3;
        }else if (strcmp(argv[2],"-c")==0){
            file=argv[3];
            infile=argv[1];
            avecOption=TRUE;
            cout << "mauvais ordre des paramètres mais problème résolut "<< endl;
        }else {
            cout << "option non reconu"<< endl;
        }
    }else {
        infile=argv[1];
    }
    return i ;
}

int main(int argc, char **argv)
{   
    
    int i =parametreSelection(argc,argv);
    // plus haut la selection de quel partie de programe executé
    if (avecOption){
        Param *P = new Parser(infile);
        Hedge * portefeuille = new Hedge(P,file);
        PnlMat * past = portefeuille->getPast();
        int i =portefeuille->getIndice(1.5);
        portefeuille->Majall();
        double pl = portefeuille->getPandL();
        cout<< "l'error de tracking est de : "<< pl<< endl;
    }else {// sans option 
        const clock_t begin_time = clock();
    
        Param *P = new Parser(infile);
        MonteCarlo *mc =new MonteCarlo(P);
        double prix;
        double ic;
        mc->price(prix,ic);
        cout << endl;
        cout<<"le prix est de "<<prix<<endl;
        cout<<"l'intervalles de confiance est de "<<ic<<endl;
        PnlVect *delta= pnl_vect_create_from_double(mc->opt_->getsize(),0);
        mc->calcDelta0(delta);
    
        for (int i =0; i< (mc->opt_->getsize()); i++){
            cout<<"le delta du "<<i+1<<"eme soucjacant "<<pnl_vect_get(delta,i)<<endl; 
        }
    
        cout <<"le temp d execution est de : " <<float( clock () - begin_time )/CLOCKS_PER_SEC << "s"<<endl;
    }
    
}


