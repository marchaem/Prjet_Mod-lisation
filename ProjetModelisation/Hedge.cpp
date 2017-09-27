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

PnlMat * Hedge::getPast(){
    return this->past;
}
void Hedge::Maj(double t, const PnlMat* past_t){
    
    PnlVect * deltat = pnl_vect_create(this->mt_->opt_->getsize());
    PnlVect * Srt = pnl_vect_create(this->mt_->opt_->getsize());
    
    
    pnl_mat_get_row(Srt,this->past,getIndice(t));
    
    
    
    this->mt_->delta(past_t,t,deltat);
    
 
    int j =getIndice(t);
    pnl_mat_set_col(this->delta,deltat,j);
    if (t==0.0){
        double ic;
        double prix;
        this->mt_->price(prix,ic);
        this->profit_and_lost=prix;
        this->profit_and_lost-=pnl_vect_scalar_prod(Srt,deltat);
        cout<< "le prix en t =0 est : "<<prix<<endl;
        
        cout<< " au temps t=0 on met dans le sans risque : "<< this->profit_and_lost<<endl;
        cout<< " au temps 0 le Srt vaut : "<<endl;
        pnl_vect_print(Srt);
        cout<<" et le delta t vaut : "<< endl;
        pnl_vect_print(deltat);
        
        cout<<"  et donc au temps t=0 st * deltat : "<< pnl_vect_scalar_prod(Srt,deltat)<<endl;
        cout<<" la tracking error en 0 "<< this->profit_and_lost + pnl_vect_scalar_prod(Srt,deltat) - prix <<endl;
        
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
        double ic;
        double prix;
       
        
        this->mt_->price(past_t,t,prix,ic);
        cout<< "le prix en t  est : "<<prix<<endl;
        cout<< " au temps t on met dans le sans risque : "<< this->profit_and_lost<<endl;
        cout<< " au temps t le Srt vaut : "<<endl;
        pnl_vect_print(Srt);
        cout<<" et le delta t vaut : "<< endl;
        pnl_vect_print(deltat);
        cout<<"  et donc au temps t st * deltat : "<< pnl_vect_scalar_prod(Srt,deltat)<<endl;
        cout<<"tracking error au temps  " <<t << " est :"<< this->profit_and_lost +pnl_vect_scalar_prod(Srt,deltat)-prix<<endl;
    }
    pnl_vect_free(&Srt);
    pnl_vect_free(&deltat);
}


void Hedge::Majall(){
    double t=0.0;
    double pasDetps= this->mt_->opt_->getMaturity()/this->NbtreRebalencement;
    PnlMat * past_t;
    for (int i =0;  i<this->NbtreRebalencement +1 ;i++){
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
    

    //PnlMat * trajectoire= pnl_mat_create(this->mt_->opt_->getsize(),this->mt_->opt_->getnbTimeSteps()+1);
    //this->mt_->mod_->asset(trajectoire,this->mt_->opt_->getMaturity(),this->mt_->opt_->getnbTimeSteps(),this->mt_->rng_);
   
    
    
    //this->profit_and_lost-= this->mt_->opt_->payoff(trajectoire);
    double payoff = this->mt_->opt_->payoff(this->gethisto);
    this->profit_and_lost-=payoff;
    cout<<" à la fin la taille qu'on met dans past pour le payoff  est de : "<< this->gethisto->n<<endl << "et il vaut "<<payoff<<endl;;
    pnl_mat_free(&past);
    pnl_vect_free(&Sfin);
    pnl_vect_free(&deltafin);
    
    cout<<"inshallah il sort"<<endl;
}

PnlMat * Hedge::getHisto(double t){
  
    int size=this->mt_->opt_->getsize();
    int fraction =this->NbtreRebalencement/this->mt_->opt_->getnbTimeSteps();
    if (t==0.0){
       
        return this->gethisto;
    }
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
        
        
        
        
        
        
    
        
    /*
    PnlMat * past_t;
    int size=this->mt_->opt_->getsize();
    int fraction =this->NbtreRebalencement/this->mt_->opt_->getnbTimeSteps();
    if (t==0){
        past_t=pnl_mat_create(size,1);
        PnlVect * vect = pnl_vect_create(size);
        pnl_mat_get_row(vect,this->past,0);
        pnl_mat_set_col(past_t,vect,0);
        pnl_vect_free(&vect);
        return past_t; 
    }
    if (getIndice(t)%fraction==0){
        past_t=pnl_mat_create(size,getIndice(t)/fraction+1);
        int j=0;
        for (int i =0; i <getIndice(t)+1;i+=fraction){
            PnlVect * vect = pnl_vect_create(size);
            pnl_mat_get_row(vect,this->past,i);
            pnl_mat_set_col(past_t,vect,j);
            j++;
            pnl_vect_free(&vect);
        }
        return past_t;   
    }
    else{
        past_t=pnl_mat_create(size,getIndice(t)/fraction +2);
        int k =0;
        for (int i=0;  i<getIndice(t)+1 ;i+=fraction){
            PnlVect * vect = pnl_vect_create(size);
            pnl_mat_get_row(vect,this->past,i);
            pnl_mat_set_col(past_t,vect,k);
            k++;
            pnl_vect_free(&vect);
        }
        PnlVect * vect = pnl_vect_create(size);
        pnl_mat_get_row(vect,this->past,getIndice(t));
        pnl_mat_set_col(past_t,vect,past_t->n-1);
        
        pnl_vect_free(&vect);
        return past_t;*/
    
    
    //PnlMat past_t = pnl_mat_wrap_mat_rows(this->past,0,getIndice(t)); 
    
    
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