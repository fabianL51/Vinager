#pragma once // prevent errors when some libraries are called in another scripts
#include <math.h>
#include <string>
#include <iostream>


/* This header defines the class Bank and its properties */


class Account{

    public:
        void clear(){
            Name.clear();
            AssetType.clear();
            CodeName.clear();
            Balance = 0;
        }

        std::string Name;
        double Balance;
        std::string AssetType;
        std::string CodeName;
};