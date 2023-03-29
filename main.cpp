
#include <iostream>
#include "Table.h"

int main()
{
    Table table;

    if (table.loadFromFile())
    {
        std::cout << "Table loaded successfully." << std::endl;
    }

    table.print();

    table.changeTitleName("New!New!New!New!Ne", 0);

    table.print();

    int stop = 0;
}