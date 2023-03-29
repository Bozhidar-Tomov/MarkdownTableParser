#include "Table.h"
#include "Utils.h"
#include "Indentation.h"
#include "constants.h"
#include <iostream>
#include <fstream>
#include <sstream>

Table::Table()
{
    this->columnTitles = Row();
    this->rows[0] = Row();
    this->indentation[0] = Indentation::left;
    this->rowsCount = 0;
    this->width = 0;
}

Row parseLine(std::stringstream &lineStream)
{
    Row row;
    char value[MAX_FIELD_SIZE + 1];

    lineStream.seekg(1, std::ios::beg);

    if (!lineStream)
    {
        return row;
    }

    lineStream.seekg(1, std::ios::cur);

    while (lineStream.getline(value, MAX_FIELD_SIZE + 1, VERTICAL_DELIM))
    {
        Cell cell;
        removeExtraSpaces(value);
        cell.setValue(value);
        row.addValue(cell);
    }

    return row;
}

// Some external resources are used in the implementation of this function!
Indentation getIndentation(char *line)
{
    int i = 0;
    bool start = false;
    bool end = false;

    while (line[i] != '\0')
    {
        if (i == 0 && line[i] == INDENT_DELIM)
        {
            start = true;
        }
        if (line[i + 1] == '\0' && line[i] == INDENT_DELIM)
        {
            end = true;
        }
        ++i;
    }

    if (!start && end)
    {
        return Indentation::right;
    }
    if (start && end)
    {
        return Indentation::middle;
    }
    return Indentation::left;
}

void parseIndentationLine(std::stringstream &lineStream, Table *table)
{
    if (!lineStream)
    {
        return;
    }

    char value[MAX_FIELD_SIZE + 1];

    int idx = 0;

    while (lineStream.getline(value, MAX_FIELD_SIZE + 1, VERTICAL_DELIM))
    {
        removeExtraSpaces(value);
        table->addIndent(getIndentation(value), idx++);
    }
}

bool Table::loadFromFile()
{
    std::ifstream file(FILE_NAME);

    if (!file.is_open())
    {
        return false;
    }

    char line[MAX_COLUMN_COUNT * (MAX_FIELD_SIZE + 1)];
    file.getline(line, MAX_COLUMN_COUNT * (MAX_FIELD_SIZE + 1));

    std::stringstream ss(line);

    this->addTitles(parseLine(ss));

    file.getline(line, MAX_COLUMN_COUNT * (MAX_FIELD_SIZE + 1));

    ss.clear();
    ss.seekp(0, std::ios::beg);
    ss.str(line);

    parseIndentationLine(ss, this);

    while (file.getline(line, MAX_COLUMN_COUNT * (MAX_FIELD_SIZE + 1)))
    {
        ss.clear();
        ss.seekp(0, std::ios::beg);
        ss.str(line);

        this->addRow(parseLine(ss));
    }

    file.close();
    return true;
}

void Table::addRow(const Row &row)
{
    if (this->rowsCount >= MAX_ROW_COUNT)
    {
        return;
    }

    this->setWidth(row);

    this->rows[rowsCount++] = row;
}

void Table::addTitles(const Row &titles)
{

    if (this->columnTitles.getSize() >= MAX_ROW_COUNT)
    {
        return;
    }

    this->setWidth(titles);

    this->columnTitles = titles;
}

void Table::addIndent(const Indentation &indentation, const int idx)
{
    this->indentation[idx] = indentation;
}

void Table::setWidth(const Row &row)
{
    for (int i = 0; i < row.getSize(); ++i)
    {
        if (row.getValues()[i].getSize() > this->width)
        {
            this->width = row.getValues()[i].getSize();
        }
    }
}

void Table::print() const
{
    std::cout << "Table: " << FILE_NAME << '\n'
              << std::endl;

    if (this->width <= 0 || this->rowsCount <= 0)
    {
        return;
    }

    for (int i = 0; i < this->columnTitles.getSize(); ++i)
    {
        std::cout << VERTICAL_DELIM << " ";
        printStr(this->columnTitles.getValues()[i].getValue());
        mySetW(this->width - this->columnTitles.getValues()[i].getSize() + 1);
    }

    std::cout << VERTICAL_DELIM << std::endl;

    for (int i = 0; i < this->columnTitles.getSize(); ++i)
    {
        std::cout << VERTICAL_DELIM << " ";
        mySetW(this->width, HORIZONTAL_DELIM);
        std::cout << " ";
    }

    std::cout << VERTICAL_DELIM << std::endl;

    for (int i = 0; i < rows->getSize(); ++i)
    {
        for (int j = 0; j < this->columnTitles.getSize(); ++j)
        {
            std::cout << VERTICAL_DELIM << " ";
            printStr(this->rows[i].getValues()[j].getValue());
            mySetW(this->width - this->rows[i].getValues()[j].getSize() + 1);
        }
        std::cout << VERTICAL_DELIM << std::endl;
    }
    std::cout << std::endl;
}

void Table::changeTitleName(const char *newTitle, const int idx)
{
    if (idx < 0 || idx >= this->columnTitles.getSize())
    {
        return;
    }
    this->columnTitles.setValue(newTitle, idx);

    setWidth(this->columnTitles);
}

void Table::changeCellValue(const int rowNumber, const char *columnTitle, const char *newValue) // working
{
    if (!columnTitle || myStrLen(columnTitle) >= MAX_FIELD_SIZE || rowNumber < 0 || rowNumber > this->rowsCount)
    {
        return;
    }

    int size = this->columnTitles.getSize();

    for (int i = 0; i < size; ++i)
    {
        if (myStrCmp(this->columnTitles.getValues()[i].getValue(), columnTitle))
        {
            this->rows[rowNumber - 1].setValue(newValue, i);
        }
    }
}

void Table::changeCellValue(const char *columnTitle, const char *changeValue, const char *newValue)
{
    if (!columnTitle || myStrLen(columnTitle) >= MAX_FIELD_SIZE || myStrLen(changeValue) >= MAX_FIELD_SIZE)
    {
        return;
    }

    int size = this->columnTitles.getSize();

    for (int i = 0; i < size; ++i)
    {
        if (myStrCmp(this->columnTitles.getValues()[i].getValue(), columnTitle))
        {
            for (int j = 0; j < this->rowsCount; ++j)
            {
                if (myStrCmp(this->rows[j].getValues()[i].getValue(), changeValue))
                {
                    this->rows[j].setValue(newValue, i);
                }
            }
        }
    }
}