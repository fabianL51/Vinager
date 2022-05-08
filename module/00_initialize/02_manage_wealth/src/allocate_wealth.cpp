#include "Account.h" // for handling Accounts classes
#include "WealthClass.h" // for handling Asset classes
#include <xlnt/xlnt.hpp> // include xlnt for excel handling
#include "utils.h" // utilities functions
#include <iomanip> // for set precision

/* This functions allocates all Liquid wealth of all Account accounts */


int main(){

    // get vectors of accounts, wealth classes, and asset types total from csv file
    std::vector <Account> accounts_vector = get_accounts_vector();
    std::vector <WealthClass> wealth_classes_vector = get_wealth_classes_vector();
    std::vector <double> asset_type_totals = get_assets();

    // get asset type totals
    double LiquidSum = asset_type_totals.at(0);
    double FixedSum = asset_type_totals.at(1);

    // iterate to get Account and wealth class data
    double LiquidSum = 0;
    double FixedSum = 0;

    // display Account data
    for (auto Account: accounts_vector){
        std::cout << Account.Name << " -- " << Account.AssetType << " :";
        std::cout << std::fixed << std::setprecision(2) << Account.Balance << std::endl; 
    }

    // ask user if wealth class has already been allocated
    int reallocate = 0;
    bool AllocateWealth = true;
    if (wealth_classes_vector.size() > 0){
        // inform user that wealth may already have been allocated
        std::cout << "Wealth might have already been allocated" << std::endl;

            // display current wealth class
            for (auto WealthClass:wealth_classes_vector){
                std::cout << "Layer: " << WealthClass.Name << " : ";
                std::cout << std::fixed << std::setprecision(2) << WealthClass.Sum << std::endl; 
            }

        // ask user whether the wealth should be new allocated
        std::cout << "Press 1 to reallocate wealth and any other keys to cancel ";
        std::cin >> reallocate;

        AllocateWealth = reallocate == 1;
    }

    if (AllocateWealth == true){
        
        // reinitialize wealth class vector
        wealth_classes_vector.clear();

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
                // n_wealth must be a real number to exit loop
                exit_loop = n_wealth > 0;
            }
            if (exit_loop == 0) {
                // if n_wealth invalid then inform the user
                std::cout << "Invalid number of wealth classes.";
            }
            // clear the input
            std::cin.clear();
            while (std::cin.get() != '\n') ;
            std::cout << std::endl; // empty line for better display in command window

        }

        // allocate wealth class (only for liquid assets)

        // initialize variables
        double confirm;
        double MaxLiquidAlloc = 100;
        WealthClass tempWealthClass;

        for (int i = 1; i <= n_wealth; i++){

            // reinitialize temporary wealth class
            tempWealthClass;

            // ask user the name of the wealth classs
            std::cout << "Set the name of the wealth class no. " << std::to_string(i) << ": ";
            std::getline(std::cin >> std::ws, tempWealthClass.Name);
            
            // get percentage for allocation
            if (i == n_wealth){
                // automatically assign the allocation for ultimate class
                tempWealthClass.PercentAllocation = MaxLiquidAlloc;
                tempWealthClass.Sum = (tempWealthClass.PercentAllocation / 100.0 ) * LiquidSum;
                MaxLiquidAlloc -= tempWealthClass.PercentAllocation;
                exit_loop = 1;
            }
            else {
                // ask user for classes before ultimate class and make sure user input a validate number
                exit_loop = false;
                while (exit_loop == 0){
                    // ask the amount of money to be allocated to current class
                    std::cout << std::endl << "How percentage of liquid wealth should be allocated to " << tempWealthClass.Name << " ";
                    std::cout << std::fixed << std::setprecision(2) << "(maximum allocable percentage = " << MaxLiquidAlloc << ") : ";
                    // check input validity
                    if (std::cin >> tempWealthClass.PercentAllocation){
                        // round wealth class sum and maximum liquid wealth less to prevent false checking

                        if (tempWealthClass.PercentAllocation >= 0 and tempWealthClass.PercentAllocation <= MaxLiquidAlloc){
                            // ask user confirmation
                            std::cout << std::fixed << std::setprecision(2) << tempWealthClass.PercentAllocation << " % of";
                            std::cout << "the liquid assets will be allocated to " << tempWealthClass.Name  << std::endl;
                            std::cout << "Type 1 to confirm or any other keys to cancel ";
                            std::cin >> confirm;
                                if (confirm == 1){
                                    tempWealthClass.Sum = (tempWealthClass.PercentAllocation / 100.0 ) * LiquidSum;
                                    MaxLiquidAlloc -= tempWealthClass.PercentAllocation;
                                    exit_loop = 1;
                                }
                            }
                        }
                        if (exit_loop == 0) {
                            if (tempWealthClass.PercentAllocation > MaxLiquidAlloc){
                                std::cout << "Invalid amount of allocation percentage! Maximal available amount is " << MaxLiquidAlloc << std::endl;
                            }
                            else if (tempWealthClass.PercentAllocation < 0){
                                std::cout << "Invalid amount of allocation percentage!" << std::endl;
                            }
                        }
                        // clear the input
                        std::cin.clear();
                        while (std::cin.get() != '\n') ;
                        std::cout << std::endl; // empty line for better display in command window
                    }
                }
                // update vector
                wealth_classes_vector.emplace_back(tempWealthClass);
            }

        // add fixed assets
        WealthClass Fixed;
        Fixed.Name = "Fixed Asset";
        Fixed.Sum = FixedSum;
        Fixed.PercentAllocation = 100;
        wealth_classes_vector.emplace_back(Fixed);
        n_wealth = wealth_classes_vector.size();
    } 
}

