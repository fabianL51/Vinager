#include "utils.h" // utilities functions
#include <algorithm>  // for std::sort

/* this script manages and processes recurring transactions */

/* Workflow : manage recurring transaction
1. get transaction type: 
2. get name
2. get category
3. get amount of money
4. get paying account (and target account for interaccount)
5. get due date
*/


int main(){

    // initialize fstream
    std::fstream recurring_csv;
    // initialize delimiter
    char delimiter = GlobalData::csv_config::delimiter;

    // get all accounts data
    std::vector <Account> accounts_vector = get_accounts_vector();
    // get maps between account names and their codenames
    std::map <std::string, int> codename_index_map = map_codename_to_index(accounts_vector);
    // create transaction type map
    std::map <std::string, std::string> transaction_type_map;
    transaction_type_map["T"] = "Interaccount";
    transaction_type_map["E"] = "Expense";
    transaction_type_map["I"] = "Income";

    // booleans
    bool loop_process = true; // boolean to keep program intact
    bool valid_transaction_type; // boolean to end loop for asking transaction type
    bool valid_money; // boolean to check the validity of amount of money involved
    bool valid_payment_acc, valid_target_account; // boolean to check the validity of the accounts for payment and target
    bool valid_due; // boolean to check the validity of due date
    // integer to get user's wish
    int user_wish;
    // strings
    std::string transaction_type_code;
    std::string payment_codename, target_codename; // store temporary account username
    // recurring transactions
    RecTrans recurring_transaction;
    
    while (loop_process == true){
        // integer to get user's wish
        std::cout << "Press 1 to add a new recurring expense or income" << "\n" << "Press 2 to manage created recurring expense or income" << "\n"
            << "Press any other key to end process" << std::endl;
        std::cin >> user_wish;

        if (user_wish == 1){
            // add recurring transaction
            // write reccuring transaction by append
            recurring_csv.open(GlobalData::FileNames::recurring_csv, std::ios::app);

            // get transaction type
            // ask the user which transactions should be processed
            valid_transaction_type = false; // set valid transaction to false
            while (valid_transaction_type == false){
                // clear the input
                std::cin.clear();
                while (std::cin.get() != '\n') ;
                std::cout << std::endl; // empty line for better display in command window 

                std::cout << "Define a transaction type: " << std::endl;
                std::cout << "Type I for Income, E for Expense and T for interaccount transfers" << std::endl;
                std::getline(std::cin, transaction_type_code);
                valid_transaction_type = transaction_type_code == "I" or transaction_type_code == "E" or transaction_type_code == "T";
            }
            recurring_transaction.Type = transaction_type_map[transaction_type_code];
            
            // get transaction name
            std::cout << "Enter the name of the transaction: " << std::endl;
            std::getline(std::cin >> std::ws, recurring_transaction.Name);

            // get transaction category
            if (transaction_type_code == "T"){
                recurring_transaction.Category = recurring_transaction.Type;
            }
            else{
                 std::cout << "Enter the category of the transaction: " << std::endl;
                std::getline(std::cin >> std::ws, recurring_transaction.Category);
            }

            // get amount of money
            valid_money = false; 
            while (valid_money == false){
                std::cout << "Enter the amount of money involved in the transaction"<< std::endl;
                // check the validity of the input money
                if (std::cin >> recurring_transaction.AmountMoney){
                    valid_money = recurring_transaction.AmountMoney > 0;
                }
                // clear the input
                std::cin.clear();
                while (std::cin.get() != '\n') ;
                std::cout << std::endl; // empty line for better display in command window 
            }


            // get paying account
            valid_payment_acc = false;
            while (valid_payment_acc == false){
                std::cout << "Enter the codename of the payment account " << std::endl;
                // display codenames and their corresponding accounts to user
                for (auto const& map_member: codename_index_map){
                    std::cout << "Type " << map_member.first << " for " << accounts_vector.at(map_member.second).Name << std::endl;
                }
                std::getline(std::cin, payment_codename);
                // check validity of giver account by checking codenames map
                valid_payment_acc = codename_index_map.count(payment_codename) > 0;
            }
            recurring_transaction.PayAccount = accounts_vector.at(codename_index_map[payment_codename]).Name;

            // get target account for interaccount
            if (transaction_type_code == "T"){
                valid_target_account = false;
                while (valid_target_account == false){
                    std::cout << "Enter the codename of the target account " << std::endl;
                    // display codenames and their corresponding accounts to user
                    for (auto const& map_member: codename_index_map){
                        // leave giver account out from loop
                        if (map_member.first != payment_codename){
                            std::cout << "Type " << map_member.first << " for " << accounts_vector.at(map_member.second).Name << std::endl;
                        }
                    }
                    std::getline(std::cin, target_codename);
                    // check validity of giver account by checking codenames map
                    valid_target_account = codename_index_map.count(target_codename) > 0 and payment_codename != target_codename;
                }
                recurring_transaction.TargetAccount = accounts_vector.at(codename_index_map[target_codename]).Name;
            }
            else{
                recurring_transaction.TargetAccount = "NONE";
            }

            // get due date
            valid_due = false; 
            while (valid_due == false){
                std::cout << "Enter the due date of the transaction (between 1 and 28) "<< std::endl;
                // check the validity of the input money
                if (std::cin >> recurring_transaction.DueDate){
                    valid_due = recurring_transaction.DueDate > 0 and recurring_transaction.DueDate < 29;
                }
                // clear the input
                std::cin.clear();
                while (std::cin.get() != '\n') ;
                std::cout << std::endl; // empty line for better display in command window 
            }

            // set status to DUE
            recurring_transaction.Status = "DUE";

            // write in csv
            recurring_csv << recurring_transaction.Type << delimiter << recurring_transaction.Name << delimiter << recurring_transaction.Category
                << delimiter << recurring_transaction.AmountMoney << delimiter << recurring_transaction.PayAccount << delimiter << recurring_transaction.TargetAccount
                << delimiter << recurring_transaction.DueDate << delimiter << recurring_transaction.Status << "\n";
            recurring_csv.close();

            // reset recurring transaction
            recurring_transaction.clear();
        }

        else if (user_wish == 2){
            // manage or delete recurring transaction

            // read recurring transaction in utils.h
        }
        else{
            // end loop
            loop_process = false;
        }
    }
    

}