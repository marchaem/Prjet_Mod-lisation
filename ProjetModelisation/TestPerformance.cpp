/* 
 * File:   TestPerformance.cpp
 * Author: benjellt
 *
 * Created on September 19, 2017, 8:57 AM
 */

#include <stdlib.h>
#include <iostream>
#include "PerformanceOption.hpp"
/*
 * Simple C++ Test Suite
 */

void test1() {
    std::cout << "TestPerformance test 1" << std::endl;
    std::vector<double> second (2,0.5);
    PerformanceOption * perform  = new PerformanceOption(10.0,4,2,second);
    
    perform->toString();
    
    
    
    
    
    
 
    PnlMat* test = pnl_mat_create_from_scalar(5,2,9.0);
    MLET(test,2,1)=4.0;
    MLET(test,0,0)=3.0;
    double payoff = perform->payoff(test);
    
    pnl_mat_print(test);
    std::cout<< "payoff est " << payoff<<endl;
}

void test2() {
    std::cout << "TestPerformance test 2" << std::endl;
    std::cout << "%TEST_FAILED% time=0 testname=test2 (TestPerformance) message=error message sample" << std::endl;
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% TestPerformance" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test1 (TestPerformance)" << std::endl;
    test1();
    std::cout << "%TEST_FINISHED% time=0 test1 (TestPerformance)" << std::endl;

    std::cout << "%TEST_STARTED% test2 (TestPerformance)\n" << std::endl;
    test2();
    std::cout << "%TEST_FINISHED% time=0 test2 (TestPerformance)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

