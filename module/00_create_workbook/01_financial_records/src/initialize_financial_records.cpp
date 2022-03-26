#include <xlnt/xlnt.hpp> // include xlnt for excel handling
#include "utils.h" // utilities functions
#include "ctime"

/* This script check and if needed create Banking workbooks and worksheets */

int main(){

    // initialize all workbooks and worksheets
    xlnt::workbook FinCordsWkb;
    xlnt::worksheet BalanceSheetWks, RecordsWks;
    // get current year for file name
    std::string current_year = std::to_string(xlnt::date::today().year);

    // initialize file name for Financial Records
    std::string TemplateFileName = "FinancialRecords_Template.xlsx";
    std::string CurrentFileName = current_year + "_FinancialRecords.xlsx";

    // open Template
    FinCordsWkb.load(TemplateFileName);

    // initialize Financial Statement sheet
    xlnt::worksheet FinStateWks = FinCordsWkb.sheet_by_title("Financial Statement");
    int start_col = xlnt::column_t::column_index_from_string("B");
    int end_col = xlnt::column_t::column_index_from_string("M");
    std::string col_string;
    for (int i = start_col; i <= end_col; i++){
        // update formula for gross income and total expenses as sum from Income and Expenses sheet
        col_string = xlnt::column_t::column_string_from_index(i);
        // gross income in 3rd row
        FinStateWks.cell(col_string + "3").formula("=SUM(Income!" + col_string + ":" + col_string + ")");
        // total expenses in 4rd row
        FinStateWks.cell(col_string + "4").formula("=SUM(Expenses!" + col_string + ":" + col_string + ")");
        // net income in 7th row
        FinStateWks.cell(col_string + "7").formula("=" + col_string + "3+" + col_string + "4");
    }

    // add formula for yearly values
    FinStateWks.cell("N3").formula("=SUM(B3:M3)");
    FinStateWks.cell("N4").formula("=SUM(B4:M4)");
    FinStateWks.cell("N7").formula("=SUM(B7:M7)");

    // save workbook as a new file
    FinCordsWkb.save(CurrentFileName);

}