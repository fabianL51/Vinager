#include <xlnt/xlnt.hpp> // include xlnt for excel handling
#include "utils.h" // utilities functions
#include "ctime" // to get current year

/* This script check and if needed create Banking workbooks and worksheets */


xlnt::workbook initialize_record_sheet(xlnt::workbook Wkb, std::string SheetName){
    
    /* this function create Balance Sheet in FinancialRecords.xlsx */

    // create sheet
    xlnt::worksheet Wks = Wkb.create_sheet();
    Wks.title(SheetName);

    // initialize headers
    Wks.cell("A1").value("Category");
    Wks.cell("B1").value("January");
    Wks.cell("C1").value("February");
    Wks.cell("D1").value("March");
    Wks.cell("E1").value("April");
    Wks.cell("F1").value("May");
    Wks.cell("G1").value("June");
    Wks.cell("H1").value("July");
    Wks.cell("I1").value("August");
    Wks.cell("J1").value("September");
    Wks.cell("K1").value("October");
    Wks.cell("L1").value("November");
    Wks.cell("M1").value("December");
    Wks.cell("N1").value("Yearly");

    return Wkb;
}



int main(){

    // initialize all workbooks and worksheets
    xlnt::workbook FinCordsWkb;
    xlnt::worksheet BalanceSheetWks, RecordsWks;

    // get current year for file name
    int tm_year;  // year since 1900
    time_t current_time = std::time(0);
    tm *ltm = localtime(&current_time);
    std::string current_year = std::to_string(1900 + ltm->tm_year);

    // initialize file name for Financial Records
    std::string TemplateFileName = "FinancialRecords_Template.xlsx";
    std::string CurrentFileName = current_year + "_FinancialRecords.xlsx";

    std::cout << file_exists(TemplateFileName) << std::endl;
    std::cout << CurrentFileName << std::endl;

    // open Template
    FinCordsWkb.load(TemplateFileName);

    // save workbook
    FinCordsWkb.save(CurrentFileName);
}