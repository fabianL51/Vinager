#include "Banks.h" // for handling Banks classes
#include "Asset.h" // for handling Asset classes
#include <xlnt/xlnt.hpp> // include xlnt for excel handling
#include "utils.h" // utilities functions
#include <iomanip> // for set precision

/* This functions updates added banks and wealth classes into FinancialRecords workbook */


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

    /* update accounts in Records sheet */

    // check if accounts already included in Records sheet
    int n_account_Rec = RecordsWks.highest_column().index - xlnt::column_t::column_index_from_string("F");

    // update header in Records sheet if accounts aren't already initialized
    if (BankVec.size() > n_account_Rec){

        // prepare border of header and border of data number 2
        xlnt::border header_border = RecordsWks.cell("F1").border();
        xlnt::border second_row_border = RecordsWks.cell("F2").border();

        int last_empty_index = xlnt::column_t::column_index_from_string(UtilWks.cell("B9").value<std::string>());

        for (int i = n_account_Rec; i <= BankVec.size() - 1; i++){
            
            // add account name to header
            RecordsWks.cell(xlnt::column_t::column_string_from_index(last_empty_index), 1).value(BankVec.at(i).Name);

            // reborder header and second row
            RecordsWks.cell(xlnt::column_t::column_string_from_index(last_empty_index), 1).border(header_border);
            RecordsWks.cell(xlnt::column_t::column_string_from_index(last_empty_index), 2).border(second_row_border);
            last_empty_index += 1;
        }

        // update information in Utilities.xls
        UtilWks.cell("B9").value(xlnt::column_t::column_string_from_index(last_empty_index));
    }

    /* update wealth class in Financial Statement sheet */

    // get the rows data
    int first_row_CF = UtilWks.cell("B3").value<int>();
    int start_row_Alloc = UtilWks.cell("B2").value<int>();

    // calculate number of missing row: 
    // start_row_Alloc + WealthClassVec.size() -> rows needed for all wealth class with an empty row to cash flow field
    // first_row_CF - 2 -> last row of allocation field
    int missing_rows = start_row_Alloc + WealthClassVec.size() - (first_row_CF - 2);

    // check if rows has to be inserted
    if (missing_rows > 0){
        
        // insert row at start_row_Alloc + 1
        FinStateWks.insert_rows(start_row_Alloc + 1, missing_rows);
        first_row_CF += missing_rows;
        
        // reborder new rows: careful: bordering already bordered cells may destroy worksheet!
        xlnt::border border_data = create_data_border(); // create border with thin line
        // allocation fields
        // start bordering at one row after start row and end at 2 row before end of allocation field
        FinStateWks.range(xlnt::range_reference("A", start_row_Alloc + 1, "N", first_row_CF - 4)).border(border_data);
        // cash flow fields
        FinStateWks.range(xlnt::range_reference("A", FinStateWks.highest_row() + 1, "N", first_row_CF + WealthClassVec.size() - 1)).border(border_data);

    }

    // get the index of already added wealth class
    int n_start_wc = start_row_Alloc - 12; // first data is at 12th row

    // remember: 2nd index means the third data in vector
    for (int i = n_start_wc; i <= WealthClassVec.size() - 1; i++){

        // add name in both Allocation and Cash Flow
        FinStateWks.cell("A", start_row_Alloc).value(WealthClassVec.at(i).Name);
        FinStateWks.cell("A", first_row_CF + i).value(WealthClassVec.at(i).Name);

        // add start_row_alloc by one
        start_row_Alloc += 1;
        
    }
    
    // save workbooks
    FinCordsWkb.save(FinCordsWkbName);
    UtilWkb.save(UtilWkbName); 
}

