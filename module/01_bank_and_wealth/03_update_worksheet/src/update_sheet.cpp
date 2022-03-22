#include "Banks.h" // for handling Banks classes
#include "Asset.h" // for handling Asset classes
#include <xlnt/xlnt.hpp> // include xlnt for excel handling
#include "utils.h" // utilities functions
#include <iomanip> // for set precision

/* This functions updates added banks and wealth classes into FinancialRecords workbook */


int main(){

    // initialize workbooks and worksheets
    xlnt::workbook Wkb;
    xlnt::worksheet AccWealthWks;

    // load workbook
    std::string current_year = std::to_string(get_current_time("year"));
    std::string WkbName =current_year + "_FinancialRecords.xlsx";
    Wkb.load(WkbName);
    AccWealthWks = Wkb.sheet_by_title("Accounts & Wealth");

    // initialize vectors to get banks and wealth
    
}

