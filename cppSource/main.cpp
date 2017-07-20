#include <iostream>
#include <cstdlib>
#include <cmath>
#include "variSwap.hpp"

using std::cout;
using std::endl;

int main(int argc, char* argv[]) {
    double strike=0.0;
    variSwap a;
    
    strike = a.varianceStrike(); // naive integration
    
    cout << endl << "by naive integration:" << endl;
    cout << "variance strike   = " << strike << endl;
    cout << "volatility strike = " << sqrt(strike) << endl;
    
    strike = a.varianceStrikeBetter(); // pair-wise approximation
    
    cout << endl << "by better pair-wise approximation:" << endl;
    cout << "variance strike   = " << strike << endl;
    cout << "volatility strike = " << sqrt(strike) << endl;
    
    return 0;
}
