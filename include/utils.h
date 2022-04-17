#include <sys/stat.h> // to check file exists or not
#pragma once // prevent errors when some libraries are called in another scripts
#include <iostream>
#include <math.h>
#include <string>
#include <xlnt/xlnt.hpp> // include xlnt for excel handling
#include "Banks.h" // for handling Banks classes
#include "Asset.h" // for handling Asset classes
#include <cctype> // for uppercase

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
    // std::setlocale(LC_ALL, "de_DE");

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
            std::cout << AssetWks.cell( "J", i).formula() << std::endl;
                WealthClassVec.emplace_back(tempWealthClass);
        }
    }

    return std::make_pair(BankVec, WealthClassVec);
}

std::vector <Bank> get_account_codenames(std::vector <Bank> BankVec){

    // preallocate variables to derive codenames
    int middle_index;
    std::string TempCodeName;

    // iterate all members of bank vectors
    for(int i = 0; i < BankVec.size(); i ++){

        // clear temporary codenames storage
        TempCodeName.clear();
        // get the index for middle character
        middle_index = std::round(BankVec.at(i).Name.length() / 2);
        // build codename
        // first character
        TempCodeName.push_back(BankVec.at(i).Name[0]);
        // middle character
        TempCodeName.push_back(toupper(BankVec.at(i).Name[middle_index]));
        // last character
        TempCodeName.push_back(toupper(BankVec.at(i).Name[BankVec.at(i).Name.length()-1]));
        // current index
        TempCodeName.append(std::to_string(i + 1));
        // store codename at bank vector 
        BankVec.at(i).CodeName = TempCodeName;
    }

    return BankVec;
}

bool isValidCodename(std::vector <Bank> BankVec, std::string InputCodeName){

    bool valid_codename = false;
    // iterate through bank vector
    for (auto Bank: BankVec){
        if (Bank.CodeName == InputCodeName){
            valid_codename = true;
            break;
        }
    }

    return valid_codename;
}