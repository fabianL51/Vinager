#include "Banks.h" // for handling Banks classes
#include <xlnt/xlnt.hpp> // include xlnt for excel handling
#include "utils.h" // utilities functions

/* this script process and categorizes transaction into incomes and expenses. */
/* the transaction histories will be saved in a separate excel data for documentation. */


int main(){



    // initialize workbooks and worksheets
    xlnt::workbook FinCordsWkb, UtilWkb;
    xlnt::worksheet FinStateWks, AssetsWks, RecordsWks, UtilWks;
    std::string FinCordsWkbName = std::to_string(xlnt::date::today().year) + "_FinancialRecords.xlsx";
    std::string UtilWkbName = "Utilities.xlsx";

    // load workbooks
    FinCordsWkb.load(FinCordsWkbName);
    UtilWkb.load(UtilWkbName);

    // load worksheets
    FinStateWks = FinCordsWkb.sheet_by_title("Financial Statement");
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

    // get accounts and wealth classes
    std::pair<std::vector <Bank>, std::vector <Asset>> bank_wealth_vec = get_accounts_wealth(AssetsWks, acc_last_row, wealth_last_row);
    BankVec = bank_wealth_vec.first;
    WealthClassVec = bank_wealth_vec.second;

    // display all accounts
    std::cout << "------------- Current accounts balance ----------" << std::endl;
    for (int i = 0; i <= BankVec.size() - 1; i++){

        std::cout << BankVec.at(i).Name << ": " << BankVec.at(i).Balance << std::endl;
    }

    // display all accounts
    std::cout << "------------- Current wealth class balance ----------" << std::endl;
    for (int i = 0; i <= WealthClassVec.size() - 1; i++){

        std::cout << WealthClassVec.at(i).Name << ": " << WealthClassVec.at(i).Sum << std::endl;
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


    
}
