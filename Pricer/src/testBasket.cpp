/* 
 * File:   testBasket.cpp
 * Author: benjellt
 *
 * Created on September 15, 2017, 10:43 AM
 */

#include <stdlib.h>
#include <iostream>
#include "BasketOption.hpp"
#include <iostream>
#include <string>
#include "parser.hpp"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <vector>
/*
 * Simple C++ Test Suite
 */

void test1() {
    std::cout << "testBasket test 1" << std::endl;
    BasketOption * basket = new BasketOption(10.0,4,4,1.0);
    
    std::vector<double> second (4,0.25);
    BasketOption *dernierTestBasket = new BasketOption(10.0,4,4,1.0,second);
    
    dernierTestBasket->toString();
    
    
    
    
    basket->setCoefficient(0.25,0);
    basket->setCoefficient(0.25,1);
    basket->setCoefficient(0.25,2);
    basket->setCoefficient(0.25,3);
    
    
    basket->toString();
    BasketOption * copie = new BasketOption(*basket);
    copie->toString();
 
    PnlMat* test = pnl_mat_create_from_scalar(4,4,4.0);
    double payoff = basket->payoff(test);
    pnl_mat_print(test);
    std::cout<< "payoff est " << payoff<<endl;
    
    
}

void test2() {
    std::cout << "testBasket test 2" << std::endl;
    std::cout << "%TEST_FAILED% time=0 testname=test2 (testBasket) message=error message sample" << std::endl;
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% testBasket" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test1 (testBasket)" << std::endl;
    test1();
    std::cout << "%TEST_FINISHED% time=0 test1 (testBasket)" << std::endl;

    std::cout << "%TEST_STARTED% test2 (testBasket)\n" << std::endl;
    test2();
    std::cout << "%TEST_FINISHED% time=0 test2 (testBasket)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}


