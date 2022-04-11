#include <sys/stat.h> // to check file exists or not
#pragma once // prevent errors when some libraries are called in another scripts
#include <iostream>
#include <math.h>
#include <string>
#include <xlnt/xlnt.hpp> // include xlnt for excel handling
#include "Banks.h" // for handling Banks classes
#include "Asset.h" // for handling Asset classes

inline bool file_exists (const std::string& name) {
    /* This functions check if a file exists */
    struct stat buffer;   
    return (stat (name.c_str(), &buffer) == 0); 
}

xlnt::border create_data_border (){
    
    xlnt::border border;
    xlnt::border::border_property border_prop;
    border_prop.style(xlnt::border_style::thin);
    border.side(xlnt::border_side::start, border_prop);
    border.side(xlnt::border_side::end, border_prop);
    border.side(xlnt::border_side::top, border_prop);
    border.side(xlnt::border_side::bottom, border_prop);

    return border;
}

std::pair <std::vector <Bank>, std::vector <Asset>> get_accounts_wealth(xlnt::worksheet AssetWks, int Acc_lr, int Wea_lr){

    std::vector <Bank> BankVec;
    std::vector <Asset> WealthClassVec;
    Bank tempBank;
    Asset tempWealthClass;

    for (int i = 3; i <= std::max(Acc_lr, Wea_lr); i++){
        
        // reinitialize temporary variables
        tempBank, tempWealthClass;

        // get bank data by checking if row is not the last row of account
        if (i <= Acc_lr){
            tempBank.Name = AssetWks.cell( "A", i).value<std::string>(); // col A: bank name
            tempBank.AssetType = AssetWks.cell( "B", i).value<std::string>(); // col B: bank asset type
            tempBank.Balance = AssetWks.cell( "D", i).value<double>(); // col D: current balance
            BankVec.emplace_back(tempBank);
        }
     
        // get wealth class data by checking row is not the last row of wealth
        if (i <= Wea_lr){
            tempWealthClass.Name = AssetWks.cell( "G", i).value<std::string>(); // col G: wealth class name
            tempWealthClass.PercentAllocation = AssetWks.cell("H",i).value<double>(); // col H: percentage allocation
            tempWealthClass.Sum = AssetWks.cell( "J", i).value<double>(); // col J: current balance
            WealthClassVec.emplace_back(tempWealthClass);
        }
    }

    return std::make_pair(BankVec, WealthClassVec);
}