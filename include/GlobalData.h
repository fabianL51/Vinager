#include <iostream>
#include <string>

/* This script creates namespaces of constant parameters */

namespace GlobalData{

    namespace FileNames{
        std::string accounts_csv = "Accounts.csv";
        std::string asset_type_csv = "AssetType.csv";
        
    };

    namespace csv_config{
        char delimiter = ',';
    };
}