using namespace std;

#include "BlackScholesModel.hpp"
#include <exception>
#include <math.h>
#include <cmath>

BlackScholesModel::BlackScholesModel(Param *param) {
    param->extract("interest rate", this->r_);
    param->extract("option size", this->size_);
    param->extract("correlation", this->rho_);
    param->extract("volatility", this->sigma_, this->size_);
    param->extract("spot", this->spot_, this->size_);
}

void BlackScholesModel::asset(PnlMat* path, double T, int nbTimeSteps, PnlRng* rng) {

    /*Generation matrice de covariance + cholesky*/
    PnlMat* cov = pnl_mat_create_from_scalar(this->size_, this->size_, this->rho_);
    for (int i = 0; i<this->size_; i++) {
        pnl_mat_set_diag(cov, 1.0, i);
    }
    pnl_mat_chol(cov);

    /*Calcul du pas*/
    double pas = T / nbTimeSteps;

    /*Generation des vecteurs gaussiens*/
    PnlMat* matGi = pnl_mat_create(this->size_, nbTimeSteps);
    pnl_mat_rng_normal(matGi, this->size_, nbTimeSteps, rng);
    PnlVect *Gi = pnl_vect_create(this->size_);

    /*Simulation de la trajectoire*/
    double S0;
    double St;
    double sigmad;
    PnlVect* Ld = pnl_vect_create(this->size_);
    for (int d = 0; d<this->size_; d++) {
        S0 = pnl_vect_get(this->spot_, d);
        pnl_mat_set(path, d, 0, S0);
        sigmad = pnl_vect_get(this->sigma_, d);
        pnl_mat_get_row(Ld, cov, d);
        for (int i = 1; i <= nbTimeSteps; i++) {
            pnl_mat_get_col(Gi, matGi, i - 1);
            St = S0 * exp((this->r_ - pow(sigmad, 2) / 2) * pas + sigmad * sqrt(pas) * pnl_vect_scalar_prod(Ld, Gi));
            S0 = St;
            pnl_mat_set(path, d, i, St);
        }
    }

    /*Libération de la mémoire*/
    pnl_vect_free(&Ld);
    pnl_mat_free(&cov);
    pnl_mat_free(&matGi);
    pnl_vect_free(&Gi);

}

void BlackScholesModel::asset(PnlMat* path, double t, double T, int nbTimeSteps, PnlRng* rng, const PnlMat* past) {

    /*Generation matrice de covariance + cholesky*/
    PnlMat* cov = pnl_mat_create_from_scalar(this->size_, this->size_, this->rho_);
    for (int i = 0; i<this->size_; i++) {
        pnl_mat_set_diag(cov, 1.0, i);
    }
    pnl_mat_chol(cov);

    /*Generation des vecteurs gaussiens*/
    PnlMat* matGi = pnl_mat_create(this->size_, nbTimeSteps);
    pnl_mat_rng_normal(matGi, this->size_, nbTimeSteps, rng);
    PnlVect *Gi = pnl_vect_create(this->size_);

    /*Simulation de la trajectoire*/
    double S = 0.0;
    double St = 0.0;
    double sigmad;
    double pas = 0.0;
    PnlVect* Ld = pnl_vect_create(this->size_);
    for (int d = 0; d<this->size_; d++) {
        St = pnl_mat_get(past, d, (past->n) - 1);
        sigmad = pnl_vect_get(this->sigma_, d);
        pnl_mat_get_row(Ld, cov, d);
        for (int j = 0; j <= nbTimeSteps; j++) {
            pnl_mat_get_col(Gi, matGi, j - 1);
            if (j < (past->n) - 1) {
                pnl_mat_set(path, d, j, pnl_mat_get(past, d, j));
            } else {
                if (j == (past->n) - 1) {
                    pas = (j + 1) * T / nbTimeSteps - t;
                } else {
                    pas = T / nbTimeSteps;
                }
                S = St * exp((this->r_ - pow(sigmad, 2) / 2) * pas + sigmad * sqrt(pas) * pnl_vect_scalar_prod(Ld, Gi));
                St = S;
                pnl_mat_set(path, d, j, S);
            }
        }
    }

    /*Libération de la mémoire*/
    pnl_mat_free(&cov);
    pnl_mat_free(&matGi);
    pnl_vect_free(&Gi);
    pnl_vect_free(&Ld);
}

void BlackScholesModel::shiftAsset(PnlMat* shift_path, const PnlMat* path, int d, double h, double t, double timestep) {

    int indice_t = this->getPasTemps(t, timestep, path->n);
    pnl_mat_clone(shift_path, path);
    double coef;
    for (int j = indice_t + 1; j < path->n; j++) {
        coef = MGET(path, d, j);
        pnl_mat_set(shift_path, d, j, coef * (1 + h));
    }
}

int BlackScholesModel::getPasTemps(double t, double timestep, int nbTimeStep) {
    if (t == 0)
        return 0;
    double pasDeTps = timestep;
    int indiceCour = 0;
    double dist = 0;
    while (dist <= t) {
        dist += pasDeTps;
        indiceCour++;
    }
    return indiceCour - 1;
}

PnlMat* BlackScholesModel::simul_market() {

}



