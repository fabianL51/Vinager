#include "Banks.h" // for handling Banks classes
#include "Asset.h" // for handling Asset classes
#include <xlnt/xlnt.hpp> // include xlnt for excel handling
#include "utils.h" // utilities functions
#include <iomanip> // for set precision

/* This functions allocates all Liquid wealth of all bank accounts */


int main(){

    // checking excel containing banks' accounts information
    std::cout << "Accesing active bank accounts" << std::endl;;

    // initialize variables and objects
    xlnt::workbook BanksWkb;
    xlnt::worksheet BanksAccWks, BanksWealthWks;
    std::string WorkbookName = "Banks.xlsx";
    int reallocate;
    std::string wealth_class_cell, wealth_sum_cell;
    double LiquidSum = 0;
    double FixedSum = 0;
    bool AllocateWealth = true;

    // load workbook
    BanksWkb.load(WorkbookName);

    // open worksheets
    BanksAccWks = BanksWkb.sheet_by_title("Accounts");
    BanksWealthWks = BanksWkb.sheet_by_title("Wealth Allocation");

    // get the last row of Wealth Allocation
    int wealth_last_row = BanksWealthWks.highest_row();

    if (wealth_last_row > 1){
        // inform user that wealth may already have been allocated
        std::cout << "Wealth might have already been allocated" << std::endl;
        std::cout << wealth_last_row << std::endl;

        for (int i = 2; i <= wealth_last_row; i++){
            // prepare cells for storing wealth informations
            // row 0 doesn't exist and row 1 is header, so row i + 2 is the right row
            wealth_class_cell = "A" + std::to_string(i);
            wealth_sum_cell = "B" + std::to_string(i);

            // display in command prompt
            std::cout << "Layer : " << BanksWealthWks.cell(wealth_class_cell).value<std::string>() << " : ";
            std::cout << std::fixed << std::setprecision(2) << BanksWealthWks.cell(wealth_sum_cell).value<double>() << std::endl; 
        }

        // ask user whether the wealth should be new allocated
        std::cout << "Press 1 to reallocate wealth and any other keys to cancel ";
        std::cin >> reallocate;

        if (reallocate != 1){
            AllocateWealth = false;
        }
    }

    if (AllocateWealth == true){

        // get last row of bank accounts sheet
        double account_last_row = BanksAccWks.highest_row();

        for (int i = 2; i <= account_last_row; i ++){
                
            // initialize cells for new bank informations
            std::string name_cell, balance_cell, asset_type_cell;

            // prepare cells for new bank informations
            name_cell = "A" + std::to_string(i);
            balance_cell = "B" + std::to_string(i);
            asset_type_cell = "C" + std::to_string(i);

            // display current bank informations 
            std::cout << BanksAccWks.cell(name_cell).value<std::string>() << " - ";
            std::cout << BanksAccWks.cell(asset_type_cell).value<std::string>() << " - ";
            std::cout << BanksAccWks.cell(balance_cell).value<double>() << std::endl;

            // get liquid and fixed asset sum
            if (BanksAccWks.cell(asset_type_cell).value<std::string>() == "Liquid"){
                LiquidSum += BanksAccWks.cell(balance_cell).value<double>();
            }
            else if (BanksAccWks.cell(asset_type_cell).value<std::string>() == "Fixed"){
                FixedSum += BanksAccWks.cell(balance_cell).value<double>();
            }
        }

        // inform user of total assets of each asset type
        std::cout << "Liquid assets total: " << std::fixed << std::setprecision(2) << LiquidSum;
        std::cout << " and Fixed assets total: " << std::fixed << std::setprecision(2) << FixedSum << std::endl;

        // ask user how many wealth classes
        int n_wealth;
        bool exit_loop = false;
        // checking if the input starting money is valid
        std::cout << "Set the number of wealth classes for liquid assets ";
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
        std::vector <Asset> WealthClass;
        Asset tempWealthClass;
        double confirm;
        double MaxLiquidWealth = LiquidSum;

        for (int i = 1; i <= n_wealth; i++){

            // reinitialize temporary wealth class
            tempWealthClass;

            // ask user the name of the wealth classs
            std::cout << "Set the name of the wealth class no. " << std::to_string(i) << ": ";
            std::getline(std::cin >> std::ws, tempWealthClass.Name);
            
            // ask the amount of money to be allocated to current class
            std::cout << std::endl << "How many money of liquid wealth should be allocated to " << tempWealthClass.Name << " ";
            std::cout << std::fixed << std::setprecision(2) << "(maximum available liquid wealth = " << MaxLiquidWealth << ") : ";

            // make sure user input a validate number
            exit_loop = false;
            while (exit_loop == 0){
                // begin loop as long as exit_loop is false
                if (std::cin >> tempWealthClass.Sum){
                    
                    // round wealth class sum and maximum liquid wealth less to prevent false checking
                    tempWealthClass.Sum = round( tempWealthClass.Sum * 100.0 ) / 100.0;
                    MaxLiquidWealth = round( MaxLiquidWealth * 100.0 ) / 100.0;

                    if (tempWealthClass.Sum >= 0 and tempWealthClass.Sum <= MaxLiquidWealth){
                        // ask user confirmation
                        std::cout << std::fixed << std::setprecision(2) << tempWealthClass.Sum << " of ";
                        std::cout << std::fixed << std::setprecision(2) << MaxLiquidWealth;
                        std::cout << " will be allocated to " << tempWealthClass.Name  << std::endl;
                        std::cout << "Type 1 to confirm or any other keys to cancel ";
                            std::cin >> confirm;
                            if (confirm == 1){
                                MaxLiquidWealth -= tempWealthClass.Sum;
                                exit_loop = 1;
                            }
                        }
                    }
                    if (exit_loop == 0) {
                        // if wealth class percentage invalid then inform the user
                        std::cout << std::endl << "How many money of liquid wealth should be allocated to " << tempWealthClass.Name << " ";
                        std::cout << std::fixed << std::setprecision(2) << "(maximum available liquid wealth = " << MaxLiquidWealth << ") : ";
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
                WealthClass.emplace_back(tempWealthClass);
            }

        // add fixed assets
        Asset Fixed;
        Fixed.Name = "Fixed Asset";
        Fixed.Sum = FixedSum;
        WealthClass.emplace_back(Fixed);

        // display in command prompt and store new wealth informations
        for (int i = 0; i < WealthClass.size(); i++){
            // prepare cells for storing wealth informations
            // row 0 doesn't exist and row 1 is header, so row i + 2 is the right row
            wealth_class_cell = "A" + std::to_string(i + 2);
            wealth_sum_cell = "B" + std::to_string(i + 2);


            // update the wealth information in excel
            BanksWealthWks.cell(wealth_class_cell).value(WealthClass.at(i).Name);
            BanksWealthWks.cell(wealth_sum_cell).value(WealthClass.at(i).Sum);

            // display in command prompt
            std::cout << "Layer : " << WealthClass.at(i).Name << " : " << std::fixed << std::setprecision(2) << WealthClass.at(i).Sum << std::endl; 
        }

        // save both fixed and liquid assets in column G and H
        BanksWealthWks.cell("G2").value("Liquid");
        BanksWealthWks.cell("G3").value("Fixed");
        BanksWealthWks.cell("H2").value(LiquidSum);
        BanksWealthWks.cell("H3").value(FixedSum);

        // save workbook
        BanksWkb.save(WorkbookName);
    }
}

