#include <sys/stat.h> // to check file exists or not
#pragma once // prevent errors when some libraries are called in another scripts
#include "Account.h" // for handling Banks classes
#include "WealthClass.h" // for handling Asset classes
#include <fstream> // to handle csv data
#include <sstream> // to read string in csv
#include <iostream> // basic
#include <math.h> // for math and vector
#include <string> // string handles
#include <xlnt/xlnt.hpp> // include xlnt for excel handling
#include <map> // for mapping

inline bool file_exists (const std::string& name) {
    /* This functions check if a file exists 
    source: https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exists-using-standard-c-c11-14-17-c
    answer by PherricOxide and IInspectable    
    */
    struct stat buffer;   
    return (stat (name.c_str(), &buffer) == 0); 
}

std::string ordinal_suffix(int n)
{
    /* this functions returns the ordinal suffix of a real number n
    for example: 1 = 1st, 2 = 2nd, 3 = 3rd, 13 = 13th 
    source: https://stackoverflow.com/questions/21548099/function-for-getting-the-ordinal-of-a-number
    answer by einpoklum   
    */
        std::vector <std::string> suffixes  = {"th", "st", "nd", "rd"};
        auto ord = n % 100;
        if (ord / 10 == 1) { ord = 0; }
        ord = ord % 10;
        if (ord > 3) { ord = 0; }
        return suffixes.at(ord);
}

std::vector <Account> get_accounts_vector(){

    /* this function read csv for accounts and return a vector of Account class */


    // initialize variables
    std::fstream account_csv_fstream;
    std::string line, word; // strings to get a whole line or a word in a line
    std::vector <Account> Account_vector; // account vector to be returned
    Account tempAccount; // temporary account to be added into vector
    char delimiter = GlobalData::csv_config::delimiter; // set delimiter in csv

    // open csv
    account_csv_fstream.open(GlobalData::FileNames::accounts_csv, std::ios::in);

    if (!account_csv_fstream.is_open()) {std::cout << "file can't be opened!";};

    // read csv
    while (std::getline(account_csv_fstream, line)){
        
        // get stringstream from line
        std::stringstream ss(line);

        // set index for storing the right information into right data
        int index = 1;
        
        // store the value into tempAccount
        while (std::getline(ss, word, ',')){
            switch (index)
            {
            case 1: // index 1 = account name
                tempAccount.Name = word; 
                break;

            case 2: // index 2 = codenames
                tempAccount.CodeName = word; 
                break;

            case 3: // index 3 = asset type
                tempAccount.AssetType = word; 
                break;

            case 4: // index 4 = balance
                tempAccount.Balance = std::stod(word); 
                break;
            }

            // add index by one
            index += 1;
        }
        
        // expand account vector
        Account_vector.emplace_back(tempAccount);

        // clear tempAccount
        tempAccount.clear();
    }

    // close csv
    account_csv_fstream.close();

    // return account vector
    return Account_vector;
}


std::vector <WealthClass> get_wealth_classes_vector(){

    /* this function read csv for wealth classes and return a vector of WealthClass class */


    // initialize variables
    std::fstream wealth_class_csv_fstream;
    std::string line, word; // strings to get a whole line or a word in a line
    std::vector <WealthClass> wealth_classes_vector; // account vector to be returned
    WealthClass tempWealthClass; // temporary account to be added into vector
    char delimiter = GlobalData::csv_config::delimiter; // set delimiter in csv

    // open csv
    wealth_class_csv_fstream.open(GlobalData::FileNames::wealth_classes_csv, std::ios::in);

    if (!wealth_class_csv_fstream.is_open()) {std::cout << "file can't be opened!";};

    // read csv
    while (std::getline(wealth_class_csv_fstream, line)){
        
        // get stringstream from line
        std::stringstream ss(line);

        // set index for storing the right information into right data
        int index = 1;
        
        // store the value into tempWealthClass
        while (std::getline(ss, word, ',')){
            switch (index)
            {
            case 1: // index 1 = wealth class name
                tempWealthClass.Name = word; 
                break;

            case 2: // index 2 = allocation from asset type in percent
                tempWealthClass.PercentAllocation = std::stod(word); 
                break;

            case 3: // index 3 = wealth class sum
                tempWealthClass.Sum = std::stod(word); 
                break;
            }

            // add index by one
            index += 1;
        }
        
        // expand account vector
        wealth_classes_vector.emplace_back(tempWealthClass);

        // clear tempAccount
        tempWealthClass.clear();
    }

    // close csv
    wealth_class_csv_fstream.close();

    // return account vector
    return wealth_classes_vector;
}

std::vector <double> get_assets(){

    /* this function read csv for asset type and return their sum in a vector */


    // initialize variables
    std::fstream asset_type_csv_fstream;
    std::string line, word; // strings to get a whole line or a word in a line
    std::vector <double> asset_sum_vector; // account vector to be returned
    char delimiter = GlobalData::csv_config::delimiter; // set delimiter in csv

    // open csv
    asset_type_csv_fstream.open(GlobalData::FileNames::asset_type_csv, std::ios::in);

    if (!asset_type_csv_fstream.is_open()) {std::cout << "file can't be opened!";};

    // read csv
    while (std::getline(asset_type_csv_fstream, line)){
        
        // get stringstream from line
        std::stringstream ss(line);

        // set index for storing the right information into right data
        int index = 1;
        
        // store the value into tempWealthClass
        while (std::getline(ss, word, ',')){
            
            // get asset type sum: 0 = Liquid, 1 = Fixed
            asset_sum_vector.at(index - 1) = std::stod(word);

            // add index by one
            index += 1;
        }
    }

    // close csv
    asset_type_csv_fstream
    // return account vector
    return asset_sum_vector;
}


xlnt::border create_data_border (){

    /* This function creates border for data type */
    
    xlnt::border border;
    xlnt::border::border_property border_prop;
    border_prop.style(xlnt::border_style::thin);
    border.side(xlnt::border_side::start, border_prop);
    border.side(xlnt::border_side::end, border_prop);
    border.side(xlnt::border_side::top, border_prop);
    border.side(xlnt::border_side::bottom, border_prop);

    return border;
}

std::pair <std::vector <Account>, std::vector <Asset>> get_accounts_wealth(xlnt::worksheet AssetWks, int Acc_lr, int Wea_lr){

    /* This function gets the accounts and wealth classes through iterating in Asset sheets */

    std::vector <Account> BankVec;
    std::vector <Asset> WealthClassVec;
    Account tempBank;
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
            WealthClassVec.emplace_back(tempWealthClass);
        }
    }

    return std::make_pair(BankVec, WealthClassVec);
}

std::vector <Account> get_account_codenames(std::vector <Account> BankVec){

    /* This function creates codenames for each account */

    // preallocate variables to derive codenames
    int middle_index; // middle index to store middle character
    std::string TempCodeName; // temporary variables to store codename

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

std::pair <std::map <std::string, std::string>, std::map <std::string, std::string>> get_acc_name_codenames_map(std::vector <Account> BankVec){

    /* This function creates two maps of account names and their codenames */

    std::map <std::string, std::string> name_codename_map, codename_name_map;
    for (auto Bank: BankVec){
        name_codename_map[Bank.Name] = Bank.CodeName;
        codename_name_map[Bank.CodeName] = Bank.Name;
    }

    return std::make_pair(name_codename_map, codename_name_map);

}

bool isValidCodename(std::vector <Account> BankVec, std::string InputCodeName){

    /* This function checks whether the codename input from user is valid */

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