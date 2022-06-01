#include "Account.h" // for handling Banks classes
#include "utils.h" // utilities functions

/* this script processes an overview for current month based on category
process and categorizes transaction into incomes and expenses. */
/* the transaction histories will be saved in a separate excel data for documentation. */

/* Workflow : create overview for current month
1. iterating all categories to show income and expense
2. get change in balances for both accounts and wealth classes
3. get change in asset type during accounts iteration
*/

int main(){

    // initialize fstream for transaction and overview csvs
    std::fstream monthly_records_csv, accounts_csv, asset_type_csv, wealth_class_csv, current_month_overview_csv;
    // forge filename for current month overview
    std::string month_overview_filename = get_string_month(get_date_info("month")) + "_" + std::to_string(get_date_info("year")) + ".csv";
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
    
}