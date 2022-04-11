#include <xlnt/xlnt.hpp> // include xlnt for excel handling
#include "utils.h" // utilities functions
#include "ctime"

/* This script check and if needed create Banking workbooks and worksheets */

int main(){

    // initialize all workbooks and worksheets
    xlnt::workbook FinCordsWkb, UtilitiesWkb;
    xlnt::worksheet BalanceSheetWks, RecordsWks, UtilitiesWks;

    // initialize file name for Financial Records
    std::string TemplateFileName = "FinancialRecords_Template.xlsx";
    std::string CurrentFileName = std::to_string(xlnt::date::today().year) + "_FinancialRecords.xlsx";

    // open Template
    FinCordsWkb.load(TemplateFileName);

    // initialize Financial Statement sheet
    xlnt::worksheet IncStateWks = FinCordsWkb.sheet_by_title("Income Statement");
    int start_col = xlnt::column_t::column_index_from_string("B");
    int end_col = xlnt::column_t::column_index_from_string("M");
    std::string col_string;
    for (int i = start_col; i <= end_col; i++){
        // update formula for gross income and total expenses as sum from Income and Expenses sheet
        col_string = xlnt::column_t::column_string_from_index(i);
        // gross income in 16th row by default= sum all income: 4th row to 8th row by default
        IncStateWks.cell(col_string, 16).formula("=SUM(" + col_string + "4:" + col_string + "8)");
        // total expenses in 17th row by default= sum all expenses: 10th row to 14th row by default
        IncStateWks.cell(col_string, 17).formula("=SUM(" + col_string + "10:" + col_string + "14)");
        // net income in 18th row by default
        IncStateWks.cell(col_string, 18).formula("=" + col_string + "16+" + col_string + "17");
    }

    // add formula for yearly values
    IncStateWks.cell("N4").formula("=SUM(B4:M4)");
    IncStateWks.cell("N11").formula("=SUM(B11:M11)");
    IncStateWks.cell("N16").formula("=SUM(B16:M16)");
    IncStateWks.cell("N17").formula("=SUM(B17:M17)");
    IncStateWks.cell("N18").formula("=SUM(B18:M18)");

    // save workbook as a new file
    FinCordsWkb.save(CurrentFileName);

    // add a utility workbook to save latest rows and columns
    UtilitiesWks = UtilitiesWkb.active_sheet();
    UtilitiesWks.title("Main");

    // header
    UtilitiesWks.cell("A1").value("Properties");
    UtilitiesWks.cell("B1").value("Current Row/Column");
    
    // for Financial Statement sheet
    UtilitiesWks.cell("A2").value("Current Income Row in IC Sheet"); // current free row for next income category
    UtilitiesWks.cell("B2").value(4);
    UtilitiesWks.cell("A3").value("Current Expense Row in IC Sheet"); // current free row for next expense category
    UtilitiesWks.cell("B3").value(10);

    // Asset sheets
    UtilitiesWks.cell("A4").value("Account Row");
    UtilitiesWks.cell("B4").value(3);
    UtilitiesWks.cell("A5").value("Wealth Row");
    UtilitiesWks.cell("B5").value(3);

    // Records sheet
    UtilitiesWks.cell("A6").value("Records Row");
    UtilitiesWks.cell("B6").value(2);
    UtilitiesWks.cell("A7").value("Records Banks Column");
    UtilitiesWks.cell("B7").value("G");

    UtilitiesWkb.save("Utilities.xlsx");

}