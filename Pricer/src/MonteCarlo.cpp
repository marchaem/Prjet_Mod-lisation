using namespace std;
#include "MonteCarlo.hpp"
#include "Option.hpp"
#include "BasketOption.hpp"
#include "AsianOption.hpp"
#include "PerformanceOption.hpp"


MonteCarlo::MonteCarlo(Param *P) {
    this->mod_=new BlackScholesModel(P);
    string typeoption;
    int nbre=0;
    double maturity;
    
    P->extract("option type",typeoption);
    if (typeoption=="basket"){
        this->opt_= new BasketOption(P);
    }
    else if (typeoption=="asian"){     
        this->opt_=new AsianOption(P);
        }
    else if(typeoption=="performance"){            
        this->opt_=new PerformanceOption(P); 
    }
    else{
        throw string("Nous ne connaissons ce type d'option, veuillez-vous adresser à monsieur Taramasco");
        }
    this->rng_=pnl_rng_create(0);
    pnl_rng_sseed(this->rng_,time(NULL));
    
    P->extract("timestep number",nbre);
   // P->extract("sample number",this->nbSamples_);
    P->extract("maturity",maturity);
    this->fdStep_=maturity/nbre;  
}


MonteCarlo::MonteCarlo(BlackScholesModel* black, Option* opt, double fdStep, int nbSamples) {
    this->mod_=black;
    this->fdStep_=fdStep;
    this->nbSamples_=nbSamples;
    this->opt_=opt;
}


void MonteCarlo::price(double &prix, double &ic) {
    double payoffCour = 0.0;
    double PrixCumul = 0.0;
    double sommeCarres = 0.0;
    double ecartype=0.0;
    double EmCarre;
    PnlMat *mat = pnl_mat_create(this->mod_->size_,this->opt_->getnbTimeSteps()+1);
    for(int j=0; j < this->nbSamples_; j++){   
        this->mod_->asset(mat,this->opt_->getMaturity(),this->opt_->getnbTimeSteps(),this->rng_); 
        payoffCour=this->opt_->payoff(mat);
        PrixCumul+=payoffCour;
        sommeCarres+=pow(payoffCour,2);       
    }
    prix = PrixCumul/this->nbSamples_ * exp(-this->mod_->r_*this->opt_->getMaturity());
    
    EmCarre = ( sommeCarres/this->nbSamples_ - pow((PrixCumul/this->nbSamples_),2) ) * exp(-2*this->mod_->r_*this->opt_->getMaturity());
    ecartype=sqrt(EmCarre)/sqrt(this->nbSamples_);
    ic = 1.96*2 *sqrt(EmCarre)/sqrt(this->nbSamples_);
    cout <<"le prix vaut "<<prix<<" et l'ecart type :"<<ecartype<<endl;
    pnl_mat_free(&mat);
    
}


void MonteCarlo::price(const PnlMat* past, double t, double& prix, double& ic) {
    double payoffCour = 0.0;
    double PrixCumul = 0.0;
    double tolerance=0.0001;
    double t_cour=0.0;
    double sommeCarres = 0.0;
    double ecartype=0.0;
    double EmCarre;
    int nbColConsta=past->n-1;
    while(t_cour<=this->opt_->getMaturity()){
        if(t>=t_cour-tolerance && t<=t_cour+tolerance){
            cout<<"t est sur la grille"<<endl; 
            t=t_cour;
        }
        t_cour+=this->opt_->getMaturity()/this->opt_->getnbTimeSteps();
    }
    double pas = this->opt_->getMaturity()/this->opt_->getnbTimeSteps();
    double t_i=(nbColConsta-1)*pas;
    if(!(t>=t_i && t<=t_i+pas)){
        cout <<"le past n'est pas bon pour pricer en t"<<endl;
        return;
    }

    
    PnlMat *mat = pnl_mat_create(this->mod_->size_,this->opt_->getnbTimeSteps()+1);
    for(int j=0; j < this->nbSamples_; j++){   
        this->mod_->asset(mat,t,this->opt_->getMaturity(),this->opt_->getnbTimeSteps(),this->rng_,past); 
        payoffCour=this->opt_->payoff(mat);
        PrixCumul+=payoffCour;  
        sommeCarres+=pow(payoffCour,2);       

    }
    EmCarre = (sommeCarres/this->nbSamples_ - pow((PrixCumul/this->nbSamples_),2) ) * exp(-2*this->mod_->r_*this->opt_->getMaturity());
    ecartype=sqrt(EmCarre)/sqrt(this->nbSamples_);
    prix = PrixCumul/this->nbSamples_ * exp(-this->mod_->r_*(this->opt_->getMaturity()-t));
    pnl_mat_free(&mat);
    cout <<"le prix vaut "<<prix<<" et l'ecart type :"<<ecartype<<endl;

}

/*
 *Calcul du delta dans tous les cas

 */

void MonteCarlo::delta(const PnlMat* past, double t, PnlVect* delta) {   
    if(t==0.0)
        calcDelta0(past,delta);         
    else if(t>0.0)
        CalcDelta_t(past,t,delta);
    else
        throw string("on ne peut pas calculer pour un temps négatif");
    
    
    
}

/*
 *calcul du delta en t = 0
 */

void MonteCarlo::calcDelta0(const PnlMat* past, PnlVect* delta) {
    PnlMat * path = pnl_mat_create(this->opt_->getsize(),this->opt_->getnbTimeSteps()+1);
    PnlMat * shiftplus = pnl_mat_create(this->opt_->getsize(),this->opt_->getnbTimeSteps()+1);
    PnlMat * shiftmoins = pnl_mat_create(this->opt_->getsize(),this->opt_->getnbTimeSteps()+1);
    double timestep=(this->opt_->getMaturity())/this->opt_->getnbTimeSteps();
    double payoffplus;
    double payoffmoins;
    double tmp=0.0;
    for (int i =0; i< this->opt_->getsize();i++){
            for (int j=0 ; j<this->nbSamples_; j++){
                this->mod_->asset(path,this->opt_->getMaturity(),this->opt_->getnbTimeSteps(),this->rng_);
                this->mod_->shiftAsset(shiftplus,path,i,this->fdStep_,0.0,timestep);
                this->mod_->shiftAsset(shiftmoins,path,i,-this->fdStep_,0.0,timestep);
                payoffmoins=this->opt_->payoff(shiftmoins);
                payoffplus= this->opt_->payoff(shiftplus);
                tmp+=payoffplus-payoffmoins;
            }
            tmp/= this->nbSamples_*this->fdStep_*2*MGET(past,i,past->n-1);
            tmp*=exp(-(this->mod_->r_*(this->opt_->getMaturity())));
            pnl_vect_set(delta,i,tmp);
            tmp=0.0;
        }
    pnl_mat_free(&path);
    pnl_mat_free(&shiftplus);
    pnl_mat_free(&shiftmoins);
    

        
}

/*
 *calcul du delta en t positif
 */

void MonteCarlo::CalcDelta_t(const PnlMat* past, double t, PnlVect* delta) {
    PnlMat * path = pnl_mat_create(this->opt_->getsize(),this->opt_->getnbTimeSteps()+1);
    PnlMat * shiftplus = pnl_mat_create(this->opt_->getsize(),this->opt_->getnbTimeSteps()+1);
    PnlMat * shiftmoins = pnl_mat_create(this->opt_->getsize(),this->opt_->getnbTimeSteps()+1);
    double timestep=(this->opt_->getMaturity())/this->opt_->getnbTimeSteps();
    double payoffplus;
    double payoffmoins;
    double tmp=0.0;
    for (int i =0; i< this->opt_->getsize();i++){
                 for (int j ; j<this->nbSamples_; j++){
                    this->mod_->asset(path,t,this->opt_->getMaturity(),this->opt_->getnbTimeSteps(),this->rng_,past);
                    this->mod_->shiftAsset(shiftplus,path,i,this->fdStep_,t,timestep);
                    this->mod_->shiftAsset(shiftmoins,path,i,-this->fdStep_,t,timestep);
                    payoffmoins =this->opt_->payoff(shiftmoins);
                    payoffplus= this->opt_->payoff(shiftplus);
                    tmp+=payoffplus-payoffmoins;
            }
                tmp/= this->nbSamples_*this->fdStep_*2*MGET(past,i,path->n);   
                tmp*=exp(-(this->mod_->r_*(this->opt_->getMaturity()-t)));
                pnl_vect_set(delta,i,tmp);
                tmp=0.0;
        }
    pnl_mat_free(&path);
    pnl_mat_free(&shiftplus);
    pnl_mat_free(&shiftmoins);
}






