#include "Account.h" // for handling Banks classes
#include "GlobalData.h" // to get global data such as csv names
#include "utils.h" // utilities functions
#include <fstream> // to handle csv data
#include <map> // to handle map
#include <cctype> // to uppercase letters
#include <typeinfo>



/* This script manages accounts and save in csv data */

int main(){
    
    /* This is the main function for bank initializaton */

    // initialize variables
    std::fstream accounts_csv_file; // fstream to read or write csv file
    bool init_accounts = !file_exists(GlobalData::FileNames::accounts_csv); // boolean whether accounts are to be initialized
    char delimiter = GlobalData::csv_config::delimiter;

    // check if csv file containing accounts data exist
    if (file_exists(GlobalData::FileNames::accounts_csv)){
        std::cout << "Initialized Accounts are found" << std::endl;
        // ask user whether he wants reset accounts or not
        int reset;
        std::cout << "Press 1 to reset accounts and any other keys to manage created accounts ";
        std::cin >> reset;
        // reset account if user input key 1, otherwise manage accounts
        init_accounts = reset == 1;
    }

    if (init_accounts == true) {
        /* initialize account */

        // creates the csv file
        accounts_csv_file.open(GlobalData::FileNames::accounts_csv, std::ios::out);
        
        std::cout << "-------------Initializing Accounts-------------" << std::endl;

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
            // current index
            tempAccount.CodeName.append(std::to_string(i));

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

            // write to csv: start balance is equal to balance
            accounts_csv_file << tempAccount.Name << delimiter << tempAccount.CodeName << delimiter << tempAccount.AssetType << 
                delimiter << tempAccount.Balance << delimiter << tempAccount.Balance << "\n"; 

            // clear tempAccount for next account
            tempAccount.clear();
        }

        // close csv
        accounts_csv_file.close();

        // save asset type
        std::fstream asset_type_csv_file;
        asset_type_csv_file.open(GlobalData::FileNames::asset_type_csv, std::ios::out);
        asset_type_csv_file << "Liquid" << delimiter << SumLiquid << "\n";
        asset_type_csv_file << "Fixed" << delimiter << SumFixed << "\n";
        asset_type_csv_file.close();
    }
    else if(init_accounts == false) {
        /* manage initialized accounts */

        // get all accounts from csv file
        std::vector <Account> accounts_vector = get_accounts_vector();

        // display all accounts in command line
        for (auto tempAccount:accounts_vector){
            std::cout << tempAccount.Name << " (" << tempAccount.CodeName << ") - " << tempAccount.AssetType
                << " :" << tempAccount.Balance << std::endl;
        }

        /* IMPLEMENT CODES TO MANAGE ACCOUNTS HERE */

        // stop to view
        int a;
        std::cin >> a;

        


    }
   
    
}