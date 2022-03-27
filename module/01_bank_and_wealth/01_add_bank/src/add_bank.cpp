#include "Banks.h" // for handling Banks classes
#include <xlnt/xlnt.hpp> // include xlnt for excel handling
#include "utils.h" // utilities functions


/* This script initializes banks and saves the data in an excel data */

int main(){
    
    /* This is the main function for bank initializaton */

    // initialize variables and objects
    xlnt::workbook FinCordsWkb, UtilitiesWkb; // object to handle workbook using xlnt
    xlnt::worksheet AssetsWks, UtilitesWks; // object to handle worksheet using xlnt
    std::string FinCordsWkbName = std::to_string(xlnt::date::today().year) + "_FinancialRecords.xlsx";
    std::string SheetName = "Assets";
    std::string UtilitiesName = "Utilities.xlsx";

    // first step: checking excel containing banks' accounts information
    std::cout << "Accesing Bank informations... ";

    // Banks.xlsx already exists

    // display message to user
    std::cout << FinCordsWkbName << " is found" << std::endl;
    std::cout << "Loading banks informations" << std::endl;

    // load Banks.xlsx using FinCordsWkb
    FinCordsWkb.load(FinCordsWkbName);

    // set AssetsWks to handle sheet Main
    AssetsWks = FinCordsWkb.sheet_by_title(SheetName);
    
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

        // get last unempty row by getting the value from utilities
        UtilitiesWkb.load(UtilitiesName);
        UtilitesWks = UtilitiesWkb.active_sheet();
        int current_row = UtilitesWks.cell("B4").value<int>();      
        // store bank data into worksheet
        // name cell for bank name in column A and one row after last_row 
        AssetsWks.cell("A", current_row).value(bank.Name);
        // asset type cell for current bank's balance in column B and one row after last_row
        AssetsWks.cell("B", current_row).value(bank.AssetType);
        // balance cell for start bank's balance in column C and one row after last_row
        AssetsWks.cell("C", current_row).value(bank.Balance);   
        // balance cell for current bank's balance in column D and one row after last_row
        AssetsWks.cell("D", current_row).value(bank.Balance);
        // formula for change in balance in column E
        AssetsWks.cell("E", current_row).formula("=D" + std::to_string(current_row) + "-C" + std::to_string(current_row));     
        
        // prepare the border for next row in current row is not the 4th row
        if (current_row >= 4){
            xlnt::border data_border = AssetsWks.cell("A4").border();
            AssetsWks.range(xlnt::range_reference("A", current_row, "E", current_row)).border(data_border);
        }
        // Update the value in utilities workbook
        UtilitesWks.cell("B4").value(current_row + 1);

        // save workbooks
        FinCordsWkb.save(FinCordsWkbName);
        UtilitiesWkb.save(UtilitiesName);
    }
}