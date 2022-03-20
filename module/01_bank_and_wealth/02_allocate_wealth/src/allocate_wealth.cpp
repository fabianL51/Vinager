#include "Banks.h" // for handling Banks classes
#include "Asset.h" // for handling Asset classes
#include <xlnt/xlnt.hpp> // include xlnt for excel handling
#include "utils.h" // utilities functions
#include <iomanip> // for set precision

/* This functions allocates all Liquid wealth of all bank accounts */


int main(){

    // checking excel containing banks' accounts information
    std::cout << "Accesing active bank accounts" << std::endl;;

    // check if Banks.xlsx exist to read bank data
    bool isBankDoc = file_exists("Banks.xlsx");

    if (isBankDoc==1){

        // initialize variables and objects
        xlnt::workbook BankWkb; // object to handle workbook using xlnt
        xlnt::worksheet BankMainWks, BankWealthWks; // object to handle worksheet using xlnt
        
        // initializes asset classes
        Asset Liquid, Fixed; // fixed and liquid asset
        Liquid.set_asset_type("Liquid"); // set liquid asset type
        Fixed.set_asset_type("Fixed"); // set fixed asset type

        // access bank data information
        std::cout << "Bank data found... Accesing Bank informations" << std::endl;
        // load excel data
        BankWkb.load("Banks.xlsx");

        // initialize worksheets
        BankMainWks = BankWkb.sheet_by_title("Main");

        // check if wealth is already allocated
        bool AllocateWealth;
        if (BankWkb.contains("Wealth")){
            AllocateWealth = false;
            BankWealthWks = BankWkb.sheet_by_title("Wealth");
            
            // inform user that wealth is found
            std::cout << "Wealth is already allocated" << std::endl;

            // show user of current wealth allocations
            int last_row = BankWealthWks.highest_row();
            int reallocate;
            std::string cell_name, cell_balance;
            for (int i = 2; i <= last_row; i++){
                // prepare cells for storing wealth informations
                // row 0 doesn't exist and row 1 is header, so row i + 2 is the right row
                cell_name = "A" + std::to_string(i);
                cell_balance = "B" + std::to_string(i);

                // display in command prompt
                std::cout << "Layer : " << BankWealthWks.cell(cell_name).value<std::string>() << " : ";
                std::cout << std::fixed << std::setprecision(2) << BankWealthWks.cell(cell_balance).value<double>() << std::endl; 
            }

            // ask user whether the wealth should be new allocated
            std::cout << "Press 1 to reallocate wealth and any other keys to cancel ";
            std::cin >> reallocate;

            if (reallocate == 1){
                AllocateWealth = true;
            }

        }
        else {
            std::cout << "Wealth sheet doesn't exist" << std::endl;
            AllocateWealth = true;
            BankWealthWks = BankWkb.create_sheet();
            BankWealthWks.title("Wealth");
            // initialize header
            BankWealthWks.cell("A1").value("Class");
            BankWealthWks.cell("B1").value("Balance");
        }

        if (AllocateWealth == true){
            // get the highest row to iterate all active bank informations
            int max_row = BankMainWks.highest_row();

            // initialize cells for new bank informations
            std::string name_cell, balance_cell, asset_type_cell;

            for (int i = 2; i <= max_row; i ++){
                
                // initialize temporary bank to store data from reading
                Bank tempBank;

                // prepare cells for new bank informations
                name_cell = "A" + std::to_string(i);
                balance_cell = "B" + std::to_string(i);
                asset_type_cell = "C" + std::to_string(i);

                // display current bank informations 
                std::cout << BankMainWks.cell(name_cell).value<std::string>() << " type: ";
                std::cout << BankMainWks.cell(asset_type_cell).value<std::string>() << " balance: ";
                std::cout << BankMainWks.cell(balance_cell).value<double>() << std::endl;

                // add sum to asset class
                if (BankMainWks.cell(asset_type_cell).value<std::string>() == "Liquid"){
                    Liquid.set_asset_sum(Liquid.get_asset_sum() + BankMainWks.cell(balance_cell).value<double>());
                }
                else if (BankMainWks.cell(asset_type_cell).value<std::string>() == "Fixed"){
                    Fixed.set_asset_sum(Fixed.get_asset_sum() + BankMainWks.cell(balance_cell).value<double>());
                }
                
            }

            // inform user of total assets of each asset type
            std::cout << "Liquid assets total: " << std::fixed << std::setprecision(2) << Liquid.get_asset_sum();
            std::cout << " and Fixed assets total: " << std::fixed << std::setprecision(2) << Fixed.get_asset_sum() << std::endl;

            // ask user how many wealth classes
            int n_wealth;
            bool exit_loop = false;
            // checking if the input starting money is valid
            std::cout << "Set the number of wealth classes ";
            while (exit_loop == 0){
                // begin loop as long as exit_loop is false
                if (std::cin >> n_wealth){
                    if (n_wealth > 0){
                    // n_wealth must be a real number to exit loop
                    exit_loop = 1;
                    }
                    
                }
                if (exit_loop == 0) {
                    // if n_wealth invalid then inform the user
                    std::cout << "Invalid number of wealth classes... Set the number of wealth classes";
                    // clear the input for n_wealth
                    std::cin.clear();
                    // set exit_loop to zero
                    exit_loop = 0;
                    // empty loop and ask user to give a new value for n_wealth
                    while (std::cin.get() != '\n') ;
                }
                std::cout << std::endl; // empty line for better display in command window

            }

            // allocate wealth class (only for liquid assets)
            std::vector <std::string> WealthClassName;
            std::vector <double> WealthClassSum;
            std::string tempName, a;
            double WealthClassPercentage, tempSum, confirm;
            double max_percentage = 100;
            for (int i = 1; i <= n_wealth; i++){
                std::cout << "Set the name of the wealth class no. " << std::to_string(i) << " ";
                std::getline(std::cin >> std::ws, tempName);
                std::cout << std::endl << "How many percentage of liquid wealth should be allocated to " << tempName;
                std::cout << std::fixed << std::setprecision(2) << "maximum possible percentage = " << max_percentage << " ";
                // make sure user input a validate number
                exit_loop = false;
                while (exit_loop == 0){
                    // begin loop as long as exit_loop is false
                    if (std::cin >> WealthClassPercentage){
                        if (WealthClassPercentage >= 0 and WealthClassPercentage <= max_percentage){
                            // wealth class percentage must be between 0 and 100
                            tempSum = Liquid.get_asset_sum() * WealthClassPercentage / 100;
                            std::cout << std::fixed << std::setprecision(2) << WealthClassPercentage << "% represents ";
                            std::cout << std::fixed << std::setprecision(2) << tempSum << std::endl;
                            std::cout << "Type 1 to confirm or any other keys to cancel ";
                            std::cin >> confirm;
                            if (confirm == 1){
                                max_percentage -= WealthClassPercentage;
                                exit_loop = 1;
                            }
                        }
                    }
                    if (exit_loop == 0) {
                        // if wealth class percentage invalid then inform the user
                        std::cout << std::endl << "How many percentage of liquid wealth should be allocated to " << tempName;
                        std::cout << ". Maximum possible percentage = " << max_percentage << " ";
                        // clear the input for wealth class percentage
                        std::cin.clear();
                        // set exit_loop to zero
                        exit_loop = 0;
                        // empty loop and ask user to give a new value for wealth class percentage
                        while (std::cin.get() != '\n') ;
                    }
                    std::cout << std::endl; // empty line for better display in command window
                }

                // update vector
                WealthClassName.emplace_back(tempName);
                WealthClassSum.emplace_back(tempSum);
            }

            // add fixed assets
            WealthClassName.emplace_back(Fixed.get_asset_type());
            WealthClassSum.emplace_back(Fixed.get_asset_sum());

            // display in command prompt and store new wealth informations
            for (int i = 0; i < WealthClassName.size(); i++){
                // prepare cells for storing wealth informations
                // row 0 doesn't exist and row 1 is header, so row i + 2 is the right row
                name_cell = "A" + std::to_string(i + 2);
                balance_cell = "B" + std::to_string(i + 2);

                // update the wealth information in excel
                BankWealthWks.cell(name_cell).value(WealthClassName.at(i));
                BankWealthWks.cell(balance_cell).value(WealthClassSum.at(i));

                // display in command prompt
                std::cout << "Layer : " << WealthClassName.at(i) << " : " << std::fixed << std::setprecision(2) << WealthClassSum.at(i) << std::endl; 
            }

            // save workbook
            BankWkb.save("Banks.xlsx");     
        }
              
    }
}