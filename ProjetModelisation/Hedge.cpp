/* 
 * File:   Hedge.cpp
 * Author: benjellt
 * 
 * Created on September 21, 2017, 11:24 AM
 */

#include "Hedge.hpp"
#include "MonteCarlo.hpp"
using namespace std;

Hedge::Hedge(Param* P, char * fichier) {
    
    this->mt_ = new MonteCarlo(P);
    P->extract("hedging dates number", this->NbtreRebalencement);
    this->delta = pnl_mat_create(this->mt_->opt_->getsize(), this->NbtreRebalencement + 1);
    this->past = pnl_mat_create_from_file(fichier);
    this->profit_and_loss = 0.0;
    PnlMat * past_t = pnl_mat_create(this->mt_->opt_->getsize(), 1);
    pnl_mat_set_col(past_t, this->mt_->mod_->spot_, 0);
    this->gethisto = past_t;
}

Hedge::Hedge(Param* P) {

    this->mt_ = new MonteCarlo(P);
    P->extract("hedging dates number", this->NbtreRebalencement);
    this->delta = pnl_mat_create(this->mt_->opt_->getsize(), this->NbtreRebalencement + 1);
    this->profit_and_loss = 0.0;
    PnlMat * past_t = pnl_mat_create(this->mt_->opt_->getsize(), 1);
    pnl_mat_set_col(past_t, this->mt_->mod_->spot_, 0);
    this->gethisto = past_t;

    PnlMat * path = pnl_mat_create(this->mt_->opt_->getsize(), this->NbtreRebalencement + 1);
    this->mt_->mod_->simul_market(path, this->mt_->opt_->getMaturity(), this->NbtreRebalencement, this->mt_->rng_);
    this->past = pnl_mat_transpose(path);
}

PnlMat * Hedge::getPast() {
    return this->past;
}

MonteCarlo * Hedge::getMt() {
    return this->mt_;
}

int Hedge::Getnbtreb() {
    return this->NbtreRebalencement;
}

PnlMat * Hedge::GetGethisto() {
    return this->gethisto;
}

void Hedge::MajZero() {

    PnlVect * delta0 = pnl_vect_create(this->mt_->opt_->getsize());
    double ic, prix;

    /*Calcul du prix et delta en zero*/
    this->mt_->calcDelta0(delta0);
    this->mt_->price(prix, ic);
    pnl_mat_set_col(this->delta, delta0, 0);

    /*Calcul de V0*/
    this->profit_and_loss = prix;
    this->profit_and_loss -= pnl_vect_scalar_prod(this->mt_->mod_->spot_, delta0);

}

void Hedge::Maj(double t, const PnlMat* past_t) {
    
    PnlVect * delta_t = pnl_vect_create(this->mt_->opt_->getsize());
    PnlVect * St = pnl_vect_create(this->mt_->opt_->getsize());

    /*Calcul et stockage du nouveau delta*/
    pnl_mat_get_row(St, this->past, getIndice(t));
    this->mt_->CalcDelta_t(past_t, t, delta_t);
    int j = getIndice(t);
    pnl_mat_set_col(this->delta, delta_t, j);
    
    /*Calcul du prix (pour test)*/
    /*double prix, ic;
    this->mt_->price(past_t, t, prix, ic);
    std::cout << "Prix au temps " << t << " = " << prix << std::endl;*/

    /*On va chercher le delta précédent*/
    PnlVect * deltat_moins_un = pnl_vect_create(this->mt_->opt_->getsize());
    pnl_mat_get_col(deltat_moins_un, this->delta, j - 1);

    /*Calcul de la différence des deltas*/
    PnlVect * diffDeltas = pnl_vect_copy(delta_t);
    pnl_vect_minus_vect(diffDeltas, deltat_moins_un);

    /*Actualisation de Vi*/
    this->profit_and_loss *= exp(this->mt_->mod_->r_ * this->mt_->opt_->getMaturity() / this->NbtreRebalencement);
    this->profit_and_loss -= pnl_vect_scalar_prod(diffDeltas, St);

    /*Libération de la mémoire*/
    pnl_vect_free(&diffDeltas);
    pnl_vect_free(&deltat_moins_un);
    pnl_vect_free(&St);
    pnl_vect_free(&delta_t);
}

void Hedge::Majall() {

    PnlVect * Sfin = pnl_vect_create(this->mt_->opt_->getsize());
    PnlVect * deltafin = pnl_vect_create(this->mt_->opt_->getsize());
    double pasDetps = this->mt_->opt_->getMaturity() / this->NbtreRebalencement;
    double t = pasDetps;
    PnlMat * past_t;

    /*Rebalancement à toutes les dates*/
    this->MajZero();
    for (int i = 1; i<this->NbtreRebalencement + 1; i++) {
        past_t = getHisto(t);
        Maj(t, past_t);
        t += pasDetps;
    }

    /*On va chercher le dernier delta et dernier prix*/
    pnl_mat_get_row(Sfin, this->past, this->NbtreRebalencement);
    pnl_mat_get_col(deltafin, this->delta, this->NbtreRebalencement);

    /*Calcul du P&L*/
    double payoff = this->mt_->opt_->payoff(this->gethisto);
    this->profit_and_loss += pnl_vect_scalar_prod(deltafin, Sfin);
    this->profit_and_loss -= payoff;

    /*Libération de la mémoire*/
    pnl_mat_free(&past_t);
    pnl_vect_free(&Sfin);
    pnl_vect_free(&deltafin);


}

PnlMat * Hedge::getHisto(double t) {

    /*Rapport entre le nb de dates de constat et de rebelancement*/
    int fraction = this->NbtreRebalencement / this->mt_->opt_->getnbTimeSteps();

    PnlVect * vect = pnl_vect_create(this->mt_->opt_->getsize());

    if (getIndice(t) % fraction == 0) {

        pnl_mat_get_row(vect, this->past, getIndice(t));
        PnlMat* transp = pnl_mat_transpose(this->gethisto);
        pnl_mat_add_row(transp, transp->m, vect);
        this->gethisto = pnl_mat_transpose(transp);
        pnl_mat_free(&transp);
        return this->gethisto;

    } else {

        PnlMat * past_t = pnl_mat_copy(this->gethisto);
        pnl_mat_get_row(vect, this->past, getIndice(t));
        PnlMat* transp = pnl_mat_transpose(past_t);
        pnl_mat_add_row(transp, transp->m, vect);
        past_t = pnl_mat_transpose(transp);
        pnl_mat_free(&transp);
        return past_t;
    }

    pnl_vect_free(&vect);

}

int Hedge::getIndice(double t) {
    if (t == 0)
        return 0;
    double pasDeTps = this->mt_->opt_->getMaturity() / this->NbtreRebalencement;

    int indiceCour = 0;
    double dist = 0.0;
    while (dist <= t) {
        dist += pasDeTps;
        indiceCour++;
    }
    return indiceCour - 1;
}

Hedge::~Hedge() {
    
    pnl_mat_free(&delta);
    pnl_mat_free(&past);
    pnl_mat_free(&gethisto);
    delete mt_;
    
}

PnlMat* Hedge::getdelta() {
    return this->delta;
}

void Hedge::setDelta(PnlMat* delta) {
    this->delta = delta;
}

double Hedge::getPandL() {
    return this->profit_and_loss;
}
