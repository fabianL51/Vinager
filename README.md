# Vinager

Vinager is a simple bookkeeping software to handle daily transactions data. Users only need to fill the transaction details and the software will deal with the bookeeping process. The goal is to give users a good overview over incomes and expenses over the year. The software can handle multiple bank accounts (including cash) and it automatically processes recurring incomes and expenses. 

The development of Vinager is meant to eliminate the disadvantages of the bookkeeping software named Diskcounter (https://github.com/fabianL51/Diskcounter) that I developed for Microsoft Excel using VBA. The use of C++ in Vinager is supposed to increase the processing speed of the transactions data compared to Diskcounter. Another important aspect is that I want Vinager to operate without requiring any other software to be installed in the machine (as users would either have to own a commercial software as is the case with Diskcounter or users have to learn how to use the other software). For this reason, the bookkeeping data are stored in CSV files, eventhough I am fully aware that this is probably not the best choice for a database in terms of efficiency. I recommend the use of a free software to view the CSV data (LibreOffice or ExcelViewer in Visual Studio Code), as the data are intended to be viewed in a table.

The software is currently still under development. At this stage, processes can only be run through the terminals in Linux. Each program still must be compiled separately. My current goal is to have all modules of programs done and compiled together in a main program that do all the processes. The users only have to run this program for their bookkeeping. The next goal is to integrate the functioning software to a user interface (probably through Qt), so that users without shell-scripting experiences can use the software through the UI.

For examples, you can check exe folder and open the .csv files generated by the scripts. Documentation about the generated .csv files will be provided after the program is launched.

## Special Features

1. Vinager can handle more than 3 Accounts compared to Diskcounter
2. Vinager stores its process' outputs in .csv files, which can be opened with every text editor. Therefore, you can open these in every machine.
