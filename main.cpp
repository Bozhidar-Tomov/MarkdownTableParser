
#include <iostream>
#include "Table.h"

int main()
{
    Table table;

    if (table.loadFromFile())
    {
        std::cout << "Table loaded successfully." << std::endl;
    }
    else
    {
        std::cout << "Cannot load table!" << std::endl;
        return 0;
    }

    table.printTo(std::cout, false);

    table.changeTitleName("Hello world!", 1);
    std::cout << "===============\n";
    table.printTo(std::cout, false);
    table.changeCellValue(1, "Hello world!", "Hello world LONG");
    std::cout << "===============\n";
    table.printTo(std::cout, false);
    table.changeCellValue("body", "b", "VALUE");
    std::cout << "===============\n";
    table.displayWhere("Hello world!", "Text");

    table.saveToFile();
}