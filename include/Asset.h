#pragma once
#include <string>
#include <iostream>

/* This header defines an asset class for wealth allocation */

class Asset{

    public:  
        std::string Name;
        double Sum = 0;
        double PercentAllocation;
};