#ifndef VARISWAP_HPP
#define VARISWAP_HPP

#include <vector>

using std::vector;

struct node {
    int strike; // strike price of call or put
    double price; // price of call or put
    
    node(int a, double b) { // constructor
        strike = a;
        price = b;
    }
};

class variSwap { // class declaration
private:
    double forwardPrice; // SPX forward price
    double spotPrice; // SPX spot price
    double r; // continuously compounded annualized risk free rate
    double T; // time to maturity
    vector<node> calls, puts; // data for calls puts
    
private:
    double f(double , double ); // payoff of log contract
    
public:
    variSwap();
    ~variSwap();
    double varianceStrike(); // return the fair value variance strike
    double varianceStrikeBetter(); // return the fair value variance strike using pair-wise approximation
};

#endif