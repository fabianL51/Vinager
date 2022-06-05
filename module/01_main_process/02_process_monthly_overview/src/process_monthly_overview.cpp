#include "Account.h" // for handling Banks classes
#include "utils.h" // utilities functions
#include <algorithm>  // for std::sort

/* this script processes monthly overview for current month based on category
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

    // initialize maps for income and expenses
    std::vector <std::pair <std::string, double>> income_list, expense_list;

    // iterate monthly categories to get incomes and expenses
    for (auto const& map_member:categories_total_map){
        if (map_member.second > 0){
            // income
            income_list.emplace_back(std::make_pair(map_member.first, map_member.second));
        }
        else if (map_member.second <0){
            // expense
            expense_list.emplace_back(std::make_pair(map_member.first, map_member.second));
        }
    }

    // sort income from highest to lowest
    std::sort(income_list.begin(), income_list.end(), [](const std::pair<std::string, double> &ego, const std::pair<std::string, double> &comparison) {
    return ego.second > comparison.second; });

    // sort expenses from lowest to highest (negative amount of money)
    std::sort(expense_list.begin(), expense_list.end(), [](const std::pair<std::string, double> &ego, const std::pair<std::string, double> &comparison) {
    return ego.second < comparison.second; });

    // open current month overview csv
    current_month_overview_csv.open(month_overview_filename, std::ios::out);

    
    // header for current month
    current_month_overview_csv << get_string_month(get_date_info("month")) + " " +  std::to_string(get_date_info("year")) << "\n" << "--------------------------------" << "\n";   
    
    // save income into csv
    // header for income
    current_month_overview_csv << "\n" << "INCOME" << "\n" << "--------------------------------" << "\n";  
    // iterate and input all income
    for (auto const& income:income_list){
        current_month_overview_csv << income.first << delimiter << income.second << "\n";
    }

    // save expenses into csv
    // header for income
    current_month_overview_csv << "\n" << "EXPENSE" << "\n" << "--------------------------------" << "\n";   
    // iterate and input all income
    for (auto const& expense:expense_list){
        current_month_overview_csv << expense.first << delimiter << expense.second << "\n";
    }

    // get change in accounts' balances
    // header
    current_month_overview_csv << "\n" << "CHANGE IN ACCOUNT BALANCE" << "\n" << "--------------------------------" << "\n";
    // get asset type change through a map
    std::map <std::string, double> asset_type_balance_change_map;
    asset_type_balance_change_map["Liquid"] = 0;
    asset_type_balance_change_map["Fixed"] = 0;
    // iterate all accounts
    for (auto const account:accounts_vector){
        // get change in balance
        current_month_overview_csv << account.Name << delimiter << account.Balance - account.StartBalance << "\n";
        // get change in asset type
        asset_type_balance_change_map[account.AssetType] += account.Balance - account.StartBalance;
    }

    // get change in wealth class balances
    // header
    current_month_overview_csv << "\n" <<  "CHANGE IN WEALTH CLASS BALANCE" << "\n" << "--------------------------------" << "\n";
    // iterate and get change in balance
    for (auto const wealth_class:wealth_classes_vector){
        current_month_overview_csv << wealth_class.Name << delimiter << wealth_class.Sum - wealth_class.StartSum << "\n";
    }

    // get change in asset type balances
    // header
    current_month_overview_csv << "\n" <<  "CHANGE IN ASSET TYPE BALANCE" << "\n" << "--------------------------------" << "\n";
    // iterate and get change in balance
    for (auto const& map_member:asset_type_balance_change_map){
        current_month_overview_csv << map_member.first << delimiter << map_member.second << "\n";
    }

    // close csv
    current_month_overview_csv.close();
    
}