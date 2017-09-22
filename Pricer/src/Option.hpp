#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <iostream>
#include <vector>
#include "parser.hpp"
using namespace std;

/// \brief Classe Option abstraite
class Option
{
public:
    
    /**
     * Calcule la valeur du payoff sur la trajectoire
     *
     * @param[in] path est une matrice de taille (N+1) x d
     * contenant une trajectoire du modèle telle que créée
     * par la fonction asset.
     * @return phi(trajectoire)
     */
    virtual double payoff(const PnlMat *path) = 0;
    Option(); // constructeur sans param
    Option(double T, int nbTImeSteps, int size);
    Option(double T, int nbTImeSteps, int size,std::vector <double> coefficient); // constructeur  
    Option(Option const& autre); // constructeur par copie
    ~Option();
    Option(Param *P);
    double getMaturity();
    void setnbTimeSteps(int n);
    void setMaturity(float T);
    int getnbTimeSteps();
    void setMaturity(double d);
    int getsize();
    void setsize(int size);
    double getCoefficient(int i);
    void setCoefficient(double d, int i);
    
    
    
    
    
    
    
protected: 
    double Maturity_; /// maturité
    int nbTimeSteps_; /// nombre de pas de temps de discrétisation
    int size_; /// dimension du modèle, redondant avec BlackScholesModel::size_
    std::vector <double> coefficient_; // vecteur de coefficients
};


