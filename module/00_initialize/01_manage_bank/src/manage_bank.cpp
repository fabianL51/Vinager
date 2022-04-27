#include "Account.h" // for handling Banks classes
#include "utils.h" // utilities functions
#include <fstream> // to handle csv data
#include <map> // to handle map
#include <sstream> // to convert char to string
#include "GlobalData.h"



/* This script manages accounts and save in csv data */

int main(){
    
    /* This is the main function for bank initializaton */

    // try to open csv file containing accounts data exist
    std::ifstream accounts_csv_file(GlobalData::FileNames::accounts_csv);

    // check if csv file containing accounts data exist
    if (accounts_csv_file.is_open()){
        /* manage bank */
    }
    else {
        /* initialize bank */
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

        for (int i = 1; i <= n_acc; i++){
            // get the account name
            std::cout << "Insert the account name ";
            std::getline(std::cin, tempAccount.Name);
            std::cout << std::endl;

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

            // clear tempAccount for next account
            tempAccount.clear();
    }

    // store accounts and asset type values in their corresponding csv files
    
    
}