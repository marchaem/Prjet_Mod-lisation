/* 
 * File:   Hedge.cpp
 * Author: benjellt
 * 
 * Created on September 21, 2017, 11:24 AM
 */

#include "Hedge.hpp"

Hedge::Hedge() {
    this->option_= new Option();
    this->delta= pnl_mat_create(this->option_.getsize(),this->option_.getnbTimeSteps());
}
Hedge::Hedge(Option option){
    this->option_=option;
    this->delta= pnl_mat_create(this->option_.getsize(),this->option_.getnbTimeSteps());
}

Hedge::Hedge(const Hedge& orig) {
}

Hedge::~Hedge() {
}
Option Hedge::getOption(){
     return this->option_;
}
 
void Hedge::setOption(Option option){
    this->option_=option;
}

PnlMat* Hedge::getdelta(){
    return this->delta;
}
void Hedge::setDelta(PnlMat* delta){
    this->delta=delta;
}