#include "utils.h" // utilities functions

/* this script processes and categorizes transaction into incomes and expenses. */
/* the transaction histories will be saved in a separate excel data for documentation. */


std::string get_transaction_category(std::map <std::string, double> categories_total_map){
    /* get the user input for the current transaction category */

    // initialize variables
    int index = 1;

    // create index to category map
    std::map <int, std::string> index_category_map;
    
    // iterate over all current categories
    for (auto const& map_member:categories_total_map){
        // display all possible categories to be chosen
        std::cout << index << ": " << map_member.first << std::endl;
        // add map member
        index_category_map[index] = map_member.first;
        index += 1;
    }

    // display option to add new category
    std::cout << index << ": Add a new category" << std::endl;

    // get user's wish
    bool valid_category = false;
    std::string category;
    int category_index;
    while(valid_category == false){
        std::cout << "Enter a number for the category of the transaction: " << std::endl;
        if (std::cin >> category_index){
            // check if user's input is valid: either the index exist in category map or it is the last index to add a new category
            valid_category = index_category_map.count(category_index) > 0 or category_index == index;
            if (valid_category == false){
                // redisplay possible choice
                for (auto const& map_member:index_category_map){
                    // display all possible categories to be chosen
                    std::cout << map_member.first << ": " << map_member.second << std::endl;
                }
                // display option to add new category
                std::cout << index << ": Add a new category" << std::endl;
            }
        }
        // clear the input
        std::cin.clear();
        while (std::cin.get() != '\n') ;
        std::cout << std::endl; // empty line for better display in command window
    }

    if (category_index == index){
        // add a new category
        std::cout << "Insert the name of the new category: ";
        std::getline(std::cin, category);
    }
    else {
            // set category from map
        category = index_category_map[category_index];
    }
    
    return category;
}

int main(){

    // initialize fstream for transaction and overview csvs
    std::fstream transaction_records_csv, monthly_records_csv, accounts_csv, asset_type_csv, wealth_class_csv;
    // initialize delimiter
    char delimiter = GlobalData::csv_config::delimiter;

    // get vectors of accounts, wealth classes, and asset types total from csv file
    std::vector <Account> accounts_vector = get_accounts_vector();
    std::vector <WealthClass> wealth_classes_vector = get_wealth_classes_vector();
    // std::map <std::string, double> asset_type_map = get_string_double_map("asset_type"); // currently not needed

    // get categories from csv file
    std::map <std::string, double> categories_total_map = get_string_double_map("category");

    // get last transaction id from csv file
    int transaction_id = get_last_transaction_process_id();

    // get maps between account names and their codenames
    std::map <std::string, int> codename_index_map = map_codename_to_index(accounts_vector);

    // create transaction type map
    std::map <std::string, std::string> transaction_type_map;
    transaction_type_map["T"] = "Interaccount";
    transaction_type_map["E"] = "Expense";
    transaction_type_map["I"] = "Income";
    
    // display all accounts
    std::cout << "------------- Current accounts balance ----------" << std::endl;
    for (auto Account: accounts_vector){
        std::cout << Account.Name << "(" << Account.CodeName << ") - " << Account.AssetType << ": " << Account.Balance << std::endl;
    }

    // display all wealth class
    std::cout << "------------- Current wealth class balance ----------" << std::endl;
    for (auto WealthClass: wealth_classes_vector){
        std::cout << WealthClass.Name << ": " << WealthClass.Sum << std::endl;
    }

    std::cout << "------------------------------------------------------" << std::endl;

    // boolean to store user's wish to input transaction
    bool input = true;

    // initialize variables for transaction records
    double transaction_amount; // transaction_amount
    std::string category, transaction_acc, transaction_detail; // Category and transaction accounts, transaction detail

    // initialize codenames for all possible transactions
    std::string giver_acc_codename, receiver_acc_codename, payment_acc_codename;

    // initialize variables to process transaction
    std::string transaction_type; // to store transaction type
    std::string date_today; // to store date for transaction records
    // all booleans for validity checks
    bool valid_payment_acc, valid_transaction_type, valid_giver_acc, valid_receiver_acc, valid_money;
    // maximum possible money in the transaction
    double MaxGiverMoney, MaxPayableAmount;

    // open transaction records to write by append
    transaction_records_csv.open(GlobalData::FileNames::transaction_records_csv, std::ios::app);

    // loop
    while (input == true){

        // update transaction id
        transaction_id += 1;

        // ask the user which transactions should be processed
        valid_transaction_type = false; // set valid transaction to false
        transaction_type.clear(); // clear transaction type
        while (valid_transaction_type == false){
            std::cout << "Define a transaction type: " << std::endl;
            std::cout << "Type I for Income, E for Expense and T for interaccount transfers" << std::endl;
            std::getline(std::cin, transaction_type);
            valid_transaction_type = transaction_type == "I" or transaction_type == "E" or transaction_type == "T";
        }
        
        // process by transaction type
        if (transaction_type == "T"){
            /* INTERACCOUNT TRANSFERS */

            // initialize codenames of giver and receiver accounts
            
            std::cout << "Interaccount transfers selected" << std::endl;
            // get giver account
           valid_giver_acc = false; // boolean to check the input validity
            while (valid_giver_acc == false){
                std::cout << "Enter the codename of the GIVER account" << std::endl;
                // loop possible codenames for the giver account
                for (auto const& map_member: codename_index_map){
                    std::cout << "Type " << map_member.first << " for " << accounts_vector.at(map_member.second).Name << std::endl;
                }
                std::getline(std::cin, giver_acc_codename);
                // check validity of giver account: input codename exists in codename to index map
                valid_giver_acc = codename_index_map.count(giver_acc_codename) > 0;
            }

            // get receiver account
            valid_receiver_acc = false; // boolean to check the input validity
            while (valid_receiver_acc == false){
                std::cout << "Enter the codename of the RECEIVER account" << std::endl;
                // loop possible codenames for the receiver account
                for (auto const& map_member: codename_index_map){
                    // leave giver account out from loop
                    if (map_member.first != giver_acc_codename){
                        std::cout << "Type " << map_member.first << " for " << accounts_vector.at(map_member.second).Name << std::endl;
                    }
                }
                std::getline(std::cin, receiver_acc_codename);
                // check validity of receiver account: input codename exists in codename to index map and is not identical to giver codename
                valid_receiver_acc = codename_index_map.count(receiver_acc_codename) > 0  and receiver_acc_codename != giver_acc_codename;
            }

            // get the amount of money for the interaccount transfer
            valid_money = false; // boolean to check the money input
            double MaxGiverMoney = std::round(accounts_vector.at(codename_index_map[giver_acc_codename]).Balance * 100) / 100; // round two decimals
            while (valid_money == false){
                // ask user how much money would be transferred
                std::cout << "Enter the amount of money to be transferred from " << accounts_vector.at(codename_index_map[giver_acc_codename]).Name;
                std::cout << " to " << accounts_vector.at(codename_index_map[receiver_acc_codename]).Balance << std::endl;
                std::cout << "Max. transferrable amounts = " << MaxGiverMoney << std::endl;
                // check if the validity of the transferred money: greater than zero and not higher than max giver money
                if (std::cin >> transaction_amount){
                    valid_money = transaction_amount > 0  and std::round(transaction_amount * 100) / 100 <= MaxGiverMoney;
                }
                // clear the input
                std::cin.clear();
                while (std::cin.get() != '\n') ;
                std::cout << std::endl; // empty line for better display in command window 
            }
            // store transfer flow in transaction account
            transaction_acc = accounts_vector.at(codename_index_map[giver_acc_codename]).Name + " to " + accounts_vector.at(codename_index_map[receiver_acc_codename]).Name;

            // update balance of giver and receiver accounts
            accounts_vector.at(codename_index_map[giver_acc_codename]).Balance -= transaction_amount;
            accounts_vector.at(codename_index_map[receiver_acc_codename]).Balance += transaction_amount;

            // category = Interaccount
            category = transaction_type_map[transaction_type];
        }
        else if (transaction_type == "E"){
            /* EXPENSES */

            std::cout << "Expenses selected" << std::endl;
            // get paying account
            valid_payment_acc = false;
            while (valid_payment_acc == false){
                std::cout << "Enter the codename of the payment account for the expense" << std::endl;
                // display codenames and their corresponding accounts to user
                for (auto const& map_member: codename_index_map){
                    std::cout << "Type " << map_member.first << " for " << accounts_vector.at(map_member.second).Name << std::endl;
                }
                std::getline(std::cin, payment_acc_codename);
                // check validity of giver account by checking codenames map
                valid_payment_acc = codename_index_map.count(payment_acc_codename) > 0;
            }

            // get the amount of money for the interaccount transfer
            valid_money = false;
            transaction_amount = 0;
            MaxPayableAmount = std::round(accounts_vector.at(codename_index_map[payment_acc_codename]).Balance * 100) / 100;
            while (valid_money == false){
                std::cout << "Expense will be paid using " << accounts_vector.at(codename_index_map[payment_acc_codename]).Name << std::endl;;
                std::cout << "Enter the amount of money involved in the transaction"<< std::endl;
                std::cout << "Max. transferrable amounts = " << MaxPayableAmount << std::endl;
                // check if input money is valid
                if (std::cin >> transaction_amount){
                    valid_money = transaction_amount > 0  and std::round(transaction_amount * 100) / 100 <= MaxPayableAmount;
                }
                // clear the input
                std::cin.clear();
                while (std::cin.get() != '\n') ;
                std::cout << std::endl; // empty line for better display in command window 
            }
            // get the account name involved in transaction
            transaction_acc = accounts_vector.at(codename_index_map[payment_acc_codename]).Name;

            // update balance of account involved in transaction
            accounts_vector.at(codename_index_map[payment_acc_codename]).Balance -= transaction_amount;

            // ask the user for category
            category = get_transaction_category(categories_total_map);
            // in case of a new category: initialize with zero
            if (categories_total_map.count(category) == 0){
                categories_total_map[category] = 0;
            }
            // add negative amount to current category
            categories_total_map[category] -= transaction_amount;
        }

        else if (transaction_type == "I"){
            /* INCOME */

            std::cout << "Income selected" << std::endl;
            // get paying account
            valid_payment_acc = false;
            while (valid_payment_acc == false){
                std::cout << "Enter the codename of the payment account for the income" << std::endl;
                // display codenames and their corresponding accounts to user
                for (auto const& map_member: codename_index_map){
                    std::cout << "Type " << map_member.first << " for " << accounts_vector.at(map_member.second).Name << std::endl;
                }
                std::getline(std::cin, payment_acc_codename);
                // check validity of giver account by checking codenames map
                valid_payment_acc = codename_index_map.count(payment_acc_codename) > 0;
            }

            // get the amount of money for the interaccount transfer
            valid_money = false;
            transaction_amount = 0;    
            while (valid_money == false){
                std::cout << "Income will be transferred into " << accounts_vector.at(codename_index_map[payment_acc_codename]).Name  << std::endl;;
                std::cout << "Enter the amount of money involved in the transaction"<< std::endl;
                // check the validity of the input money
                if (std::cin >> transaction_amount){
                    valid_money = transaction_amount > 0;
                }
                // clear the input
                std::cin.clear();
                while (std::cin.get() != '\n') ;
                std::cout << std::endl; // empty line for better display in command window 
            }

            // get the account name involved in transaction
            transaction_acc = accounts_vector.at(codename_index_map[payment_acc_codename]).Name;

            // update balance of account involved in transaction
            accounts_vector.at(codename_index_map[payment_acc_codename]).Balance += transaction_amount;

            // ask the user for category
            category = get_transaction_category(categories_total_map);
            // in case of a new category: initialize with zero
            if (categories_total_map.count(category) == 0){
                categories_total_map[category] = 0;
            }
            // add positive amount to current category
            categories_total_map[category] += transaction_amount;
        }

        // ask the user for detail
        std::cout << "Enter the detail of the transaction: " << std::endl;
        std::getline(std::cin >> std::ws, transaction_detail);

        // get current date
        std::string date_today = get_current_date(get_date_info("day"), get_date_info("month"), get_date_info("year"), "dd/mm/yyyy");

        // write in format: id, type, amount, category, account, detail
        transaction_records_csv << ("#" + std::to_string(transaction_id)) << delimiter << date_today << delimiter << transaction_type_map[transaction_type] 
            << delimiter << transaction_amount << delimiter << category << delimiter << transaction_acc << delimiter << transaction_detail;
        // write: all accounts name with their balance after the transaction
        for (auto account:accounts_vector){
            transaction_records_csv << delimiter << account.Name << delimiter << account.Balance;
        }
        transaction_records_csv << "\n";

        // ask user if they want to input another transaction
        std::cout << "------------TRANSACTION #" << transaction_id << " PROCESSED--------------" << std::endl;
        std::cout << "Press 1 to input another transaction or any other key to end" << std::endl;
        // store user input in an integer
        int next_input;
        std::cin >> next_input;
        // reinput is next input is equal one
        input = next_input == 1;

        // clear the input
        std::cin.clear();
        while (std::cin.get() != '\n') ;
        std::cout << std::endl; // empty line for better display in command window 
        
    }  

    // close transaction records
    transaction_records_csv.close();

    // write monthly categories: rewrite all
    monthly_records_csv.open(GlobalData::FileNames::monthly_categories_csv, std::ios::out);
    for (auto const& map_member:categories_total_map){
        monthly_records_csv << map_member.first << delimiter << map_member.second << "\n";
    }
    // close monthly records csv
    monthly_records_csv.close();

    // update accounts csv data: rewrite
    accounts_csv.open(GlobalData::FileNames::accounts_csv, std::ios::out);
    // initialize map to store asset type balance
    std::map <std::string, double> asset_type_map;
    asset_type_map["Liquid"] = 0;
    asset_type_map["Fixed"] = 0;
    for (auto tempAccount: accounts_vector){
        // write account data in csv
        accounts_csv << tempAccount.Name << delimiter << tempAccount.CodeName << delimiter << tempAccount.AssetType << 
            delimiter << tempAccount.StartBalance << delimiter << tempAccount.Balance << "\n"; 
        // update the balance of each asset type
        asset_type_map[tempAccount.AssetType] += tempAccount.Balance;
    }
    // close accounts csv
    accounts_csv.close();

    // update asset type csv data: rewrite
    asset_type_csv.open(GlobalData::FileNames::asset_type_csv, std::ios::out);
    for (auto const& map_member:asset_type_map){
        asset_type_csv << map_member.first << delimiter << map_member.second << delimiter << map_member.second << "\n";
    }
    // close asset type csv
    asset_type_csv.close();

    // update wealth class csv: rewrite
    wealth_class_csv.open(GlobalData::FileNames::wealth_classes_csv, std::ios::out);
    for (auto tempWealthClass:wealth_classes_vector){
        if (tempWealthClass.Name.find("Fixed") != std::string::npos){
            // fixed asset sum = 100%
            tempWealthClass.Sum = asset_type_map["Fixed"];
        }
        else{
            // liquid asset sum: use percentage allocation
            tempWealthClass.Sum = (tempWealthClass.PercentAllocation / 100.0 ) * asset_type_map["Liquid"];
        }
        // write csv
        wealth_class_csv << tempWealthClass.Name <<  delimiter << tempWealthClass.PercentAllocation 
            << delimiter << tempWealthClass.StartSum << delimiter << tempWealthClass.Sum << "\n"; 
    }
    // close wealth class csv
    wealth_class_csv.close();
    
}