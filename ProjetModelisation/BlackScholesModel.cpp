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
     
    PnlMat* cov = pnl_mat_create_from_scalar(this->size_,this->size_,this->rho_);
    pnl_mat_set_diag(cov,1,this->size_);
    pnl_mat_chol(cov);
    
    double pas =T/nbTimeSteps;     int i = 0;
    PnlVect* Gi = pnl_vect_create(this->size_);
    double S0;
    double St;
    double sigmad;
    PnlVect* Ld = pnl_vect_create(this->size_);
    for (int d=0; d<this->size_; d++) { 
        S0 = pnl_vect_get(this->spot_,d);
        pnl_mat_set(path,d,0,S0);
        sigmad = pnl_vect_get(this->sigma_,d);
        pnl_mat_get_row(Ld,cov,d);
        for (int i=1; i<=nbTimeSteps; i++) {           
            pnl_vect_rng_normal(Gi,this->size_,rng);            
            St = S0 * exp((this->r_- pow(sigmad,2)/2) * pas + sigmad *sqrt(pas)*pnl_vect_scalar_prod(Ld,Gi));           
            S0=St;          
            pnl_mat_set(path,d,i,St); 
        }
    }  
    pnl_vect_free(&Ld);
    pnl_mat_free(&cov);
    pnl_vect_free(&Gi);
    
}

void BlackScholesModel::asset(PnlMat* path, double t, double T, int nbTimeSteps, PnlRng* rng, const PnlMat* past) {
    
    PnlMat* cov = pnl_mat_create_from_scalar(this->size_,this->size_,this->rho_);
    pnl_mat_set_diag(cov,1,this->size_);
    pnl_mat_chol(cov);
    
    pnl_mat_chol(cov);
    double S;
    double Sigmad;
    double pas = 0;
    PnlVect* Gi = pnl_vect_create(this->size_);
    PnlVect* Ld = pnl_vect_create(this->size_);
    
    for (int d=0; d<this->size_; d++) {
        double St = pnl_mat_get(past,d,(past->n)-1);
        Sigmad = pnl_vect_get(this->sigma_,d);
        pnl_mat_get_row(Ld,cov,d);
        for (int j=0; j<nbTimeSteps; j++) {
            pnl_vect_rng_normal(Gi,this->size_,rng);
            pas = j*T/nbTimeSteps - t;
            if (j<(past->n)-1) {
                pnl_mat_set(path,d,j,pnl_mat_get(past,d,j)); 
            } else {
                S = St * exp( (this->r_- pow(Sigmad,2)/2) * pas + Sigmad *sqrt(pas)*pnl_vect_scalar_prod(Ld,Gi));
            }
        }
    }  
}

void BlackScholesModel::shiftAsset(PnlMat* shift_path, const PnlMat* path, int d, double h, double t, double timestep) {
    
    int indicet = this->getPasTemps(t,timestep,path->n);
    pnl_mat_clone(shift_path,path);
    double wesh;
    for (int j= indicet+1;j<path->n;j++){
        wesh=MGET(path,d,j);
        pnl_mat_set(shift_path,d,j,wesh*(1+h));
    }
    
   
    
}

int BlackScholesModel::getPasTemps(double t, double timestep,int nbTimeStep) {
    double pasDeTps = timestep;
    int indiceCour = 0;
    double dist = 0;
    if(t==0)
        return 0;
    while (dist <= t) {
        dist += pasDeTps;
        indiceCour++;
    }
    return indiceCour-1;
}


