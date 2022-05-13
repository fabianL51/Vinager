#include "Account.h" // for handling Banks classes
#include "WealthClass.h" // for handling Asset classes
#include <xlnt/xlnt.hpp> // include xlnt for excel handling
#include "utils.h" // utilities functions

/* this script process and categorizes transaction into incomes and expenses. */
/* the transaction histories will be saved in a separate excel data for documentation. */


int main(){

    // initialize fstream for transaction and overview csvs
    std::fstream transaction_records_csv, monthly_records_csv, main_overview_csv;

    // get vectors of accounts, wealth classes, and asset types total from csv file
    std::vector <Account> accounts_vector = get_accounts_vector();
    std::vector <WealthClass> wealth_classes_vector = get_wealth_classes_vector();
    std::map <std::string, double> asset_type_map = get_assets();


    // get maps between account names and their codenames
    std::map <std::string, int> codename_index_map = map_codename_to_index(accounts_vector);
    for (auto const& map_member: codename_index_map){
        std::cout << map_member.first << " : " << map_member.second << std::endl;
    }
    
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

    // loop
    while (input == true){
         // ask the user which transactions should be processed
        std::string transaction_type;
        bool valid_transaction_type = false;
        while (valid_transaction_type == false){
            std::cout << "Define a transaction type: " << std::endl;
            std::cout << "Type I for Income, E for Expense and T for interaccount transfers" << std::endl;
            std::getline(std::cin, transaction_type);
            valid_transaction_type = transaction_type == "I" or transaction_type == "E" or transaction_type == "T";
        }

        // initialize all variables used by all transactions
        double money_in, money_out;
        std::string Category, transaction_acc, giver_acc_codename, receiver_acc_codename, payment_acc_codename;

        // process by transaction type
        if (transaction_type == "T"){
            /* INTERACCOUNT TRANSFERS */

            std::cout << "Interaccount transfers selected" << std::endl;
            // get giver account
            bool valid_giver_acc = false; // boolean to check the input validity
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
            bool valid_receiver_acc = false; // boolean to check the input validity
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
            bool valid_money = false; // boolean to check the money input
            double MaxGiverMoney = std::round(accounts_vector.at(codename_index_map[giver_acc_codename]).Balance * 100) / 100; // round two decimals
            while (valid_money == false){
                std::cout << "Enter the amount of money to be transferred from " << accounts_vector.at(codename_index_map[giver_acc_codename]).Name;
                std::cout << " to " << accounts_vector.at(codename_index_map[receiver_acc_codename]).Balance << std::endl;
                std::cout << "Max. transferrable amounts = " << MaxGiverMoney << std::endl;
                if (std::cin >> money_in){
                    valid_money = money_in > 0  and std::round(money_in * 100) / 100 <= MaxGiverMoney;
                }
                // clear the input
                std::cin.clear();
                while (std::cin.get() != '\n') ;
                std::cout << std::endl; // empty line for better display in command window 
            }
            // for interaccount transfer: money_in = money_out
            money_out = money_in;   

            // store transfer flow in transaction account
            transaction_acc = acc_codename_name_key[giver_acc_codename] + " to " + acc_codename_name_key[receiver_acc_codename];
        }
        else if (transaction_type == "E"){
            /* EXPENSES */

            std::cout << "Expenses selected" << std::endl;
            // get paying account
            bool valid_payment_acc = false;
            while (valid_payment_acc == false){
                std::cout << "Enter the codename of the payment account for the expense" << std::endl;
                for (auto const& map: acc_codename_name_key){
                    std::cout << "Type " << map.first << " for " << map.second << std::endl;
                }
                std::getline(std::cin, payment_acc_codename);
                // check validity of giver account by checking codenames map
                valid_payment_acc = acc_codename_name_key.count(payment_acc_codename) > 0;
            }

            // get the amount of money for the interaccount transfer
            bool valid_money = false;
            money_in = 0;
            double MaxPayableAmount = std::round(RecordsWks.cell(acc_codename_column_RecordsKey[payment_acc_codename], new_row - 1).value<double>() * 100) / 100;
            while (valid_money == false){
                std::cout << "Expense will be paid using " << acc_codename_name_key[payment_acc_codename] << std::endl;;
                std::cout << "Enter the amount of money involved in the transaction"<< std::endl;
                std::cout << "Max. transferrable amounts = " << MaxPayableAmount << std::endl;
                if (std::cin >> money_out){
                    valid_money = money_out > 0  and std::round(money_out * 100) / 100 <= MaxPayableAmount;
                }
                // clear the input
                std::cin.clear();
                while (std::cin.get() != '\n') ;
                std::cout << std::endl; // empty line for better display in command window 
            }
            // get the account name involved in transaction
            transaction_acc = acc_codename_name_key[payment_acc_codename];
        }

        else if (transaction_type == "I"){
            /* INCOME */

            std::cout << "Income selected" << std::endl;
            // get paying account
            bool valid_payment_acc = false;
            while (valid_payment_acc == false){
                std::cout << "Enter the codename of the payment account for the income" << std::endl;
                for (auto const& map: acc_codename_name_key){
                    std::cout << "Type " << map.first << " for " << map.second << std::endl;
                }
                std::getline(std::cin, payment_acc_codename);
                // check validity of giver account by checking codenames map
                valid_payment_acc = acc_codename_name_key.count(payment_acc_codename) > 0;
            }

            // get the amount of money for the interaccount transfer
            bool valid_money = false;
            money_out = 0;    
            while (valid_money == false){
                std::cout << "Income will be transferred into " << acc_codename_name_key[payment_acc_codename] << std::endl;;
                std::cout << "Enter the amount of money involved in the transaction"<< std::endl;
                if (std::cin >> money_in){
                    valid_money = money_in > 0;
                }
                // clear the input
                std::cin.clear();
                while (std::cin.get() != '\n') ;
                std::cout << std::endl; // empty line for better display in command window 
            }

            // get the account name involved in transaction
            transaction_acc = acc_codename_name_key[payment_acc_codename];
        }
    }

   

    // ask the user for detail
    bool valid_detail = false;
    std::string Detail;
    std::cout << "Enter the detail of the transaction which will be used as transaction ID " << std::endl;
    std::getline(std::cin >> std::ws, Detail);

    // input transaction into Records sheet
    // column A: date
    RecordsWks.cell("A", new_row).value(xlnt::date::today());
    // column B and C: amount of money
    if (money_in > 0){
        RecordsWks.cell("B", new_row).value(money_in);
    }
    if (money_out > 0){
        RecordsWks.cell("C", new_row).value(money_out);
    }
    
    // column D: category
    if (!Category.empty()){
        RecordsWks.cell("D", new_row).value(Category);
    }
    // column E: Detail
    RecordsWks.cell("E", new_row).value(Detail);
    // column F: Account for Transaction
    RecordsWks.cell("F", new_row).value(transaction_acc);
    // update amount of money of all accounts
    for (auto Bank: BankVec){

        if (Bank.CodeName == receiver_acc_codename or (Bank.CodeName == payment_acc_codename and transaction_type == "I")){
            Bank.Balance += money_in;
        }
        else if (Bank.CodeName == giver_acc_codename or (Bank.CodeName == payment_acc_codename and transaction_type == "E")){
            Bank.Balance -= money_out;
        }

        // update balance in Records sheet
        RecordsWks.cell(acc_codename_column_RecordsKey[Bank.CodeName], new_row).value(Bank.Balance);

        // update current balance in Assets sheet: column D is current Balance
        AssetsWks.cell("D", acc_name_row_AssetsKey[Bank.Name]).value(Bank.Balance);
    }
    // save workbooks
    FinCordsWkb.save(FinCordsWkbName);

    
}
