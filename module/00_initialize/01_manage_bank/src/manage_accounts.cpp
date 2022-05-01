#include "Account.h" // for handling Banks classes
#include "GlobalData.h" // to get global data such as csv names
#include "utils.h" // utilities functions
#include <fstream> // to handle csv data
#include <map> // to handle map
#include <sstream> // to convert char to string
#include <cctype> // to uppercase letters



/* This script manages accounts and save in csv data */

int main(){
    
    /* This is the main function for bank initializaton */

    // open or create csv file containing accounts data
    std::fstream accounts_csv_file;

    // check if csv file containing accounts data exist
    if (file_exists(GlobalData::FileNames::accounts_csv)){
        /* manage bank */
    }
    else {
        /* initialize bank */

        // creates the csv file
        accounts_csv_file.open(GlobalData::FileNames::accounts_csv, std::ios::out | std::ios::app);
        
        std::cout << "-------------Initializing Accounts--------------" << std::endl;

        // get number of accounts to be initialized
        int n_acc;

        // checking if the input starting money is valid
        bool exit_loop = false; // variable exit_loop is bool in case user give invalid input
        std::cout << "Set the number of accounts to be initialized ";
        while (exit_loop == 0){
            // begin loop as long as exit_loop is false
            if (std::cin >> n_acc){
                // n_wealth must be a real number to exit loop
                exit_loop = n_acc > 0;
            }
            if (exit_loop == 0) {
                // if n_wealth invalid then inform the user
                std::cout << "Invalid number of accounts.";
            }
            // clear the input
            std::cin.clear();
            while (std::cin.get() != '\n') ;
            std::cout << std::endl; // empty line for better display in command window
        }

        // initialize storage variables
        std::vector <Account> accounts_vector; // vector to store accounts information
        Account tempAccount; // temporary storage of account information
        double SumLiquid = 0; // total sum for each liquid asset
        double SumFixed = 0; // total sum for each fixed asset
        int middle_index; // integer to get middle index of account name for codenames

        for (int i = 1; i <= n_acc; i++){
            // get the account name
            std::cout << "Insert the name for the " << i << ordinal_suffix(i) << " account ";
            std::getline(std::cin, tempAccount.Name);
            std::cout << std::endl;

            // get account codename
            middle_index = std::round(tempAccount.Name.length() / 2);
            // first character
            tempAccount.CodeName.push_back(tempAccount.Name[0]);
            // middle character
            tempAccount.CodeName.push_back(toupper(tempAccount.Name[middle_index]));
            // last character
            tempAccount.CodeName.push_back(toupper(tempAccount.Name[tempAccount.Name.length() - 1]));

            // get the account balance
            exit_loop = false; // variable exit_loop is bool in case user give invalid input
            while (exit_loop == false){
                // ask the user in command window for the starting money of the bank
                std::cout << "Insert the start amount of money for " << tempAccount.Name << " : ";
                // begin loop as long as exit_loop is false
                if (std::cin >> tempAccount.Balance){
                    // if input value is valid then set exit_loop to true to exit loop
                    exit_loop = tempAccount.Balance >= 0;
                }
                if (exit_loop == 0) {
                    // if input value invalid then inform the user
                    std::cout << "Invalid amount of money.";               
                }
                // clear the input
                std::cin.clear();
                while (std::cin.get() != '\n') ;
                std::cout << std::endl; // empty line for better display in command window
            }

            // get the account asset type
            exit_loop = false; // variable exit_loop is bool in case user give invalid input
            while (exit_loop == false){
                // ask the user in command window for the asset type of the bank account
                std::cout << "Insert current asset type " << tempAccount.Name << std::endl << "Insert 'L' for liquid asset and 'F' for fixed asset :" << std::endl;
                // get input
                std::getline(std::cin, tempAccount.AssetType);
                // if input value is valid then set exit_loop to true to exit loop
                exit_loop = tempAccount.AssetType == "L" or tempAccount.AssetType == "F";
                // define asset type if input is valid
                if (tempAccount.AssetType == "L"){
                    tempAccount.AssetType = "Liquid";
                    SumLiquid += tempAccount.Balance;
                }
                else if (tempAccount.AssetType == "F"){
                    tempAccount.AssetType = "Fixed";
                    SumFixed += tempAccount.Balance;
                }
                // warning if input is invalid
                if (exit_loop == false){
                    std::cout << "Invalid asset type." << std::endl;
                    }
                }

            // add tempAccount to accounts vector
            accounts_vector.emplace_back(tempAccount);

            // write to csv
            char delimiter = ',';
            accounts_csv_file << tempAccount.Name << delimiter << tempAccount.CodeName << delimiter << tempAccount.AssetType << 
                delimiter << tempAccount.Balance << "\n"; 

            // clear tempAccount for next account
            tempAccount.clear();
        }

        // close csv
        accounts_csv_file.close();

    }
   
    
}