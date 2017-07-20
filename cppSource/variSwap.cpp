#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include "variSwap.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ifstream;
using std::ios;
using std::abs;

variSwap::variSwap() { // constructor
    double bidPrice, askPrice;
    char dataFile[100];
    
    cout << "input bid for SPX:" << endl;
    cin >> bidPrice;
    cout << "input ask for SPX:" << endl;
    cin >> askPrice;
    cout << "input continuously compounded annualized risk free rate:" << endl;
    cin >> r;
    cout << "input time to maturity(in years):" << endl;
    cin >> T;
    cout << "input data file name:" << endl;
    cin >> dataFile;
    
    spotPrice = (bidPrice + askPrice) / 2.0;
    
    forwardPrice = spotPrice * exp(r * T);
    
    cout << "forward price = " << forwardPrice << endl;
    cout << "spot price    = " << spotPrice << endl;
    
    ifstream inFile;
    inFile.open(dataFile, ios::in);
    
    while(!inFile.eof()) { // read strike prices according to dataFile data format
        int strike;
        double price;
        
        inFile >> strike >> price; if(inFile.eof()) break;
        
        calls.push_back(node(strike, price));
        
        inFile >> strike >> price;
        
        puts.push_back(node(strike, price));
    }
    
    inFile.close();
}

variSwap::~variSwap() { // destructor
    // empty
}

double variSwap::varianceStrike() { // return fair value variance strike
    
    int sStar;
    double minDistance;
    
    minDistance = abs(calls[0].strike - forwardPrice);
    sStar = calls[0].strike;
    
    for(int i=1;i<calls.size();i++) {
        if(abs(calls[i].strike - forwardPrice) < minDistance) {
            minDistance = abs(calls[i].strike - forwardPrice);
            
            sStar = calls[i].strike;
        }
    }
    
    //sStar, the value separates OTM calls and puts
        
    double dk = abs((double)calls[0].strike - (double)calls[1].strike);
    double sum=0.0;
    
    for(int i=0;i<puts.size();i++) {
        if(puts[i].strike <= sStar) {
            sum += puts[i].price / puts[i].strike / puts[i].strike;
        }
    }
    
    for(int i=0;i<calls.size();i++) {
        if(calls[i].strike >= sStar) {
            sum += calls[i].price / calls[i].strike / calls[i].strike;            
        }
    }
    
    sum *= dk * exp(r * T); // value of portfolio of options
        
    sum += r * T - (spotPrice / sStar * exp(r * T) - 1) - log(sStar / spotPrice);
    
    sum *= 2.0 / T;

    return sum;
}

double variSwap::varianceStrikeBetter() { // return the fair value variance strike using pair-wise approximation
    
    int sStar;
    double minDistance;
    
    minDistance = abs(calls[0].strike - forwardPrice);
    sStar = calls[0].strike;
    
    for(int i=1;i<calls.size();i++) {
        if(abs(calls[i].strike - forwardPrice) < minDistance) {
            minDistance = abs(calls[i].strike - forwardPrice);
            
            sStar = calls[i].strike;
        }
    }
    
    //sStar, the value separates OTM calls and puts
    
    double ac=0.0;
    double sum=0.0;
    
    for(int i=puts.size()-1;i>0;i--) {
        if(puts[i].strike <= sStar) {
            double weight;
            double k, knext;
            
            k = puts[i].strike;
            knext = puts[i-1].strike;
            
            weight = (f(knext, sStar) - f(k, sStar)) / (k - knext) - ac; // pair-wise approximation weight
        
            sum += weight * puts[i].price;
        
            ac += weight;
        }
    }
    
    ac = 0.0;
    
    for(int i=0;i<calls.size()-1;i++) {
        if(calls[i].strike >= sStar) {
            double weight;
            double k, knext;
            
            k = calls[i].strike;
            knext = calls[i+1].strike;
            
            weight = (f(knext, sStar) - f(k, sStar)) / (knext - k) - ac;
            
            sum += weight * calls[i].price;
            
            ac += weight;
        }
    }
    
    sum *= exp(r * T); // value of portfolio of options
    
    sum += r * T - (spotPrice / sStar * exp(r * T) - 1) - log(sStar / spotPrice);
    
    sum *= 2.0 / T;
    
    return sum;
}

double variSwap::f(double x, double star) { // payoff of log contract
    
    double res;
    
    res = (x - star) / star - log(x / star);
    
    return res;
}








