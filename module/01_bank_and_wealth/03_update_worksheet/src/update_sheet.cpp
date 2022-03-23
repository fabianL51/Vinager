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

    // get the last row of worksheet
    int last_row = AccWealthWks.highest_row();

    // initialize variables
    std::vector <Bank> BankVec;
    std::vector <Asset> WealthClassVec;
    Bank tempBank;
    Asset tempWealthClass;

    // get banks and wealth data
    for (int i = 2; i <= last_row; i++){
        
        // reinitialize temporary variables
        tempBank, tempWealthClass;

        // get bank data by checking if current cell A of current row has value
        if (AccWealthWks.cell("A" + std::to_string(i)).has_value() == true){
            tempBank.Name = AccWealthWks.cell( "A" + std::to_string(i) ).value<std::string>(); // col A: bank name
            tempBank.AssetType = AccWealthWks.cell( "B" + std::to_string(i) ).value<std::string>(); // col B: bank asset type
            tempBank.Balance = AccWealthWks.cell( "D" + std::to_string(i) ).value<double>(); // col D: current balance
        }
        else {
            // if empty then name tempBank with error message
            tempBank.Name = "21Err33";
        }

        // add bank to vector if tempBank contains valid data bank
        if (tempBank.Name != "21Err33"){
            BankVec.emplace_back(tempBank);
        }
     
        // get wealth class data by checking if current cell H of current row has value
        if (AccWealthWks.cell("H" + std::to_string(i)).has_value() == true){
            tempWealthClass.Name = AccWealthWks.cell( "H" + std::to_string(i) ).value<std::string>(); // col H: wealth class name
            tempWealthClass.Sum = AccWealthWks.cell( "J" + std::to_string(i) ).value<double>(); // col J: current balance
        }
        else {
            // if empty then name tempBank with error message
            tempWealthClass.Name = "42Err11";
        }

        // add wealth class to vector if tempBank contains valid data bank
        if (tempWealthClass.Name != "42Err11"){
            WealthClassVec.emplace_back(tempWealthClass);
        }        
    }

    // update Wealth in Balance Sheet: starting at row 14 for wealth allocation and row 22 for cash flow
    if (WealthClassVec.size() > 0){
        xlnt::worksheet BalSheetWks = Wkb.sheet_by_title("Balance Sheet");
        for (int i = 0; i <= WealthClassVec.size() - 1; i++){
            BalSheetWks.cell("A" + std::to_string(i + 14)).value(WealthClassVec.at(i).Name);
            BalSheetWks.cell("A" + std::to_string(i + 22)).formula("=A" + std::to_string(i + 14));
        }
    }
    
    // expand Bank column in Records sheet: 1st row starting at column G
    if (BankVec.size() > 0){
        xlnt::worksheet RecordsWks = Wkb.sheet_by_title("Records");
        xlnt::column_t last_col = RecordsWks.highest_column();
        xlnt::border header_border;
        header_border = RecordsWks.cell(last_col.column_string_from_index(last_col.index) + "1").border(); // border style of F1
    
        for (int i = 0; i <= BankVec.size() - 1; i++){
            RecordsWks.cell(last_col.column_string_from_index(last_col.index + i + 1) + "1").value(BankVec.at(i).Name);
            RecordsWks.cell(last_col.column_string_from_index(last_col.index + i + 1) + "1").border(header_border);
        }
    }
    
    // save workbook
    Wkb.save(WkbName);
}

