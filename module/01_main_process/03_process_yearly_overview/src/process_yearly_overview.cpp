#include "Account.h" // for handling Banks classes
#include "utils.h" // utilities functions
#include <algorithm>  // for std::sort

/* this script processes data from monthly overviews to create the yearly overview. */
/* the transaction histories will be saved in a separate excel data for documentation. */

/* Workflow : create overview for current year
1. get expenses and incomes for all months
2. get changes in accounts, wealth classes, and asset types
*/

std::vector <std::map <std::pair <std::string, int>, double>> get_monthly_expense_income_map (){

    /* This function returns maps containing data to create the yearly overview
    expense and income map: Key: <category_name, month_integer>, Value: category total
    */

    // initialize variables
    // initialize all maps
    std::map <std::pair <std::string, int>, double> expense_map, income_map, acc_bal_changes_map, weacla_bal_changes_map, astype_balance_changes_map;
    // filename for current month overview
    std::string current_month_overview_filename;
    // fstream for current month's overview csv
    std::fstream current_month_overview_csv;

    // iterate all months
    for (int month_int = 1; month_int <= 12; month_int ++){
        
        // get filename for current month
        current_month_overview_filename = get_string_month(month_int) + "_" + std::to_string(get_date_info("year")) + ".csv";
        // initialize delimiter
        char delimiter = GlobalData::csv_config::delimiter;

        // check if csv for current month exists
        if (file_exists(current_month_overview_filename)){
            // open csv
            current_month_overview_csv.open(current_month_overview_filename, std::ios::in);

            // initialize int to store current row in csv
            int curRow = 0;
            // initialize int to store word index in a line
            int index;
            // initialize string to store current line and a string to get word in a line
            std::string line, word;
            // initialize temporary key either as string or as a pair
            std::string key_string;
            std::pair<std::string, int> key_pair;


            // iterate all headers 6th row
            while (std::getline(current_month_overview_csv, line)){
                // add current row by one
                curRow++;
                // 6th row: income data
                if (curRow >= 6){
                    // get stringstream from line
                    std::stringstream ss(line);
                    // set index by zero
                    index = 0;
                    // read current line 
                    while (std::getline(ss, word, delimiter)){
                        switch (index)
                        {
                        case 1: // index 1 = income name to make key pair
                            // make key pair
                            key_pair = std::make_pair(word, month_int);
                            break;

                        case 2: // index 2 = income value:
                            // add income to income map
                            income_map.insert(std::make_pair(key_pair, std::stod(word)));
                            break;
                        }
                        // add index by one
                        index += 1;
                    }
                }
            }
            
        }
        else{
            
        }

    }
}



int main(){

    // initialize fstream for yearly overview csv
    std::fstream current_year_overview_csv;
    // forge filename for current year overview
    std::string year_overview_filename = "Overview_" + std::to_string(get_date_info("year")) + ".csv";
    // initialize delimiter
    char delimiter = GlobalData::csv_config::delimiter;

    
    
}