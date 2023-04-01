#include <iostream>
#include <sstream>
#include "Utils.h"
#include "constants.h"
#include "Table.h"

void run()
{

    std::cout << "Available commands:\n";

    std::cout << "1) load <file name>\n";
    std::cout << "2) changeColumn <column> <new value>\n";
    std::cout << "3) addRow <column> ... <column> (max " << MAX_COLUMN_COUNT << ")\n";
    std::cout << "4) changeRow:\n\t<row number> <column> <new value>\n\t<column> <cell value> <new value>\n";
    std::cout << "5) save\n";
    std::cout << "6) print\n";
    std::cout << "7) quit" << std::endl;

    Table table;

    while (true)
    {
        // clearing std::cin input buffer
        std::cin.clear();
        fflush(stdin);

        char command[COMMAND_MAX_SIZE];
        std::cin >> command;

        if (myStrCmp(command, "print"))
        {
            table.printTo(std::cout, false);
            continue;
        }

        if (myStrCmp(command, "changeColumn"))
        {

            char parameters[COMMAND_MAX_SIZE];

            char parameter1[COMMAND_MAX_SIZE];
            char parameter2[COMMAND_MAX_SIZE];

            std::cin.getline(parameters, COMMAND_MAX_SIZE);
            std::stringstream ss(parameters);
            ss.seekg(1, std::ios::beg);

            ss.getline(parameter1, COMMAND_MAX_SIZE, ' ');
            ss.getline(parameter2, COMMAND_MAX_SIZE);

            table.changeTitleName(parameter1, parameter2);
            continue;
        }

        if (myStrCmp(command, "addRow"))
        {
            Cell cells[MAX_COLUMN_COUNT];
            char line[COMMAND_MAX_SIZE];
            char value[MAX_FIELD_SIZE];
            int idx = 0;

            std::cin.getline(line, COMMAND_MAX_SIZE);
            std::stringstream ss(line);
            ss.seekg(1, std::ios::beg);

            while (!ss.eof() && idx < table.getColumnCount())
            {
                ss.getline(value, MAX_FIELD_SIZE, ' ');
                cells[idx++] = Cell(value);
            }

            table.addRow(Row(cells, idx));
            continue;
        }

        if (myStrCmp(command, "changeRow"))
        {
            char parameters[COMMAND_MAX_SIZE];

            char parameter[3][COMMAND_MAX_SIZE];

            std::cin.getline(parameters, COMMAND_MAX_SIZE);
            std::stringstream ss(parameters);
            ss.seekg(1, std::ios::beg);

            for (int i = 0; i < 3; ++i)
            {
                ss.getline(parameter[i], COMMAND_MAX_SIZE, ' ');
            }

            int colNum = strToInt(parameter[0]);
            if (colNum < 0)
            {
                table.changeCellValue(parameter[0], parameter[1], parameter[2]);
            }

            table.changeCellValue(strToInt(parameter[0]), parameter[1], parameter[2]);
            continue;
        }

        if (myStrCmp(command, "save"))
        {
            char parameter1[COMMAND_MAX_SIZE];
            std::cin >> parameter1;

            if (!table.saveToFile(parameter1))
            {
                std::cout << "Cannot save table!" << std::endl;
                return;
            };

            std::cout << "Table saved successfully." << std::endl;
            continue;
        }

        if (myStrCmp(command, "load"))
        {
            char parameter1[COMMAND_MAX_SIZE];
            std::cin >> parameter1;
            if (!parameter1)
            {
                std::cout << "Invalid parameter.";
                return;
            }

            if (!table.loadFromFile(parameter1))
            {
                std::cout << "Cannot load table!" << std::endl;
                return;
            };

            std::cout << "Table loaded successfully." << std::endl;
            continue;
        }

        if (myStrCmp(command, "quit"))
        {
            return;
        }

        std::cout << "Wrong input" << std::endl;
    }
}