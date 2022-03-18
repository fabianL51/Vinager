#include "Banks.h" // for handling Banks classes
#include <xlnt/xlnt.hpp> // include xlnt for excel handling
#include "utils.h" // utilities functions

/* this script process and categorizes transaction into incomes and expenses. */
/* the transaction histories will be saved in a separate excel data for documentation. */


int main(){

    // checking excel containing banks' accounts information
    std::cout << "Accesing active bank accounts" << std::endl;;

    // check if Banks.xlsx exist to read bank data
    bool isBankDoc = file_exists("Banks.xlsx");

    if (isBankDoc==1){

        std::vector<Bank> ActiveBanks;

        // initialize variables and objects
        xlnt::workbook wkb; // object to handle workbook using xlnt
        xlnt::worksheet wks; // object to handle worksheet using xlnt

        // access bank data information
        std::cout << "Bank data found... Accesing Bank informations" << std::endl;
        // load excel data
        wkb.load("Banks.xlsx");
        // set worksheet to main sheet
        wks = wkb.sheet_by_title("Main");
        // get the highest row to iterate all active bank informations
        int max_row = wks.highest_row();

        for (int i = 2; i <= max_row; i ++){
            
            // initialize temporary bank to store data from reading
            Bank tempBank;

            // prepare cells for new bank informations
            std::string name_cell = "A" + std::to_string(i); // name cell for bank name in column A
            std::string balance_cell = "B" + std::to_string(i); // balance cell for current bank's balance in column B

            // get the bank information
            tempBank.set_Bank_name(wks.cell(name_cell).to_string());
            tempBank.set_Bank_money(wks.cell(balance_cell).value<int>());
            
            // store bank into vector ActiveBanks
            ActiveBanks.emplace_back(tempBank);
        }
        std::cout << ActiveBanks.size() << std::endl;
        std::cout << "Enter to finish"; 

        char a;
        std::cin >> a;
    }
    else{
        std::cout << "please initialize your bank first." << std::endl;
    }
}
