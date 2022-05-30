#include "Account.h" // for handling Accounts classes
#include "utils.h" // utilities functions
#include <iomanip> // for set precision

/* This functions allocates all Liquid wealth of all Account accounts */


int main(){

    // get vectors of accounts, wealth classes, and asset types total from csv file
    std::vector <Account> accounts_vector = get_accounts_vector();
    std::vector <WealthClass> wealth_classes_vector = get_wealth_classes_vector();
    std::map <std::string, double> asset_type_map = get_string_double_map("asset_type");

    // initialize map to store asset type starting balance
    std::map <std::string, double> asset_type_start_map;
    asset_type_start_map["Liquid"] = 0;
    asset_type_start_map["Fixed"] = 0;

    // display Account data
    for (auto Account: accounts_vector){
        std::cout << Account.Name << " -- " << Account.AssetType << " :";
        std::cout << std::fixed << std::setprecision(2) << Account.Balance << std::endl; 
        // update starting balance for each asset type
        asset_type_start_map[Account.AssetType] += Account.StartBalance;
    }
    std::cout << "------------------------------------------------------" << std::endl;

    // ask user if wealth class has already been allocated
    int command_code = 1;
    if (wealth_classes_vector.size() > 0){
        // inform user that wealth may already have been allocated
        std::cout << "Wealth might have already been allocated" << std::endl;

            // display current wealth class
            for (auto WealthClass:wealth_classes_vector){
                std::cout << "Layer: " << WealthClass.Name << " : ";
                std::cout << std::fixed << std::setprecision(2) << WealthClass.Sum << std::endl; 
            }
        
        std::cout << "------------------------------------------------------" << std::endl;
        // ask user whether the wealth should be new allocated
        std::cout << "Press 1 to redefine wealth class" << std::endl << "Press 2 to reallocate wealth class" << std::endl <<
             "Press any other keys to cancel ";
        std::cin >> command_code;
    }

    switch (command_code)
    {
    case 1:
        {
            /* redefine wealth classes */

            // reinitialize wealth class vector
            wealth_classes_vector.clear();

            // inform user of total assets of each asset type
            for (auto const& map: asset_type_map){
                std::cout << map.first << " assets total: " << std::fixed << std::setprecision(2) << map.second << std::endl;
            }

            // initialize variables to get number of wealth classes
            int n_wealth;
            bool exit_loop = false;

            // ask user how many wealth classes
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

            /* allocate wealth class (only for liquid assets) */

            // initialize variables
            double confirm;
            double MaxLiquidAlloc = 100;
            WealthClass tempWealthClass; // temporary wealth class to store
            std::fstream wealth_class_csv; // fstream to handle csv for wealth class
            char delimiter = GlobalData::csv_config::delimiter; // delimiter to write csv. data

            // open csv with write permission (replace old data)
            wealth_class_csv.open(GlobalData::FileNames::wealth_classes_csv, std::ios::out);

            // ask user for allocation
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
                    tempWealthClass.Sum = (tempWealthClass.PercentAllocation / 100.0 ) * asset_type_map["Liquid"];
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
                                        tempWealthClass.Sum = (tempWealthClass.PercentAllocation / 100.0 ) * asset_type_map["Liquid"];
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

                    // write data in csv: start sum is equal to sum
                    wealth_class_csv << tempWealthClass.Name <<  delimiter << tempWealthClass.PercentAllocation 
                        << delimiter << tempWealthClass.Sum << delimiter << tempWealthClass.Sum << "\n"; 

                }

            // add fixed assets in csv data: start sum is equal to current sum
            wealth_class_csv << "Fixed Asset" << delimiter << 100 << delimiter << asset_type_map["Fixed"] << delimiter << asset_type_map["Fixed"] << "\n";

            // save and close csv data
            wealth_class_csv.close();

            break;
        }

    case 2:{
        /* reallocate wealth classes */

        // initialize variables
        bool exit_loop = false;
        double confirm;
        double MaxLiquidAlloc = 100;
        std::fstream wealth_class_csv; // fstream to handle csv for wealth class
        char delimiter = GlobalData::csv_config::delimiter; // delimiter to write csv. data

        // open csv with write permission (replace old data)
        wealth_class_csv.open(GlobalData::FileNames::wealth_classes_csv, std::ios::out);

        // iterate all wealth classes to get new allocation
        for (int i = 0; i < wealth_classes_vector.size(); i++){
            // filter fixed asset out
            if (wealth_classes_vector.at(i).Name.find("Fixed") == std::string::npos){
                // if not fixed assets = liquid, then ask the allocation
                // get percentage for allocation
                if (i == wealth_classes_vector.size() - 2){
                    // automatically assign the allocation for ultimate class: -2 because the last class is fixed asset class
                    wealth_classes_vector.at(i).PercentAllocation = MaxLiquidAlloc;
                    wealth_classes_vector.at(i).StartSum = (wealth_classes_vector.at(i).PercentAllocation / 100.0 ) * asset_type_start_map["Liquid"];
                    wealth_classes_vector.at(i).Sum = (wealth_classes_vector.at(i).PercentAllocation / 100.0 ) * asset_type_map["Liquid"];
                    MaxLiquidAlloc -= wealth_classes_vector.at(i).PercentAllocation;
                    exit_loop = 1;
                }
                else {
                    // ask user for classes before ultimate class and make sure user input a validate number
                    exit_loop = false;
                    while (exit_loop == 0){
                        // ask the amount of money to be allocated to current class
                        std::cout << std::endl << "How percentage of liquid wealth should be allocated to " << wealth_classes_vector.at(i).Name << " ";
                        std::cout << std::fixed << std::setprecision(2) << "(maximum allocable percentage = " << MaxLiquidAlloc << ") : ";
                        // check input validity
                        if (std::cin >> wealth_classes_vector.at(i).PercentAllocation){
                            // round wealth class sum and maximum liquid wealth less to prevent false checking

                            if (wealth_classes_vector.at(i).PercentAllocation >= 0 and wealth_classes_vector.at(i).PercentAllocation <= MaxLiquidAlloc){
                                // ask user confirmation
                                std::cout << std::fixed << std::setprecision(2) << wealth_classes_vector.at(i).PercentAllocation << " % of";
                                std::cout << "the liquid assets will be allocated to " << wealth_classes_vector.at(i).Name  << std::endl;
                                std::cout << "Type 1 to confirm or any other keys to cancel ";
                                std::cin >> confirm;
                                    if (confirm == 1){
                                        wealth_classes_vector.at(i).StartSum = (wealth_classes_vector.at(i).PercentAllocation / 100.0 ) * asset_type_start_map["Liquid"];
                                        wealth_classes_vector.at(i).Sum = (wealth_classes_vector.at(i).PercentAllocation / 100.0 ) * asset_type_map["Liquid"];
                                        MaxLiquidAlloc -= wealth_classes_vector.at(i).PercentAllocation;
                                        exit_loop = 1;
                                    }
                                }
                            }
                            if (exit_loop == 0) {
                                if (wealth_classes_vector.at(i).PercentAllocation > MaxLiquidAlloc){
                                    std::cout << "Invalid amount of allocation percentage! Maximal available amount is " << MaxLiquidAlloc << std::endl;
                                }
                                else if (wealth_classes_vector.at(i).PercentAllocation < 0){
                                    std::cout << "Invalid amount of allocation percentage!" << std::endl;
                                }
                            }
                            // clear the input
                            std::cin.clear();
                            while (std::cin.get() != '\n') ;
                            std::cout << std::endl; // empty line for better display in command window
                        }
                    }

                }
            // write data in csv: start sum is equal to sum
            wealth_class_csv << wealth_classes_vector.at(i).Name <<  delimiter << wealth_classes_vector.at(i).PercentAllocation 
                << delimiter << wealth_classes_vector.at(i).StartSum << delimiter << wealth_classes_vector.at(i).Sum << "\n"; 

            }
        // save and close csv data
        wealth_class_csv.close();

        break;
    }
        
    default:
        break;
    }

    
}

