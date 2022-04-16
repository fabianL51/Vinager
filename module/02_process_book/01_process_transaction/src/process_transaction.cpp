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

    // process by transaction type
    if (transaction_type == "T"){
        // interaccount transfers
        std::cout << "Interaccount transfers selected" << std::endl;

        bool valid_giver_acc = false;
        std::string giver_acc_codename;
        while (valid_giver_acc == false){
            std::cout << "Enter the codename of the giver account" << std::endl;
            for (auto Bank: BankVec){
                std::cout << "Type " << Bank.CodeName << " for " << Bank.Name << std::endl;
            }
            std::getline(std::cin, giver_acc_codename);
            valid_giver_acc = isValidCodename(BankVec, giver_acc_codename);
        }
    }

    
}
