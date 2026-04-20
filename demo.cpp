#include "skiplist.h"
#include <iostream>

using namespace CustomADT;

int main()
{
    std::cout << "--- DEMONSTRATION MODE ---\n";
    SkipList list;

    list += 10; list += 5; list += 20; list += 15;
    std::cout << "After inserts: " << list.toString() << "\n";

    std::pair<int, int> ed = {10,12};
    list %= ed;
    std::cout << "After editing 10 to 12: " << list.toString() << "\n";

    list -= 5;
    std::cout << "After removing 5: " << list.toString() << "\n";

    std::cout << "Does 20 exist? " << (list[20] ? "Yes" : "No") << "\n";

    try
    {
        list += 20;
    } 
    catch(const DuplicateValueException& e)
    {
        std::cout << "Caught expected custom exception: " << e.what() << "\n";
    }

    SkipList copyList = list;
    std::cout << "Copied List: " << copyList.toString() << "\n";
    
    std::cout << "Lists are equal? " << (list == copyList ? "Yes" : "No") << "\n";

    !copyList;
    std::cout << "Cleaned Copied List: " << copyList.toString() << "\n";

    return 0;
}