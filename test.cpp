#include "skiplist.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace CustomADT;

void logAndPrint(std::ofstream& logFile, const std::string& msg)
{
    std::cout << msg << "\n";
    logFile << msg << "\n";
}

int main()
{
    std::ofstream logFile("log.txt");
    if (!logFile.is_open()) return 1;

    int lineCount = 0;
    auto log = [&](const std::string& msg)
    {
        logAndPrint(logFile, "[" + std::to_string(++lineCount) + "] " + msg);
    };
    
    SkipList listA;
    log("[TEST] Initial State Validation");
    log(listA.toString() == "SkipList [Size: 0, Max Actual Lvl: 0] Elements: " ? "PASS: List A is empty" : "FAIL: List A not empty");

    log("[TEST] Bulk Insertions via operator+=");
    for (int i = 10; i <= 200; i += 10)
    {
        listA += i;
        log("Inserted: " + std::to_string(i) + " | Status: " + (listA[i] ? "PASS" : "FAIL"));
    }

    log("[TEST] Specific Searches via operator[]");
    log("Search 50 (exists): " + std::string(listA[50] ? "PASS" : "FAIL"));
    log("Search 150 (exists): " + std::string(listA[150] ? "PASS" : "FAIL"));
    log("Search 999 (does not exist): " + std::string(!listA[999] ? "PASS" : "FAIL"));

    log("[TEST] Multiple Deletions via operator-=");
    for (int i = 20; i <= 60; i += 20)
    {
        listA -= i;
        log("Deleted: " + std::to_string(i) + " | Status: " + (!listA[i] ? "PASS" : "FAIL"));
    }

    log("[TEST] Exception Handling Validation");
    try
    { 
        listA += 10; 
        log("FAIL: Duplicate 10 did not throw exception."); 
    }
    catch(const DuplicateValueException&)
    { 
        log("PASS: Custom DuplicateValueException safely caught."); 
    }
    
    try
    { 
        listA -= 888; 
        log("FAIL: Missing 888 did not throw exception."); 
    }
    catch (const std::invalid_argument&)
    { 
        log("PASS: Standard std::invalid_argument safely caught."); 
    }

    log("[TEST] Edit Operation via operator%=");
    std::pair<int, int> ed = {70,75};
    listA %= ed;
    log("Edit 70 to 75 | Status: " + std::string(listA[75] && !listA[70] ? "PASS" : "FAIL"));

    log("Current List A Data: " + listA.toString());
    
    log("[TEST] Deep Copy & All 6 Logical Comparisons");
    SkipList listB = listA;
    log("Deep Copy | Status: " + std::string((listA == listB) ? "PASS: operator==" : "FAIL"));
    log("Compare | Status: " + std::string(!(listA != listB) ? "PASS: operator!=" : "FAIL"));
    
    listB += 999;
    log("Modified List B (added 999) to test inequalities.");
    log("Compare | Status: " + std::string((listA < listB) ? "PASS: operator<" : "FAIL"));
    log("Compare | Status: " + std::string((listA <= listB) ? "PASS: operator<=" : "FAIL"));
    log("Compare | Status: " + std::string((listB > listA) ? "PASS: operator>" : "FAIL"));
    log("Compare | Status: " + std::string((listB >= listA) ? "PASS: operator>=" : "FAIL"));

    log("[TEST] Memory Cleanup via operator!");
    !listB;
    log("Cleaned List B | Status: " + std::string(!listB[999] ? "PASS" : "FAIL"));
    
    logFile.close();
    return 0;
}