#include <xlnt/xlnt.hpp> // include xlnt for excel handling
#include "utils.h" // utilities functions
#include "ctime"

/* This script check and if needed create Banking workbooks and worksheets */

int main(){

    // initialize all workbooks and worksheets
    xlnt::workbook FinCordsWkb;
    xlnt::worksheet BalanceSheetWks, RecordsWks;

    // get current year for file name
    std::string current_year = std::to_string(get_current_time("year"));

    // initialize file name for Financial Records
    std::string TemplateFileName = "FinancialRecords_Template.xlsx";
    std::string CurrentFileName = current_year + "_FinancialRecords.xlsx";

    // open Template
    FinCordsWkb.load(TemplateFileName);

    // get sheet accounts and wealth to be initialized
    xlnt::worksheet Wks = FinCordsWkb.sheet_by_title("Accounts & Wealth");

    // initialize headers
    // Bank and Accounts
    Wks.cell("A1").value("Bank Name");
    Wks.cell("B1").value("Asset Type");
    Wks.cell("C1").value("Start Balance"); 
    Wks.cell("D1").value("Current Balance");
    Wks.cell("E1").value("Change in Balance");

    // Wealth
    Wks.cell("H1").value("Class Name");
    Wks.cell("I1").value("Start Balance"); 
    Wks.cell("J1").value("Current Balance");
    Wks.cell("K1").value("Change in Balance");

    Wks.cell("N1").value("Asset Type"); // asset type for liquid and fixed
    Wks.cell("O1").value("Asset Total"); // total value for asset type

    // save workbook as a new file
    FinCordsWkb.save(CurrentFileName);

}