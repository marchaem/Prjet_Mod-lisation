/* 
 * File:   Hedge.cpp
 * Author: benjellt
 * 
 * Created on September 21, 2017, 11:24 AM
 */

#include "Hedge.hpp"
#include "MonteCarlo.hpp"
using namespace std;


Hedge::Hedge(Param* P, char * fichier){
    this->mt_= new MonteCarlo(P);
    int nbtrebalencement;
    P->extract("hedging dates number",nbtrebalencement);
    this->delta= pnl_mat_create(this->mt_->opt_->getsize(),nbtrebalencement+1);
    this->past= pnl_mat_create_from_file(fichier);
    this->profit_and_lost=0.0;
    this->NbtreRebalencement=nbtrebalencement;
}

PnlMat * Hedge::getPast(){
    return this->past;
}
void Hedge::Maj(double t, const PnlMat* past){
    
    PnlVect * deltat = pnl_vect_create(this->mt_->opt_->getsize());
    PnlVect * Srt = pnl_vect_create(this->mt_->opt_->getsize());
    // PnlMat * transp=pnl_mat_transpose(past);
    
    pnl_mat_get_col(Srt,past,getIndice(t));
    
    
    this->mt_->delta(past,t,deltat);
    
    int j =getIndice(t);
    pnl_mat_set_col(this->delta,deltat,j);
    if (t==0.0){
        double ic;
        this->mt_->price(this->profit_and_lost,ic);
        this->profit_and_lost-=pnl_vect_scalar_prod(Srt,deltat);
    }
    else {
        PnlVect * deltat_moins_un =pnl_vect_create(this->mt_->opt_->getsize());
        pnl_mat_get_col(deltat_moins_un,this->delta,j-1);
        
        PnlVect * tmp =pnl_vect_copy(deltat);
        this->profit_and_lost*=exp(this->mt_->mod_->r_ * this->mt_->opt_->getMaturity()/this->NbtreRebalencement);
        pnl_vect_minus_vect(tmp,deltat_moins_un);

      
        this->profit_and_lost-=pnl_vect_scalar_prod(tmp,Srt);                
        pnl_vect_free(&tmp);
        pnl_vect_free(&deltat_moins_un);
    }
    pnl_vect_free(&Srt);
    pnl_vect_free(&deltat);
}


void Hedge::Majall(){
    double t=0.0;
    double pasDetps= this->mt_->opt_->getMaturity()/this->NbtreRebalencement;
    PnlMat * past;
    for (int i =0;  i<this->NbtreRebalencement +1 ;i++){
        past=getHisto(t);
        Maj(t,past);
        t+=pasDetps;              
    }
    PnlVect * Sfin = pnl_vect_create(this->mt_->opt_->getsize());
    pnl_mat_get_row(Sfin,this->past,this->NbtreRebalencement);
    PnlVect * deltafin = pnl_vect_create(this->mt_->opt_->getsize());
    pnl_mat_get_col(deltafin,this->delta,this->NbtreRebalencement);

    this->profit_and_lost+= pnl_vect_scalar_prod(deltafin,Sfin);
    

    //PnlMat * trajectoire= pnl_mat_create(this->mt_->opt_->getsize(),this->mt_->opt_->getnbTimeSteps()+1);
    //this->mt_->mod_->asset(trajectoire,this->mt_->opt_->getMaturity(),this->mt_->opt_->getnbTimeSteps(),this->mt_->rng_);
   
    
    
    //this->profit_and_lost-= this->mt_->opt_->payoff(trajectoire);
    PnlMat * traj = GetTrajectoire();
    this->profit_and_lost-=this->mt_->opt_->payoff(GetTrajectoire());
    pnl_mat_free(&past);
    pnl_vect_free(&Sfin);
    pnl_vect_free(&deltafin);
    //pnl_mat_free(&trajectoire);
}

PnlMat * Hedge::getHisto(double t){
    
    PnlMat past_t = pnl_mat_wrap_mat_rows(this->past,0,getIndice(t));   
    return pnl_mat_transpose(&past_t);
}
PnlMat * Hedge::GetTrajectoire(){
    //PnlMat * transp = pnl_mat_transpose(this->past);
    PnlVect * spot=pnl_vect_create(this->mt_->opt_->getsize());
    pnl_mat_get_row(spot,this->past,0);
    int fraction = this->NbtreRebalencement/this->mt_->opt_->getnbTimeSteps();
    PnlMat * traj = pnl_mat_create(this->mt_->opt_->getsize(),this->mt_->opt_->getnbTimeSteps()+1);
    pnl_mat_set_col(traj,spot,0);
    
    PnlVect *St=pnl_vect_create(this->mt_->opt_->getsize());
    int i=1;
    for (int q=fraction; q<this->NbtreRebalencement+1; q+=fraction){
        pnl_mat_get_row(St,this->past,q);
        pnl_mat_set_col(traj,St,i);
        i++;
    }    
    pnl_vect_free(&St);
    pnl_vect_free(&spot);
    
    return traj;
}

int Hedge::getIndice(double t){
    if(t==0)
        return 0;
    double pasDeTps = this->mt_->opt_->getMaturity()/this->NbtreRebalencement;
    
    int indiceCour = 0;
    double dist = 0.0;
    while (dist <= t) {
        
        dist += pasDeTps;
        indiceCour++;
    }
    return indiceCour-1;
}


Hedge::Hedge(const Hedge& orig) {
}

Hedge::~Hedge() {
}
PnlMat* Hedge::getdelta(){
    return this->delta;
}
void Hedge::setDelta(PnlMat* delta){
    this->delta=delta;
}
double Hedge::getPandL(){
    return this->profit_and_lost;
}