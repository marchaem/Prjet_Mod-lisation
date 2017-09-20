using namespace std;
#include "MonteCarlo.hpp"
#include "Option.hpp"


MonteCarlo::MonteCarlo() {

}


MonteCarlo::MonteCarlo(BlackScholesModel* black, Option* opt, double fdStep, int nbSamples) {
    this->mod_=black;
    this->fdStep_=fdStep;
    this->nbSamples_=nbSamples;
    this->opt_=opt;
}


void MonteCarlo::price(double &prix, double &ic){
    double somme=0.0;
    double somme1 = 0.0;
    double somme2=0.0;
    double somme3=0.0;
    double var = 0.0;
    PnlRng * rng = pnl_rng_create(0);
    PnlMat * mat = pnl_mat_create(this->mod_->size_,this->opt_->getnbTimeSteps()+1);
    for(int j=0; j < this->nbSamples_; j++){      
        this->mod_->asset(mat,this->opt_->getMaturity(),this->opt_->getnbTimeSteps(),rng);
        somme=this->opt_->payoff(mat);
        somme1+=somme;
        somme2+=pow(somme,2);       
    }
    
    pnl_mat_free(&mat);
    pnl_rng_free(&rng);
    somme1/=this->nbSamples_;
    somme2/=this->nbSamples_;
    somme3=pow(somme1,2);
    prix=somme1*exp(-this->mod_->r_*this->opt_->getMaturity());
    var=exp(-this->mod_->r_*this->opt_->getMaturity())*sqrt(somme2-somme3);
    ic=3.92*var/sqrt(this->nbSamples_);
    cout << " l'option vaut " << prix << " et a pour largeur d'intervalle de confiance " << ic <<endl; 
    
}


void MonteCarlo::price(const PnlMat* past, double t, double& prix, double& ic) {

}

void MonteCarlo::delta(const PnlMat* past, double t, PnlVect* delta) {

}




