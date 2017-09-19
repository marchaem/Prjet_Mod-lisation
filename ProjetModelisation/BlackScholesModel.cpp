using namespace std;

#include "BlackScholesModel.hpp"
#include <exception>
#include <math.h>
#include <cmath>

BlackScholesModel::BlackScholesModel(Param *param) {
    param->extract("interest rate", this->r_);
    param->extract("option size", this->size_);
    param->extract("correlation", this->rho_);
    param->extract("volatility",this->sigma_,this->size_);
    param->extract("spot",this->spot_,this->size_);
}


void BlackScholesModel::asset(PnlMat* path, double T, int nbTimeSteps, PnlRng* rng) {
    
    
    
    PnlMat* cov = pnl_mat_create_from_zero(this->size_,this->size_);
    for (int i=0; i<this->size_; i++) {
        for (int j=0; j<this->size_; j++) {
           
            if (i==j) {
                pnl_mat_set(cov,i,j,1);
            }
            else {
                pnl_mat_set(cov,i,j,this->rho_);
            }
        } 
    }
    
    pnl_mat_chol(cov);
    
    double pas =T/nbTimeSteps;  //à vérifier
    PnlVect* Gi = pnl_vect_create(this->size_);
    int i = 0;
    double S0;
    double St;
    double Sigmad;
    PnlVect* Ld = pnl_vect_create(this->size_);
    for (int d=0; d<this->size_; d++) { 
        
        /*On met S0 en première valeur*/
        S0 = pnl_vect_get(this->spot_,d);
        pnl_mat_set(path,d,0,S0);
        i = 1;
        Sigmad = pnl_vect_get(this->sigma_,d);
        pnl_mat_get_row(Ld,cov,d);
        while (i<=nbTimeSteps) {
            
            pnl_vect_rng_normal(Gi,this->size_,rng);
            
            St = S0 * exp((this->r_- pow(Sigmad,2)/2) * pas + Sigmad *sqrt(pas)*pnl_vect_scalar_prod(Ld,Gi));
           
            S0=St;
           
            pnl_mat_set(path,d,i,St); 
            i++;
        }
        cout << endl;
    }
    
    pnl_vect_free(&Ld);
    pnl_mat_free(&cov);
    
    
    
}

void BlackScholesModel::asset(PnlMat* path, double t, double T, int nbTimeSteps, PnlRng* rng, const PnlMat* past) {
    
    path = pnl_mat_create_from_zero(nbTimeSteps+1,this->size_);
    PnlVect* browniens = pnl_vect_create(this->size_);
    pnl_vect_rng_normal(browniens,this->size_,rng);
    PnlMat* cov = pnl_mat_create_from_zero(this->size_,this->size_);
    for (int i=0; i<this->size_; i++) {
        for (int j=0; j<this->size_; j++) {
            if (i==j) {
                pnl_mat_set(cov,i,j,1);
            }
            else {
                pnl_mat_set(cov,i,j,this->rho_);
            }
        } 
    }
    
    /*Il faudrait pouvoir renvoyer une exception en cas d'erreur*/
    pnl_mat_chol(cov);
    
    double pasDeTps = T/nbTimeSteps;
    double dateCour = 0;
    int m = 0;
    while (t<dateCour) {
        t += dateCour + pasDeTps;
        m++;
    }
    
    for (int d=0; d<this->size_; d++) {
        
    }
    
    
}

void BlackScholesModel::shiftAsset(PnlMat* shift_path, const PnlMat* path, int d, double h, double t, double timestep) {
}


