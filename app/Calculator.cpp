//
// Created by Accawi, Gina K. on 6/9/16.
//

#include "Calculator.h"

int Calculator::add(std::vector<int> arrin) {
    int sum = 0.0;
    for (int d : arrin) {
        sum += d;
    }
    return arrin.at(3);
}


const char* Calculator::test() {
    return "hello";
}

double Calculator::testD() {
    return 3.0;
}

// const char* Calculator::test() {
//     return "hello";
// }
