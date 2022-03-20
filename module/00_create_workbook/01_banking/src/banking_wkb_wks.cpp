#include <xlnt/xlnt.hpp> // include xlnt for excel handling
#include "utils.h" // utilities functions

/* This script check and if needed create Banking workbooks and worksheets */


xlnt::workbook create_accounts_sheet(xlnt::workbook BanksWkb){
    
    /* this function create accounts sheet in Banks.xlsx */

    // create sheet
    xlnt::worksheet BanksAccWks = BanksWkb.create_sheet();
    BanksAccWks.title("Accounts");

    // initialize headers
    BanksAccWks.cell("A1").value("Bank Name"); // header for bank names
    BanksAccWks.cell("B1").value("Current Balance"); // header for current balance
    BanksAccWks.cell("C1").value("Asset Type"); // header for asset type

    return BanksWkb;
}

xlnt::workbook create_wealth_sheet(xlnt::workbook BanksWkb){
    
    /* this function create wealth allocations sheet in Banks.xlsx */

    // create sheet
    xlnt::worksheet BanksWealthWks = BanksWkb.create_sheet();
    BanksWealthWks.title("Wealth Allocation");

    // initialize headers
    BanksWealthWks.cell("A1").value("Class"); // header for wealth class type
    BanksWealthWks.cell("B1").value("Balance"); // balance for each wealth class
    BanksWealthWks.cell("G1").value("Asset Type"); // asset type for liquid and fixed
    BanksWealthWks.cell("H1").value("Asset Total"); // total value for asset type


    return BanksWkb;
}

int main(){

    // initialize all workbooks and worksheets
    xlnt::workbook BanksWkb;
    xlnt::worksheet BanksAccWks, BanksWealthWks;

    // initialize file name for Banks
    std::string BankFileName = "Banks.xlsx";

    // check Banks.xlsx
    bool isBank = file_exists(BankFileName);

    if (isBank == false){
    // if Banks.xlsx hasn't been created

        // initialize Accounts and Wealth Allocation sheet
        BanksWkb = create_accounts_sheet(BanksWkb);
        BanksWkb = create_wealth_sheet(BanksWkb);

    }
    else {
    // Banks.xlsx has already been created

        BanksWkb.load(BankFileName);

        if (BanksWkb.contains("Accounts") == false){
            // create accounts sheet if accounts sheet doesn't exist
            BanksWkb = create_accounts_sheet(BanksWkb);
        } 

        if (BanksWkb.contains("Wealth Allocation") == false){
            // create accounts sheet if wealth allocation sheet doesn't exist
            BanksWkb = create_wealth_sheet(BanksWkb);
        } 
    }

    // remove Sheet1 if workbook contains this sheet
    if(BanksWkb.contains("Sheet1") == true){
        xlnt::worksheet UnusedSheet = BanksWkb.sheet_by_title("Sheet1");
        BanksWkb.remove_sheet(UnusedSheet);
    } 

    // save workbook
    BanksWkb.save(BankFileName);
}