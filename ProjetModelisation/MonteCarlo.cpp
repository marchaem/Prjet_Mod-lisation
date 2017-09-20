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
    PnlMat * path = pnl_mat_create(this->opt_->getsize(),this->opt_->getnbTimeSteps()+1);
    PnlRng * rng = pnl_rng_create(0);
    PnlMat * shiftplus = pnl_mat_create(this->opt_->getsize(),this->opt_->getnbTimeSteps()+1);
    PnlMat * shiftmoins = pnl_mat_create(this->opt_->getsize(),this->opt_->getnbTimeSteps()+1);
    double timestep=(this->opt_->getMaturity())/this->opt_->getnbTimeSteps();
    double payoffplus;
    double payoffmoins;
    double tmp=0.0;
    if(t==0.0){
        cout <<"on est dans le if"<<endl;
        for (int i =0; i< this->opt_->getsize();i++){
            for (int j=0 ; j<this->nbSamples_; j++){
                this->mod_->asset(path,this->opt_->getMaturity(),this->opt_->getnbTimeSteps(),rng);
                this->mod_->shiftAsset(shiftplus,path,i,this->fdStep_,0.0,timestep);
                this->mod_->shiftAsset(shiftmoins,path,i,-this->fdStep_,0.0,timestep);
                cout <<"on est dans la boucle"<<endl;
                payoffmoins=this->opt_->payoff(shiftmoins);
                payoffplus= this->opt_->payoff(shiftplus);
                cout <<"payoffplus vaut" <<payoffplus<<endl;
                cout <<"payoffmoins vaut" <<payoffmoins<<endl;
                tmp+=payoffplus-payoffmoins;
            }
            cout << "après le for du delta "<< path->n <<endl;
            tmp/= this->nbSamples_*this->fdStep_*2*MGET(past,i,past->n-1);
            cout <<"après le GET" << endl;
            tmp*=exp(-(this->mod_->r_*(this->opt_->getMaturity()-t)));
            pnl_vect_set(delta,i,tmp);
            cout << "après le set et tmp vaut"<<tmp<<endl;
            tmp=0.0;
        }
        
    }
    else{
        for (int i =0; i< this->opt_->getsize();i++){
                 for (int j ; j<this->nbSamples_; j++){
                    this->mod_->asset(path,t,this->opt_->getMaturity(),this->opt_->getnbTimeSteps(),rng,past);
                    this->mod_->shiftAsset(shiftplus,path,i,this->fdStep_,0.0,timestep);
                    this->mod_->shiftAsset(shiftmoins,path,i,-this->fdStep_,0.0,timestep);
                    payoffmoins =this->opt_->payoff(shiftmoins);
                    payoffplus= this->opt_->payoff(shiftplus);
                    tmp+=payoffplus-payoffmoins;
            }
                tmp/= this->nbSamples_*this->fdStep_*2*MGET(past,i,path->n);   
                tmp*=exp(-(this->mod_->r_*(this->opt_->getMaturity()-t)));
                pnl_vect_set(delta,i,tmp);
                tmp=0.0;
        }
    }
    
    
}

void MonteCarlo::calcDelta0(const PnlMat* past, PnlVect* delta) {
    
}

void MonteCarlo::CalcDelta_t(const PnlMat* past, double t, PnlVect* delta) {

}






