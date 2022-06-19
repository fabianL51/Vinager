#include "utils.h" // utilities functions
#include <algorithm>  // for std::sort

/* this script processes data from monthly overviews to create the yearly overview. */
/* the transaction histories will be saved in a separate excel data for documentation. */

/* Workflow : create overview for current year
1. get expenses and incomes for all months
2. get changes in accounts, wealth classes, and asset types for all months
3. create yearly overview
*/

std::pair <std::vector <std::map <std::pair <std::string, int>, double>>, std::vector <std::map <std::string, double>>> process_monthly_overviews_data (std::vector <std::map <std::string, double>> total_map_vector){

    /* This function returns maps containing data to create the yearly overview
    expense and income map: Key: <category_name, month_integer>, Value: category total
    */

    // initialize variables
    // initialize vector containing maps
    std::vector <std::map <std::pair <std::string, int>, double>> map_vector(5);
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
        // std::cout << current_month_overview_filename;
        
        // check if csv for current month exists
        if (file_exists(current_month_overview_filename)){

            // std::cout << " exists!" << std::endl;
            // open csv
            current_month_overview_csv.open(current_month_overview_filename, std::ios::in);

            // initialize int to store current row in csv
            int curRow = 0;
            // initialize int to store all indeces
            int index;
            // set start values for all indeces, otherwise it will be set as zero and causes error in the reading logic
            int start_income_index = 6;
            int start_expense_index = 6;
            int start_account_change_index = 6;
            int start_wealth_class_change_index = 6;
            int start_asset_type_index = 6;
            // initialize vector index
            int vector_index = 0;
            // initialize booleans to store which data is currently read
            bool income_read = true; 
            bool expense_read= true; 
            bool account_change_read = true; 
            bool wealth_class_change_read = true; 
            bool asset_type_change_read = true;
            // initialize string to store current line and a string to get word in a line
            std::string line, word;
            // initialize temporary key either as string or as a pair
            std::string key_string;
            std::pair<std::string, int> key_pair;

            // iterate all headers 6th row
            while (std::getline(current_month_overview_csv, line)){
                // add current row by one
                curRow++;

                // income reading begins at 6th row
                if (curRow >= start_income_index and income_read == true){

                    
                    if (line.empty()){
                        // current line is empty: income data done
                        income_read = false;
                        // expense data begins 3 rows after current row
                        start_expense_index = curRow + 3;
                        // add vector index by one
                        vector_index++;
                    }

                    else{
                        // current line is not empty: then read income data
                        // get stringstream from line
                        std::stringstream ss(line);
                        // set index by zero
                        index = 1;
                        // read current line 
                        while (std::getline(ss, word, delimiter)){
                            switch (index)
                            {
                            case 1: // index 1 = income name to make key pair
                                // make key pair
                                key_pair = std::make_pair(word, month_int);
                                // get single key
                                key_string = word;
                                break;

                            case 2: // index 2 = income value:
                                // add income to income map
                                map_vector.at(vector_index).insert(std::make_pair(key_pair, std::stod(word)));
                                // add to total map vector
                                total_map_vector.at(vector_index)[key_string] += std::stod(word);
                                break;
                            }
                            // add index by one
                            index += 1;
                        }

                    }
                }

                // expense reading
                else if(curRow >= start_expense_index and expense_read == true){
                    if (line.empty()){
                        // current line is empty: expense data done
                        expense_read = false;
                        // account balance change data begins 3 rows after current row
                        start_account_change_index = curRow + 3;
                        // add vector index by one
                        vector_index++;
                    }

                    else{
                        // current line is not empty: then read expense data
                        // get stringstream from line
                        std::stringstream ss(line);
                        // set index by zero
                        index = 1;
                        // read current line 
                        while (std::getline(ss, word, delimiter)){
                            switch (index)
                            {
                            case 1: // index 1 = expense name to make key pair
                                // make key pair
                                key_pair = std::make_pair(word, month_int);
                                // get single key
                                key_string = word;
                                break;

                            case 2: // index 2 = expense value:
                                // add expense to expense map
                                map_vector.at(vector_index).insert(std::make_pair(key_pair, std::stod(word)));
                                // add to total map vector
                                total_map_vector.at(vector_index)[key_string] += std::stod(word);
                                break;
                            }
                            // add index by one
                            index += 1;
                        }
                    }
                }

                // account balance change reading
                else if(curRow >= start_account_change_index and account_change_read == true){
                    if (line.empty()){
                        // current line is empty: account balance change data done
                        account_change_read = false;
                        // wealth class balance change data begins 3 rows after current row
                        start_wealth_class_change_index = curRow + 3;
                        // add vector index by one
                        vector_index++;
                    }

                    else{
                        // current line is not empty: then read account balance change data
                        // get stringstream from line
                        std::stringstream ss(line);
                        // set index by zero
                        index = 1;
                        // read current line 
                        while (std::getline(ss, word, delimiter)){
                            switch (index)
                            {
                            case 1: // index 1 = account name to make key pair
                                // make key pair
                                key_pair = std::make_pair(word, month_int);
                                // get single key
                                key_string = word;
                                break;

                            case 2: // index 2 = account balance change value:
                                // add change to account balance change map
                                map_vector.at(vector_index).insert(std::make_pair(key_pair, std::stod(word)));
                                // add to total map vector
                                total_map_vector.at(vector_index)[key_string] += std::stod(word);
                                break;
                            }
                            // add index by one
                            index += 1;
                        }
                    }
                }

                // wealth class balance change reading
                else if(curRow >= start_wealth_class_change_index and wealth_class_change_read == true){
                    if (line.empty()){
                        // current line is empty: wealth class balance change data done
                        wealth_class_change_read = false;
                        // asset type balance change data begins 3 rows after current row
                        start_asset_type_index = curRow + 3;
                        // add vector index by one
                        vector_index++;
                    }

                    else{
                        // current line is not empty: then read wealth class balance change data
                        // get stringstream from line
                        std::stringstream ss(line);
                        // set index by zero
                        index = 1;
                        // read current line 
                        while (std::getline(ss, word, delimiter)){
                            switch (index)
                            {
                            case 1: // index 1 = wealth class name to make key pair
                                // make key pair
                                key_pair = std::make_pair(word, month_int);
                                // get single key
                                key_string = word;
                                break;

                            case 2: // index 2 = wealth class balance change value:
                                // add change to wealth class balance change map
                                map_vector.at(vector_index).insert(std::make_pair(key_pair, std::stod(word)));
                                // add to total map vector
                                total_map_vector.at(vector_index)[key_string] += std::stod(word);
                                break;
                            }
                            // add index by one
                            index += 1;
                        }
                    }
                }

                // asset type balance change reading
                else if(curRow >= start_asset_type_index and asset_type_change_read == true){
                    if (not line.empty()){
                        // current line is not empty: then read asset type balance change data
                        // get stringstream from line
                        std::stringstream ss(line);
                        // set index by zero
                        index = 1;
                        // read current line 
                        while (std::getline(ss, word, delimiter)){
                            switch (index)
                            {
                            case 1: // index 1 = wealth class name to make key pair
                                // make key pair
                                key_pair = std::make_pair(word, month_int);
                                // get single key
                                key_string = word;
                                break;

                            case 2: // index 2 = wealth class balance change value:
                                // add change to wealth class balance change map
                                map_vector.at(vector_index).insert(std::make_pair(key_pair, std::stod(word)));
                                // add to total map vector
                                total_map_vector.at(vector_index)[key_string] += std::stod(word);
                                break;
                            }
                            // add index by one
                            index += 1;
                        }
                    }
                }
            }
            // std::cout << "index_vector is " << vector_index << std::endl;
            // for (auto const& map_member:total_map_vector.at(0)){
            //     std::cout << map_member.first << " :" << map_member.second << std::endl;
            // }
        }
        else{
            // if csv can't be found
            // std::cout << " doesn't exist." << std::endl;
        }

    }
    return std::make_pair(map_vector, total_map_vector);
}



int main(){

    // initialize fstream for yearly overview csv
    std::fstream current_year_overview_csv;
    // forge filename for current year overview
    std::string year_overview_filename = "Overview_" + std::to_string(get_date_info("year")) + ".csv";
    // initialize delimiter
    char delimiter = GlobalData::csv_config::delimiter;
    // initialize maps to store yearly totals
    std::vector <std::map <std::string, double>> total_map_vector(5);
    // get all data from monthly overviews and store in map_vector
    std::pair <std::vector <std::map <std::pair <std::string, int>, double>>, std::vector <std::map <std::string, double>>> map_vector_pair = 
        process_monthly_overviews_data (total_map_vector);
    // unload results
    std::vector <std::map <std::pair <std::string, int>, double>> map_vector = map_vector_pair.first;
    total_map_vector = map_vector_pair.second;

    // get total of total for each data
    std::vector<double> total_vector(5);
    for (int i = 0; i < total_map_vector.size(); i++){
        // iterate each component to get total
        for (auto const& map_member: total_map_vector.at(i)){
            total_vector.at(i) += map_member.second;
        }
    }
    
    // open or create yearly csv with write permission
    current_year_overview_csv.open(year_overview_filename, std::ios::out);
    // header for current year
    current_year_overview_csv << "YEARLY OVERVIEW FOR " << std::to_string(get_date_info("year")) << "\n" << "-----------------------------------------------------" << "\n";
    // get all totals
    std::string total_detail;
    for (int i = 0; i < total_vector.size(); i++){
        switch(i){
            case 0:
                total_detail = "TOTAL INCOMES ";
                break;
            case 1:
                total_detail = "TOTAL EXPENSES ";
                break;
            case 2:
                total_detail = "TOTAL CHANGE IN ACCOUNT ";
                break;
            case 3:
                total_detail = "TOTAL CHANGE IN WEALTH CLASSES ";
                break;
            case 4:
                total_detail = "TOTAL CHANGE IN ASSET TYPE ";
                break;
        }
        current_year_overview_csv << total_detail << delimiter << total_vector.at(i) << "\n";
    }
    current_year_overview_csv << "-----------------------------------------------------" << "\n";
    

    // initialize variables for header details
    std::string header_top, header_down;
    // use loop to display all necessary data
    for (int i = 0; i < map_vector.size(); i++){
        // use switch case to get header data
        switch (i){
            case 0: // index 0 = income
                header_top = "INCOME";
                header_down = "Category";
                break;
        
            case 1: // index 1 = expense
                header_top = "EXPENSE";
                header_down = "Category";
                break;
            
            case 2: // index 2 = account balance change
                header_top = "CHANGE IN ACCOUNT BALANCE";
                header_down = "Account";
                break;
            
            case 3: // index 3 = wealth class balance change
                header_top = "CHANGE IN WEALTH CLASS BALANCE";
                header_down = "Wealth Class";  
                break;
            
            case 4: // index 34= asset type balance change
                header_top = "CHANGE IN ASSET TYPE BALANCE";
                header_down = "Asset Type";      
                break;

        }
        // display header data in csv
        current_year_overview_csv << "\n" << header_top << "\n" << "-----------------------------------------------------" << "\n";
        current_year_overview_csv << header_down;  
        for (int i = 1; i <= 12; i++){
            current_year_overview_csv << delimiter << get_string_month(i) ;
        }
        current_year_overview_csv << delimiter << "TOTAL" << "\n";
        
        // save data in csv
        // iterate all key to be displayed
        for (auto const& map_member:total_map_vector.at(i)){
            // get current category
            current_year_overview_csv << map_member.first;
            // iterate month
            for (int j = 1; j <= 12; j++){
                // display monthly details in csv
                current_year_overview_csv << delimiter << map_vector.at(i)[std::make_pair(map_member.first, j)];
            }
            // display total 
            current_year_overview_csv << delimiter << map_member.second << "\n";
        }
        current_year_overview_csv << "-----------------------------------------------------" << "\n";
    }


    // close csv
    current_year_overview_csv.close();
}