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

    // add a utility workbook to save latest rows and columns
    UtilitiesWks = UtilitiesWkb.active_sheet();
    UtilitiesWks.title("Main");

    // header
    UtilitiesWks.cell("A1").value("Properties");
    UtilitiesWks.cell("B1").value("Current Row/Column");
    
    // for Financial Statement sheet
    UtilitiesWks.cell("A2").value("Wealth Class in Allocation Row"); // wealth class in allocation
    UtilitiesWks.cell("B2").value(12);
    UtilitiesWks.cell("A3").value("Wealth Class in Cash Flow Row"); // wealth class in cash flow
    UtilitiesWks.cell("B3").value(16);

    // Asset sheets
    UtilitiesWks.cell("A4").value("Account Row");
    UtilitiesWks.cell("B4").value(3);
    UtilitiesWks.cell("A5").value("Wealth Row");
    UtilitiesWks.cell("B5").value(3);

    // Income and Expenses sheets
    UtilitiesWks.cell("A6").value("Income Row");
    UtilitiesWks.cell("B6").value(2);
    UtilitiesWks.cell("A7").value("Expense Row");
    UtilitiesWks.cell("B7").value(2);

    // Records sheet
    UtilitiesWks.cell("A8").value("Records Row");
    UtilitiesWks.cell("B8").value(2);
    UtilitiesWks.cell("A9").value("Records Banks Column");
    UtilitiesWks.cell("B9").value("G");

    UtilitiesWkb.save("Utilities.xlsx");

}