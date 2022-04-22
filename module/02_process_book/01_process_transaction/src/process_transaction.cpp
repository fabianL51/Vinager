#include "Banks.h" // for handling Banks classes
#include <xlnt/xlnt.hpp> // include xlnt for excel handling
#include "utils.h" // utilities functions

/* this script process and categorizes transaction into incomes and expenses. */
/* the transaction histories will be saved in a separate excel data for documentation. */


int main(){

    // initialize workbooks and worksheets
    xlnt::workbook FinCordsWkb, UtilWkb;
    xlnt::worksheet IncStateWks, AssetsWks, RecordsWks, UtilWks;
    std::string FinCordsWkbName = std::to_string(xlnt::date::today().year) + "_FinancialRecords.xlsx";
    std::string UtilWkbName = "Utilities.xlsx";

    // load workbooks
    FinCordsWkb.load(FinCordsWkbName);
    UtilWkb.load(UtilWkbName);

    // load worksheets
    IncStateWks = FinCordsWkb.sheet_by_title("Income Statement");
    AssetsWks = FinCordsWkb.sheet_by_title("Assets");
    RecordsWks = FinCordsWkb.sheet_by_title("Records");
    UtilWks = UtilWkb.sheet_by_title("Main");

    // get the last row of both accounts and wealth
    int acc_last_row = UtilWks.cell("B4").value<int>() - 1;
    int wealth_current_row = UtilWks.cell("B5").value<int>();
    int wealth_last_row = wealth_current_row - 1;

    // iterate to get bank and wealth class data
    // initialize variables
    std::vector <Bank> BankVec;
    std::vector <Asset> WealthClassVec;
    std::vector <std::string> CodeNames;

    // get accounts and wealth classes
    std::pair<std::vector <Bank>, std::vector <Asset>> bank_wealth_vec = get_accounts_wealth(AssetsWks, acc_last_row, wealth_last_row);
    BankVec = bank_wealth_vec.first;
    WealthClassVec = bank_wealth_vec.second;

    // get accounts codenames
    BankVec = get_account_codenames(BankVec);

    // get maps between account names and their codenames
    std::map <std::string, std::string> acc_name_codename_key, acc_codename_name_key;
    std::pair <std::map <std::string, std::string>, std::map <std::string, std::string>> map_pair = get_acc_name_codenames_map(BankVec);
    acc_name_codename_key = map_pair.first;
    acc_codename_name_key = map_pair.second;

    // create map of account codenames to its corresponding column in Records sheet
    int start_col = xlnt::column_t::column_index_from_string("G"); // start at G column
    std::map <std::string, int> acc_codename_column_key;
    for (int i = start_col; i <= RecordsWks.highest_column().index; i++){
        acc_codename_column_key[acc_name_codename_key[RecordsWks.cell(i,1).value<std::string>()]] = i;
    }

    // display all accounts
    std::cout << "------------- Current accounts balance ----------" << std::endl;
    for (auto Bank: BankVec){
        std::cout << Bank.Name << "(" << Bank.CodeName << ") : " << Bank.Balance << std::endl;
    }

    // display all wealth class
    std::cout << "------------- Current wealth class balance ----------" << std::endl;
    for (auto WealthClass: WealthClassVec){
        std::cout << WealthClass.Name << ": " << WealthClass.Sum << std::endl;
    }

    std::cout << "------------------------------------------------------" << std::endl;

    // ask the user which transactions should be processed
    std::string transaction_type;
    bool valid_transaction_type = false;
    while (valid_transaction_type == false){
        std::cout << "Define a transaction type: " << std::endl;
        std::cout << "Type I for Income, E for Expense and T for interaccount transfers" << std::endl;
        std::getline(std::cin, transaction_type);
        valid_transaction_type = transaction_type == "I" or transaction_type == "E" or transaction_type == "T";
    }

    // get next row in Records sheet by reading from UtilWks
    int new_row = UtilWks.cell("B6").value<int>();

    // process by transaction type
    if (transaction_type == "T"){
        /* INTERACCOUNT TRANSFERS */

        std::cout << "Interaccount transfers selected" << std::endl;
        // get giver account
        bool valid_giver_acc = false;
        std::string giver_acc_codename;
        while (valid_giver_acc == false){
            std::cout << "Enter the codename of the GIVER account" << std::endl;
            for (auto const& map: acc_codename_name_key){
                std::cout << "Type " << map.first << " for " << map.second << std::endl;
            }
            std::getline(std::cin, giver_acc_codename);
            // check validity of giver account by checking codenames map
            valid_giver_acc = acc_codename_name_key.count(giver_acc_codename) > 0;
        }
        
        // get a map for receiver possible accounts: all accounts except giver account
        std::map <std::string, std::string> receiver_key = acc_codename_name_key;
        receiver_key.erase(giver_acc_codename);

        // get receiver account
        bool valid_receiver_acc = false;
        std::string receiver_acc_codename;
        while (valid_receiver_acc == false){
            std::cout << "Enter the codename of the RECEIVER account" << std::endl;
            for (auto const& map: receiver_key){
                std::cout << "Type " << map.first << " for " << map.second << std::endl;
            }
            std::getline(std::cin, receiver_acc_codename);
            // check validity of giver account by checking codenames map
            valid_receiver_acc = receiver_key.count(receiver_acc_codename) > 0;
        }

        // get the amount of money for the interaccount transfer
        bool valid_money = false;
        double transfer_amount;
        double MaxGiverMoney = std::round(RecordsWks.cell(acc_codename_column_key[giver_acc_codename], new_row - 1).value<double>() * 100) / 100;
        while (valid_money == false){
            std::cout << "Enter the amount of money to be transferred from " << acc_codename_name_key[giver_acc_codename];
            std::cout << " to " << acc_codename_name_key[receiver_acc_codename] << std::endl;
            std::cout << "Max. transferrable amounts = " << MaxGiverMoney << std::endl;
            if (std::cin >> transfer_amount){
                valid_money = transfer_amount > 0  and std::round(transfer_amount * 100) / 100 <= MaxGiverMoney;
            }
            // clear the input
            std::cin.clear();
            while (std::cin.get() != '\n') ;
            std::cout << std::endl; // empty line for better display in command window 
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
        RecordsWks.cell("B", new_row).value(transfer_amount);
        RecordsWks.cell("C", new_row).value(transfer_amount);
        // column D: category is empty
        // column E: Detail
        RecordsWks.cell("E", new_row).value(Detail);
        // column F: Account for Transaction
        std::string transaction_acc = acc_codename_name_key[giver_acc_codename] + " to " + acc_codename_name_key[receiver_acc_codename];
        RecordsWks.cell("F", new_row).value(transaction_acc);
        // reduce money from giver and add the same amount to giver
        RecordsWks.cell(acc_codename_column_key[giver_acc_codename], new_row).value(MaxGiverMoney - transfer_amount);
        double ReceiverInitialMoney = std::round(RecordsWks.cell(acc_codename_column_key[receiver_acc_codename], new_row - 1).value<double>() * 100) / 100;
        RecordsWks.cell(acc_codename_column_key[receiver_acc_codename], new_row).value(ReceiverInitialMoney + transfer_amount);

    }

    // save workbooks
    FinCordsWkb.save(FinCordsWkbName);

    
}
