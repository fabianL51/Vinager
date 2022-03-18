#pragma once // prevent errors when some libraries are called in another scripts
#include <math.h>
#include <string>
#include <iostream>


/* This header defines the class Bank and its properties */


class Bank{

    public:

        Bank(){}; // initialize class

        // transfer input variables into properties in class
        Bank(std::string Name, double MoneyAmount){
            _Name = Name; _MoneyAmount = MoneyAmount; 
        }

        // get functions to get values from bank's properties
        std::string get_Bank_name() {return _Name;};
        double get_Bank_money() {return _MoneyAmount;};

        // set functions to set values of bank's properties
        std::string set_Bank_name(std::string Name) {return _Name = Name;};
        double set_Bank_initial_money(double StartAmount) {return _MoneyAmount = StartAmount;};

    private:
        // initialize both properties in class
        std::string _Name;
        double _MoneyAmount;
};