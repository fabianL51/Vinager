#pragma once
#include <string>
#include <iostream>

/* This header defines an asset class for wealth allocation */

class Asset{

    public:
        Asset(){}; // initialize class

        // transfer input variables into class
        Asset(std::string type, double sum){
            _type = type; _sum = sum;
        }

        // get functions
        std::string get_asset_type(){return _type;};
        double get_asset_sum(){return _sum;};

        // set functions
        std::string set_asset_type(std::string type){return _type = type;};
        double set_asset_sum(double sum){return _sum = sum;};

    private:
        // initialize both properties in class
        double _sum = 0;
        std::string _type; 
};