#include "Banks.h" // for handling Banks classes
#include "Asset.h" // for handling Asset classes
#include <xlnt/xlnt.hpp> // include xlnt for excel handling
#include "utils.h" // utilities functions
#include <iomanip> // for set precision

/* This functions updates added banks and wealth classes into FinancialRecords workbook */


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

    // get accounts and wealth classes
    std::pair<std::vector <Bank>, std::vector <Asset>> bank_wealth_vec = get_accounts_wealth(AssetsWks, acc_last_row, wealth_last_row);
    BankVec = bank_wealth_vec.first;
    WealthClassVec = bank_wealth_vec.second;


    // check if accounts already included in Records sheet
    int n_account_Rec = RecordsWks.highest_column().index - xlnt::column_t::column_index_from_string("F");

    // update header in Records sheet if accounts aren't already initialized
    if (BankVec.size() > n_account_Rec){

        // prepare border of header and border of data number 2
        xlnt::border header_border = RecordsWks.cell("F1").border();
        xlnt::border second_row_border = RecordsWks.cell("F2").border();
        xlnt::border third_row_border = RecordsWks.cell("F3").border();

        int last_empty_index = xlnt::column_t::column_index_from_string(UtilWks.cell("B7").value<std::string>());

        for (int i = n_account_Rec; i <= BankVec.size() - 1; i++){
            
            // add account name to header
            RecordsWks.cell(xlnt::column_t::column_string_from_index(last_empty_index), 1).value(BankVec.at(i).Name);

            // reborder header and second, third row
            RecordsWks.cell(xlnt::column_t::column_string_from_index(last_empty_index), 1).border(header_border);
            RecordsWks.cell(xlnt::column_t::column_string_from_index(last_empty_index), 2).border(second_row_border);
            RecordsWks.cell(xlnt::column_t::column_string_from_index(last_empty_index), 3).border(third_row_border);
            last_empty_index += 1;
        }

        // update information in Utilities.xls
        UtilWks.cell("B7").value(xlnt::column_t::column_string_from_index(last_empty_index));
    }

    // save workbooks
    FinCordsWkb.save(FinCordsWkbName);
    UtilWkb.save(UtilWkbName); 
}

