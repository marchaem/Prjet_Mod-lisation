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
    PnlMat * past_t=pnl_mat_create(this->mt_->opt_->getsize(),1);
    PnlVect * vect = pnl_vect_create(this->mt_->opt_->getsize());
    pnl_mat_get_row(vect,this->past,0);
    pnl_mat_set_col(past_t,vect,0);
    pnl_vect_free(&vect);     
    this->gethisto=  past_t;
    
}
Hedge::Hedge(Param* P){
    this->mt_= new MonteCarlo(P);
    int nbtrebalencement;
    P->extract("hedging dates number",nbtrebalencement);
    this->delta= pnl_mat_create(this->mt_->opt_->getsize(),nbtrebalencement+1);
    PnlMat * path =  pnl_mat_create(this->mt_->opt_->getsize(),nbtrebalencement+1);
    this->mt_->mod_->simul_market(path,this->mt_->opt_->getMaturity(),nbtrebalencement,this->mt_->rng_);
    this->past=pnl_mat_transpose(path);    
    this->profit_and_lost=0.0;
    this->NbtreRebalencement=nbtrebalencement;
    PnlMat * past_t=pnl_mat_create(this->mt_->opt_->getsize(),1);
    PnlVect * vect = pnl_vect_create(this->mt_->opt_->getsize());
    pnl_mat_get_row(vect,this->past,0);
    pnl_mat_set_col(past_t,vect,0);
    pnl_vect_free(&vect);     
    this->gethisto=  past_t;
    
    
}



PnlMat * Hedge::getPast(){
    return this->past;
}
MonteCarlo * Hedge::getMt(){
    return this->mt_;
}
int Hedge::Getnbtreb(){
    return this->NbtreRebalencement;
}
PnlMat * Hedge::GetGethisto(){
    return this->gethisto;
}
void Hedge::MajZero() {
    PnlVect * deltat = pnl_vect_create(this->mt_->opt_->getsize());
    PnlVect * Srt = pnl_vect_create(this->mt_->opt_->getsize());
    pnl_mat_get_row(Srt,this->past,0);
    this->mt_->calcDelta0(deltat);
    double ic;
    double prix;
    this->mt_->price(prix,ic);
    pnl_mat_set_col(this->delta,deltat,0);
    this->profit_and_lost=prix;
    this->profit_and_lost-=pnl_vect_scalar_prod(Srt,deltat);     
    cout<<" la tracking error en 0 "<< this->profit_and_lost + pnl_vect_scalar_prod(Srt,deltat) - prix <<endl;
    pnl_vect_free(&deltat);
    pnl_vect_free(&Srt);

}

void Hedge::Maj(double t, const PnlMat* past_t){
    
    PnlVect * deltat = pnl_vect_create(this->mt_->opt_->getsize());
    PnlVect * Srt = pnl_vect_create(this->mt_->opt_->getsize());
    pnl_mat_get_row(Srt,this->past,getIndice(t));
    this->mt_->CalcDelta_t(past_t,t,deltat);
    int j =getIndice(t);
    pnl_mat_set_col(this->delta,deltat,j);
    PnlVect * deltat_moins_un =pnl_vect_create(this->mt_->opt_->getsize());
    pnl_mat_get_col(deltat_moins_un,this->delta,j-1);
    this->profit_and_lost*=exp(this->mt_->mod_->r_ * this->mt_->opt_->getMaturity()/this->NbtreRebalencement);
    PnlVect * tmp =pnl_vect_copy(deltat);
    pnl_vect_minus_vect(tmp,deltat_moins_un);
    this->profit_and_lost-=pnl_vect_scalar_prod(tmp,Srt);                
    pnl_vect_free(&tmp);
    pnl_vect_free(&deltat_moins_un);    
    pnl_vect_free(&Srt);
    pnl_vect_free(&deltat);
}


void Hedge::Majall(){
    
    double pasDetps= this->mt_->opt_->getMaturity()/this->NbtreRebalencement;
    double t=pasDetps;
    PnlMat * past_t;
    this->MajZero();
    for (int i =1;  i<this->NbtreRebalencement +1 ;i++){
        cout<<"i " << i<<endl;
        past_t=getHisto(t);
        Maj(t,past_t); 
        t+=pasDetps;              
    }
    
    PnlVect * Sfin = pnl_vect_create(this->mt_->opt_->getsize());
    pnl_mat_get_row(Sfin,this->past,this->NbtreRebalencement);
    PnlVect * deltafin = pnl_vect_create(this->mt_->opt_->getsize());
    pnl_mat_get_col(deltafin,this->delta,this->NbtreRebalencement);

    this->profit_and_lost+= pnl_vect_scalar_prod(deltafin,Sfin);
    double payoff = this->mt_->opt_->payoff(this->gethisto);
    this->profit_and_lost-=payoff;
    pnl_mat_free(&past_t);
    pnl_vect_free(&Sfin);
    pnl_vect_free(&deltafin);
    
    
}

PnlMat * Hedge::getHisto(double t){
  
    int size=this->mt_->opt_->getsize();
    int fraction =this->NbtreRebalencement/this->mt_->opt_->getnbTimeSteps();
    if (getIndice(t)%fraction==0){
        PnlVect * vect = pnl_vect_create(size);
        pnl_mat_get_row(vect,this->past,getIndice(t));
        PnlMat* transp = pnl_mat_transpose(this->gethisto);     
        pnl_mat_add_row(transp,transp->m,vect);       
        this->gethisto= pnl_mat_transpose(transp);
        pnl_vect_free(&vect);
        pnl_mat_free(&transp);      
        return this->gethisto;
    }
    else{
        
        PnlMat * past_t = pnl_mat_copy(this->gethisto);
        PnlVect * vect = pnl_vect_create(size);
        pnl_mat_get_row(vect,this->past,getIndice(t));
        PnlMat* transp = pnl_mat_transpose(past_t);
        pnl_mat_add_row(transp,transp->m,vect);
        past_t= pnl_mat_transpose(transp);
        pnl_vect_free(&vect);
        pnl_mat_free(&transp);
        return past_t;
    }
    
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




Hedge::~Hedge() {
    pnl_mat_free(&delta);
    pnl_mat_free(&past);
    pnl_mat_free(&gethisto);
    delete mt_;
    
    
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