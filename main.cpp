
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

    table.print();

    table.changeTitleName("new!", 1);
    table.print();
    table.changeCellValue(1, "new!", "ROW1");
    table.print();
    table.changeCellValue("Syntax", "Paragraph", "VALUE");

    table.print();

    int stop = 0;
}