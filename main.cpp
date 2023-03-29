
#include <iostream>
#include "Table.h"

// TODO: alignment, selection->done, safe to file

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

    table.print();

    table.changeTitleName("Hello world!", 1);
    std::cout << "===============\n";
    table.print();
    table.changeCellValue(1, "Hello world!", "Hello world LONG!");
    std::cout << "===============\n";
    table.print();
    table.changeCellValue("body", "b", "VALUE");
    std::cout << "===============\n";
    table.displayWhere("Hello world!", "Text");
}