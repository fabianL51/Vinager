#include "Banks.h" // for handling Banks classes
#include "Asset.h" // for handling Asset classes
#include <xlnt/xlnt.hpp> // include xlnt for excel handling
#include "utils.h" // utilities functions
#include <iomanip> // for set precision

/* This functions allocates all Liquid wealth of all bank accounts */


int main(){

    // checking excel containing banks' accounts information
    std::cout << "Accesing active bank accounts" << std::endl;;

    // initialize workbook and worksheets
    xlnt::workbook FinCordsWkb, UtilWkb;
    xlnt::worksheet AssetWks, FinStateWks, UtilWks;
    std::string FinCordsWkbName = std::to_string(xlnt::date::today().year) + "_FinancialRecords.xlsx";
    std::string UtilWkbName = "Utilities.xlsx";

    // load workbooks
    FinCordsWkb.load(FinCordsWkbName);
    UtilWkb.load(UtilWkbName);

    // open worksheets
    AssetWks = FinCordsWkb.sheet_by_title("Assets");
    FinStateWks = FinCordsWkb.sheet_by_title("Financial Statement");
    UtilWks = UtilWkb.active_sheet();

    // get the last row of both accounts and wealth
    int acc_last_row = UtilWks.cell("B4").value<int>() - 1;
    int wealth_current_row = UtilWks.cell("B5").value<int>();
    int wealth_last_row = wealth_current_row - 1;

    // iterate to get bank and wealth class data
    // initialize variables
    std::vector <Bank> BankVec;
    std::vector <Asset> WealthClassVec;
    double LiquidSum = 0;
    double FixedSum = 0;
    int reallocate = 0;
    bool AllocateWealth = true;

    // get accounts and wealth classes
    std::pair<std::vector <Bank>, std::vector <Asset>> bank_wealth_vec = get_accounts_wealth(AssetWks, acc_last_row, wealth_last_row);
    BankVec = bank_wealth_vec.first;
    WealthClassVec = bank_wealth_vec.second;

    // display bank data
    for (int i = 0; i <= BankVec.size() - 1; i++){
        std::cout << BankVec.at(i).Name << " -- " << BankVec.at(i).AssetType << " :";
        std::cout << std::fixed << std::setprecision(2) << BankVec.at(i).Balance << std::endl; 

        if (BankVec.at(i).AssetType == "Liquid"){
            LiquidSum += BankVec.at(i).Balance;
        }
        else if (BankVec.at(i).AssetType == "Fixed"){
            FixedSum += BankVec.at(i).Balance;
        }
    }

    // ask user if wealth class has already been allocated
    if (WealthClassVec.size() > 0){
        // inform user that wealth may already have been allocated
        std::cout << "Wealth might have already been allocated" << std::endl;

            // display current wealth class
            for (int i = 0; i <= WealthClassVec.size() - 1; i++){
            std::cout << "Layer: " << WealthClassVec.at(i).Name << " : " ;
            std::cout << std::fixed << std::setprecision(2) << WealthClassVec.at(i).Sum << std::endl; 
        }

        // ask user whether the wealth should be new allocated
        std::cout << "Press 1 to reallocate wealth and any other keys to cancel ";
        std::cin >> reallocate;

        if (reallocate != 1){
            AllocateWealth = false;
        }
    }

    if (AllocateWealth == true){
        
        // reinitialize wealth class vector
        WealthClassVec.clear();

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

        // initialize variables
        double confirm;
        double MaxLiquidWealth = LiquidSum;
        Asset tempWealthClass;

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
                WealthClassVec.emplace_back(tempWealthClass);
            }

        // add fixed assets
        Asset Fixed;
        Fixed.Name = "Fixed Asset";
        Fixed.Sum = FixedSum;
        WealthClassVec.emplace_back(Fixed);
        n_wealth = WealthClassVec.size();
        
        // reset values of current wealth row
        wealth_current_row = 3;
        // delete unused cells
        xlnt::range_reference delete_range = xlnt::range_reference("G", 4, "J", AssetWks.highest_row());
        AssetWks.range(delete_range).clear_cells();
        // reborder Assets sheet
        xlnt::border range_border = create_data_border();
        xlnt::range_reference reborder_range;
        reborder_range = xlnt::range_reference("G", 4, "J", n_wealth + wealth_current_row - 1);
        AssetWks.range(reborder_range).border(range_border);        

        // display in command prompt and store new wealth informations
        for (int i = 0; i < WealthClassVec.size(); i++){

            // update the wealth information in Assets sheet
            // row 0 doesn't exist and row 1 is header, so row i + wealth current row
            AssetWks.cell("G", i + wealth_current_row).value(WealthClassVec.at(i).Name); // col G: wealth class name
            AssetWks.cell("H", i + wealth_current_row).value(WealthClassVec.at(i).Sum); // col H: start balance
            AssetWks.cell("I", i + wealth_current_row).value(WealthClassVec.at(i).Sum); // col I: current balance
            // formula for change in balance
            AssetWks.cell("J", i + wealth_current_row).formula("=I" + std::to_string(i + wealth_current_row) + "-H" + std::to_string(i + wealth_current_row));

            
            // display in command prompt
            std::cout << "Layer : " << WealthClassVec.at(i).Name << " : ";
            std::cout << std::fixed << std::setprecision(2) << WealthClassVec.at(i).Sum << std::endl; 


        }

        // save both fixed and liquid assets in column L and M
        AssetWks.cell("L2").value("Liquid");
        AssetWks.cell("L3").value("Fixed");
        AssetWks.cell("M2").value(LiquidSum);
        AssetWks.cell("M3").value(FixedSum);

        // update rows in utilities
        UtilWks.cell("B5").value(wealth_current_row + n_wealth);

        // save workbooks
        FinCordsWkb.save(FinCordsWkbName);
        UtilWkb.save(UtilWkbName); 
    } 
}

