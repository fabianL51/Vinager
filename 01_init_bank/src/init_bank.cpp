#include "Banks.h" // for handling Banks classes
#include <xlnt/xlnt.hpp> // include xlnt for excel handling
#include "utils.h" // utilities functions


/* This script initializes banks and saves the data in an excel data */

Bank initialize_bank(){

    /* This function initializes single bank upon called in main function */
    
    // initialize variables
    Bank bank_; // bank_ with class Bank (defined in Banks.h) to store all information about the bank account
    std::string Bank_name; // string to store bank's name
    double start_money = NAN; // double to store starting money, initialize as NAN for validity checking later


    // ask the user in command window for the bank name
    std::cout << "Insert the bank/account name ";
    std::cin >> Bank_name;
    std::cout << std::endl;

    // ask the user in command window for the starting money of the bank
    std::cout << "Insert the start amount of money for " << Bank_name << " : ";
    
    // checking if the input starting money is valid
    bool exit_loop = false; // varable exit_loop is bool and controls loop's exit when user gives a valid starting money
    while (exit_loop == 0){
        // begin loop as long as exit_loop is false
        if (std::cin >> start_money){
            // if start_money is valid then set exit_loop to true to exit loop
            exit_loop = 1;
        }
        else {
            // if start_money invalid then inform the user
            std::cout << "Invalid amount of money, please insert a valid number for the start amount of money.";
            // clear the input for start_money
            std::cin.clear();
            // set exit_loop to zero
            exit_loop = 0;
            // empty loop and ask user to give a new value for start_money
            while (std::cin.get() != '\n') ;
        }
        std::cout << std::endl; // empty line for better display in command window

    }

    // set bank name and initial money by calling respective functions in Banks.h
    bank_.set_Bank_name(Bank_name);
    bank_.set_Bank_initial_money(start_money);

    // return bank_ as output
    return bank_;
}



int main(){
    
    /* This is the main function for bank initializaton */

    // initialize variables and objects
    xlnt::workbook wkb; // object to handle workbook using xlnt
    xlnt::worksheet wks; // object to handle worksheet using xlnt

    // first step: checking excel containing banks' accounts information
    std::cout << "Accesing Bank informations";

    // store the existing status of Banks.xlsx by using file_exist function
    bool isfile = file_exists("Banks.xlsx");

    if (isfile == 0) {
        // if Banks.xlsx doesn't exist

        // display message to user
        std::cout << "No Banks informations can be found" << std::endl;
        std::cout << "Creating file Banks.xlsx to store new bank informations" << std::endl;

        // create an object to handle sheet Main
        wks = wkb.create_sheet();
        wks.title("Main");

        // delete first default sheet
        xlnt::worksheet wks_to_delete = wkb.sheet_by_title("Sheet1");
        wkb.remove_sheet(wks_to_delete);

        // initialize Banks.xlsx
        wks.cell("A1").value("Bank Name"); // header for bank names
        wks.cell("B1").value("Current Balance"); // header for current balance
    }
    else {
        // Banks.xlsx already exists

        // display message to user
        std::cout << "Banks.xlsx is found" << std::endl;
        std::cout << "Loading banks informations" << std::endl;

        // load Banks.xlsx using wkb
        wkb.load("Banks.xlsx");

        // set wks to handle sheet Main
        wks = wkb.sheet_by_title("Main");
    }

    // get last unempty row and inform the user
    int last_row = wks.highest_row();

    // Inform the user abput initialization
    std::cout << std::endl << "Initializing new bank informations ..." << std::endl;
    
    // call the function initialize_bank to begin bank's intialization
    Bank bank_ = initialize_bank();

    // save bank name in a string variable
    std::string bank_name = bank_.get_Bank_name();

    // prepare cells for new bank informations
    std::string name_cell = "A" + std::to_string(last_row + 1); // name cell for bank name in column A and one row after last_row 
    std::string balance_cell = "B" + std::to_string(last_row + 1); // balance cell for current bank's balance in column B and one row after last_row
    
    // Inform the user about saving new bank properties
    std::cout << std::endl << "Saving new bank informations ..." << std::endl;

    // write the bank's property in excel table
    wks.cell(name_cell).value(bank_name);
    wks.cell(balance_cell).value(bank_.get_Bank_money());
    // column properties: währung

    // Save the workbook and free any allocated memory
    wkb.save("Banks.xlsx");
}