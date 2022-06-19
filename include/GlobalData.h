#include <iostream>
#include <string>
#pragma once // prevent errors when some libraries are called in another scripts

/* This script creates namespaces of constant parameters */

namespace GlobalData{

    namespace FileNames{
        std::string accounts_csv = "Accounts.csv";
        std::string asset_type_csv = "AssetType.csv";
        std::string wealth_classes_csv = "WealthClasses.csv";
        std::string transaction_records_csv = "TransactionRecords.csv";
        std::string monthly_categories_csv = "MonthlyCategories.csv";
        std::string recurring_csv = "Recurring.csv";
        
    };

    namespace csv_config{
        char delimiter = ',';
    };
}