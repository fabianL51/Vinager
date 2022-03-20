#include "Banks.h" // for handling Banks classes
#include <xlnt/xlnt.hpp> // include xlnt for excel handling
#include "utils.h" // utilities functions


/* This script initializes banks and saves the data in an excel data */

int main(){
    
    /* This is the main function for bank initializaton */

    // initialize variables and objects
    xlnt::workbook BankWkb; // object to handle workbook using xlnt
    xlnt::worksheet BankAccountWks; // object to handle worksheet using xlnt
    std::string WorkbookName = "Banks.xlsx";
    std::string SheetName = "Accounts";

    // first step: checking excel containing banks' accounts information
    std::cout << "Accesing Bank informations";

    // Banks.xlsx already exists

    // display message to user
    std::cout << WorkbookName << " is found" << std::endl;
    std::cout << "Loading banks informations" << std::endl;

    // load Banks.xlsx using BankWkb
    BankWkb.load(WorkbookName);

    // set BankAccountWks to handle sheet Main
    BankAccountWks = BankWkb.sheet_by_title(SheetName);
    
    // initialize variables to add bank
    Bank bank;

     // inform user about current process
    std::cout << "Adding a bank... type 'stop' to end process" << std::endl;

    // ask the user in command window for the bank name
    std::cout << "Insert the bank/account name ";
    std::getline(std::cin, bank.Name);
    std::cout << std::endl;

    // store user input for bank class
    if (bank.Name != "stop"){

        // ask the user in command window for the starting money of the bank
        std::cout << "Insert the start amount of money for " << bank.Name << " : ";
        
        // checking if the input starting money is valid
        bool exit_loop = false; // variable exit_loop is bool in case user give invalid input
        while (exit_loop == 0){
            // begin loop as long as exit_loop is false
            if (std::cin >> bank.Balance){
                // if input value is valid then set exit_loop to true to exit loop
                exit_loop = 1;
            }
            if (exit_loop == 0) {
                // if input value invalid then inform the user
                std::cout << "Invalid amount of money, please insert a valid number for the start amount of money.";
                // clear the input
                std::cin.clear();
                // set exit_loop to zero
                exit_loop = 0;
                // empty loop and ask user to give a new value
                while (std::cin.get() != '\n') ;
            }
            std::cout << std::endl; // empty line for better display in command window

        }

        // ask the user in command window for the asset type of the bank account
        std::cout << "Insert current asset type " << bank.Name << std::endl;
        std::cout << "Insert 'L' for liquid asset and 'F' for fixed asset :" ;

        // checking if the input asset_type is valid
        exit_loop = false; // variable exit_loop is bool in case user give invalid input
        while (exit_loop == 0){
            // begin loop as long as exit_loop is false
            if (std::cin >> bank.AssetType){
                if (bank.AssetType == "L" or bank.AssetType == "F"){
                    // if input for asset_type is valid then set exit loop as true
                    exit_loop = 1;
                    // change asset type from abbreviations 
                    if (bank.AssetType == "L"){
                        bank.AssetType = "Liquid";
                    }
                    else if (bank.AssetType == "F"){
                        bank.AssetType = "Fixed";
                    }
                }
            }
            if (exit_loop == 0) {
                // if asset_type invalid then inform the user
                std::cout << "Invalid asset type... Please insert a valid asset type" << std::endl;
                std::cout << "Insert 'L' for liquid asset and 'F' for fixed asset :" ;
                // clear the input for asset_type
                std::cin.clear();
                // set exit_loop to zero
                exit_loop = 0;
                // empty loop and ask user to give a new value for asset_type
                while (std::cin.get() != '\n') ;
            }
            std::cout << std::endl; // empty line for better display in command window
        }

        // get last unempty row and inform the user
        int last_row = BankAccountWks.highest_row();

        // prepare cells for new bank informations
        // name cell for bank name in column A and one row after last_row 
        std::string name_cell = "A" + std::to_string(last_row + 1); 
        // balance cell for current bank's balance in column B and one row after last_row
        std::string balance_cell = "B" + std::to_string(last_row + 1); 
        // asset type cell for current bank's balance in column C and one row after last_row
        std::string asset_type_cell = "C" + std::to_string(last_row + 1); 
        
        // store bank data into worksheet
        BankAccountWks.cell(name_cell).value(bank.Name);
        BankAccountWks.cell(balance_cell).value(bank.Balance);
        BankAccountWks.cell(asset_type_cell).value(bank.AssetType);

        // save workbook
        BankWkb.save(WorkbookName);
    }
}