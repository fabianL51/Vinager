#pragma once // prevent errors when some libraries are called in another scripts
#include <math.h>
#include <string>
#include <iostream>


/* This header defines the class Bank and its properties */


class Bank{

    public:
        std::string Name;
        double Balance;
        std::string AssetType;
        std::string CodeName;
};