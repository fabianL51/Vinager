#include <sys/stat.h> // to check file exists or not
#include <iostream> // basic
#include "Account.h" // for handling Banks classes
#include "WealthClass.h" // for handling Asset classes
#include "GlobalData.h" // for global variables in entire project
#include <fstream> // to handle csv data
#include <sstream> // to read string in csv
#include <math.h> // for math
#include <vector> // for vector
#include <string> // string handles
#include <map> // for mapping
#include <ctime> // to get date and time
#pragma once // prevent errors when some libraries are called in another scripts

inline bool file_exists (const std::string& name) {
    /* This functions check if a file exists 
    source: https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exists-using-standard-c-c11-14-17-c
    answer by PherricOxide and IInspectable    
    */
    struct stat buffer;   
    return (stat (name.c_str(), &buffer) == 0); 
}

std::string ordinal_suffix(int n)
{
    /* this functions returns the ordinal suffix of a real number n
    for example: 1 = 1st, 2 = 2nd, 3 = 3rd, 13 = 13th 
    source: https://stackoverflow.com/questions/21548099/function-for-getting-the-ordinal-of-a-number
    answer by einpoklum   
    */
        std::vector <std::string> suffixes  = {"th", "st", "nd", "rd"};
        auto ord = n % 100;
        if (ord / 10 == 1) { ord = 0; }
        ord = ord % 10;
        if (ord > 3) { ord = 0; }
        return suffixes.at(ord);
}

std::vector <Account> get_accounts_vector(){

    /* this function read csv for accounts and return a vector of Account class */


    // initialize variables
    std::fstream account_csv_fstream;
    std::string line, word; // strings to get a whole line or a word in a line
    std::vector <Account> Account_vector; // account vector to be returned
    Account tempAccount; // temporary account to be added into vector
    char delimiter = GlobalData::csv_config::delimiter; // set delimiter in csv
    int index; // index for storing the right information into right data

    // open csv
    account_csv_fstream.open(GlobalData::FileNames::accounts_csv, std::ios::in);

    if (account_csv_fstream.is_open()) {

        // read csv
        while (std::getline(account_csv_fstream, line)){
            
            // get stringstream from line
            std::stringstream ss(line);

            // set index for storing the right information into right data
            index = 1;
            
            // store the value into tempAccount
            while (std::getline(ss, word, delimiter)){
                switch (index)
                {
                case 1: // index 1 = account name
                    tempAccount.Name = word; 
                    break;

                case 2: // index 2 = codenames
                    tempAccount.CodeName = word; 
                    break;

                case 3: // index 3 = asset type
                    tempAccount.AssetType = word; 
                    break;

                case 4: // index 4 = starting balance
                    tempAccount.StartBalance = std::stod(word); 
                    break;
                
                case 5: // index 5 = balance
                    tempAccount.Balance = std::stod(word); 
                    break;
                }

                // add index by one
                index += 1;
            }
            
            // expand account vector
            Account_vector.emplace_back(tempAccount);

            // clear tempAccount
            tempAccount.clear();
        }

        // close csv
        account_csv_fstream.close();
    }

    // return account vector
    return Account_vector;
}


std::vector <WealthClass> get_wealth_classes_vector(){

    /* this function read csv for wealth classes and return a vector of WealthClass class */


    // initialize variables
    std::fstream wealth_class_csv_fstream;
    std::string line, word; // strings to get a whole line or a word in a line
    std::vector <WealthClass> wealth_classes_vector; // account vector to be returned
    WealthClass tempWealthClass; // temporary account to be added into vector
    char delimiter = GlobalData::csv_config::delimiter; // set delimiter in csv
    int index; // index for storing the right information into right data

    // open csv
    wealth_class_csv_fstream.open(GlobalData::FileNames::wealth_classes_csv, std::ios::in);

    if (wealth_class_csv_fstream.is_open()) {

        // read csv
        while (std::getline(wealth_class_csv_fstream, line)){
            
            // get stringstream from line
            std::stringstream ss(line);

            // set index for storing the right information into right data
            index = 1;
            
            // store the value into tempWealthClass
            while (std::getline(ss, word, delimiter)){
                switch (index)
                {
                case 1: // index 1 = wealth class name
                    tempWealthClass.Name = word; 
                    break;

                case 2: // index 2 = allocation from asset type in percent
                    tempWealthClass.PercentAllocation = std::stod(word); 
                    break;

                case 3: // index 3 = wealth class start sum
                    tempWealthClass.StartSum = std::stod(word); 
                    break;

                case 4: // index 4 = wealth class sum
                    tempWealthClass.Sum = std::stod(word); 
                    break;
                }

                // add index by one
                index += 1;
            }
            
            // expand account vector
            wealth_classes_vector.emplace_back(tempWealthClass);

            // clear tempAccount
            tempWealthClass.clear();
        }

        // close csv
        wealth_class_csv_fstream.close();
    }

    // return account vector
    return wealth_classes_vector;
}

std::map <std::string, double> get_string_double_map(std::string which_data){

    /* this function read string:double map : 
    -csv for asset types and their sum 
    -csv for categories of expenses and incomes and their sum
    */


    // initialize variables
    std::fstream csv_fstream;
    std::string line, word, csv_name; // strings to get a whole line or a word in a line
    std::map <std::string, double>  string_double_map; // account vector to be returned
    char delimiter = GlobalData::csv_config::delimiter; // set delimiter in csv
    int index; // index to check whether it's key or value

    // get csv names by checking which case it is
    if (which_data == "asset_type"){
        csv_name = GlobalData::FileNames::asset_type_csv;
    }
    else if (which_data == "category"){
        csv_name = GlobalData::FileNames::monthly_categories_csv;
    }

    // open csv
    csv_fstream.open(csv_name, std::ios::in);

    if (csv_fstream.is_open()) {

        // read csv
        while (std::getline(csv_fstream, line)){
            
            // get stringstream from line
            std::stringstream ss(line);

            // initialize index
            int index = 0;

            // initialize a string to store temporarily the key name
            std::string key_name;

            // store the value into map
            while (std::getline(ss, word, delimiter)){
                
                // check whether current index is odd or even
                switch (index % 2)
                {
                case 0:
                    // even: get map key
                    key_name = word;
                    break;

                case 1:
                    // odd: set parameter value to last key
                    string_double_map[key_name] = std::stod(word);
                    // clear key name
                    key_name.clear();
                    break;

                }

                // add index by one
                index += 1;                
            }
        }

        // close csv
        csv_fstream.close();
    }
    
    // return account vector
    return string_double_map;
    
}

int get_last_transaction_process_id(){

    /* this function read csv for wealth classes and return a vector of WealthClass class */


    // initialize variables
    std::fstream transaction_process_fstream;
    std::string line, word; // strings to get a whole line or a word in a line
    std::string transaction_id_str; // string to store the information of transaction id 
    int last_transaction_id; // last transaction id to be returned
    char delimiter = GlobalData::csv_config::delimiter; // set delimiter in csv
    int index; // index for storing the right information into right data

    // open csv
    transaction_process_fstream.open(GlobalData::FileNames::transaction_records_csv, std::ios::in);

    if (transaction_process_fstream.is_open()) {

        std::cout << "Searching the last last transaction's ID" << std::endl;

        // read csv
        while (std::getline(transaction_process_fstream, line)){
            
            // get stringstream from line
            std::stringstream ss(line);

            // set index for storing the right information into right data
            index = 1;
            
            // store the value into tempWealthClass
            while (std::getline(ss, word, delimiter)){
                switch (index)
                {
                case 1: // index 1 = wealth class name
                    transaction_id_str = word;
                    break;

                default:
                    break;
                }

                // add index by one
                index += 1;
            }
        }

        // if transaction id is not empty
        if (transaction_id_str.length() > 0){
            // convert last id to integer
            last_transaction_id = std::stoi(transaction_id_str.substr(1, transaction_id_str.length() - 1));
        }
        else {
            // set to zero if empty
            last_transaction_id = 0;
        }
        
        // close csv
        transaction_process_fstream.close();
    }
    else {
        // first time? ;)
        last_transaction_id = 0;
    }

    std::cout << "Last transaction ID is " << last_transaction_id << std::endl;

    // return account vector
    return last_transaction_id;
}

std::map <std::string, int> map_codename_to_index(std::vector <Account> accounts_vector){

    /* This function maps codename to index in current vectors */

    std::map <std::string, int> codename_index_map;
    for (auto Account: accounts_vector){
        // index is the last character in codename minus 1 (first index in vectors is zero) 
        // '1' means int(char - 1)
        codename_index_map[Account.CodeName] = (Account.CodeName[Account.CodeName.length() - 1]) - '1';
    }

    return codename_index_map;

}

int get_date_info(std::string what_info){

    /* This function returns full or part of current date */

    // initialize return variable
    int date_info;

    // current date/time based on current system
    time_t now = time(0);

    tm *ltm = localtime(&now);

    if (what_info == "year"){
       // get current year
        date_info = 1900 + ltm->tm_year;
    }
    else if(what_info == "month"){
        // get current month
        date_info = 1 + ltm->tm_mon;
    }
    else if(what_info == "day"){
        // get current date
        date_info = ltm->tm_mday;
    }

    return date_info;
}

std::string get_current_date (int day, int month, int year, std::string format){

    /* This function build a date based on given format */
    
    // initialize variables
    std::string date, str_day, str_month;

    // apply conversion logic for day and month
    if (day < 10){
        str_day = "0" + std::to_string(day);
    }
    else {
        str_day = std::to_string(day);
    }
    if (month < 10){
        str_month = "0" + std::to_string(month);
    }
    else {
        str_month = std::to_string(month);
    }

    if (format == "dd.mm.yyyy"){
        date = str_day + "." + str_month + "." + std::to_string(year);
    }
    else if (format == "dd/mm/yyyy"){
        date = str_day + "/" + str_month + "/" + std::to_string(year);
    }

    return date;
}

std::string get_string_month(int month){

    /* This function translate integer month to string */

    // create map
    std::string month_str;

    switch (month)
    {
    case 1:
        month_str = "January";
        break;
    
    case 2:
        month_str = "February";
        break;
    
    case 3:
        month_str = "March";
        break;
    
    case 4:
        month_str = "April";
        break;
    
    case 5:
        month_str = "May";
        break;
    
    case 6:
        month_str = "June";
        break;
    
    case 7:
        month_str = "July";
        break;
    
    case 8:
        month_str = "August";
        break;
    
    case 9:
        month_str = "September";
        break;
    
    case 10:
        month_str = "October";
        break;
    
    case 11:
        month_str = "November";
        break;
    
    case 12:
        month_str = "December";
        break;
    
    default:
        break;
    }

    return month_str;
}
std::map <int, std::string> month_int_to_string(int month){

    /* This function creates map that translate integer month to string */

    // create map
    std::map <int, std::string> month_int_to_str_map;

    switch (month)
    {
    case 1:
        month_int_to_str_map[month] = "January";
        break;
    
    case 2:
        month_int_to_str_map[month] = "February";
        break;
    
    case 3:
        month_int_to_str_map[month] = "March";
        break;
    
    case 4:
        month_int_to_str_map[month] = "April";
        break;
    
    case 5:
        month_int_to_str_map[month] = "May";
        break;
    
    case 6:
        month_int_to_str_map[month] = "June";
        break;
    
    case 7:
        month_int_to_str_map[month] = "July";
        break;
    
    case 8:
        month_int_to_str_map[month] = "August";
        break;
    
    case 9:
        month_int_to_str_map[month] = "September";
        break;
    
    case 10:
        month_int_to_str_map[month] = "October";
        break;
    
    case 11:
        month_int_to_str_map[month] = "November";
        break;
    
    case 12:
        month_int_to_str_map[month] = "December";
        break;
    
    default:
        break;
    }

    return month_int_to_str_map;
}
