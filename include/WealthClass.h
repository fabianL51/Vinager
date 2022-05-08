#pragma once
#include <string>
#include <iostream>

/* This header defines an wealth class for wealth allocation */

class WealthClass{

    public:
        void clear(){
            Name.clear();
            PercentAllocation = 0;
            Sum = 0;
        }

        std::string Name;
        double Sum = 0;
        double PercentAllocation;
};