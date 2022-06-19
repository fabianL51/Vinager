#pragma once // prevent errors when some libraries are called in another scripts
#include <math.h>
#include <string>
#include <iostream>


/* This header defines the class Bank and its properties */


class RecTrans{

    public:
        void clear(){
            Type.clear();
            Name.clear();
            Category.clear();
            AmountMoney = 0;
            PayAccount.clear();
            TargetAccount.clear();
            DueDate = 1250;
            Status.clear();
        }

        std::string Type;
        std::string Name;
        std::string Category;
        double AmountMoney;
        std::string PayAccount;
        std::string TargetAccount;
        std::string Status;
        int DueDate;
};