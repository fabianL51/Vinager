#include "Account.h" // for handling Banks classes
#include "utils.h" // utilities functions

/* this script process and categorizes transaction into incomes and expenses. */
/* the transaction histories will be saved in a separate excel data for documentation. */


int main(){

    // initialize fstream for transaction and overview csvs
    std::fstream transaction_records_csv, monthly_records_csv, main_overview_csv;

    // get vectors of accounts, wealth classes, and asset types total from csv file
    std::vector <Account> accounts_vector = get_accounts_vector();
    std::vector <WealthClass> wealth_classes_vector = get_wealth_classes_vector();
    std::map <std::string, double> asset_type_map = get_string_double_map("asset_type");

    // get categories from csv file
    std::map <std::string, double> categories_total_map = get_string_double_map("category");

    // get maps between account names and their codenames
    std::map <std::string, int> codename_index_map = map_codename_to_index(accounts_vector);
    
    // display all accounts
    std::cout << "------------- Current accounts balance ----------" << std::endl;
    for (auto Account: accounts_vector){
        std::cout << Account.Name << "(" << Account.CodeName << ") : " << Account.Balance << std::endl;
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
    int transaction_id; // id that represent current transaction
    std::string category, transaction_acc, transaction_detail; // Category and transaction accounts, transaction detail

    // initialize codenames for all possible transactions
    std::string giver_acc_codename, receiver_acc_codename, payment_acc_codename;

    // initialize variables to process transaction
    std::string transaction_type; // to store transaction type
    // all booleans for validity checks
    bool valid_payment_acc, valid_transaction_type, valid_giver_acc, valid_receiver_acc, valid_money;
    // maximum possible money in the transaction
    double MaxGiverMoney, MaxPayableAmount;


    // loop
    while (input == true){
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
        }

        // ask the user for detail
        std::cout << "Enter the detail of the transaction which will be used as transaction ID " << std::endl;
        std::getline(std::cin >> std::ws, transaction_detail);

    }    
}
