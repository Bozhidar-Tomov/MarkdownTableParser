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

    if (!lineStream)
    {
        return row;
    }

    lineStream.seekg(1, std::ios::beg);

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

    lineStream.seekg(1, std::ios::beg);

    char value[MAX_FIELD_SIZE + 1] = {'\0'};

    int idx = 0;

    while (lineStream.getline(value, MAX_FIELD_SIZE + 1, VERTICAL_DELIM))
    {
        removeExtraSpaces(value);
        table->addIndent(getIndentation(value), idx++);
    }
}

bool Table::loadFromFile(const char *fileName)
{
    std::ifstream file(fileName);

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

bool Table::saveToFile(const char *fileName) const
{
    std::ofstream file(fileName);

    if (!file.is_open())
    {
        return false;
    }

    printTo(file, true);
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

void Table::displayHeaders(std::ostream &oStream, bool isFileStream) const
{
    if (!isFileStream)
    {
        oStream << "Table: " << FILE_NAME << '\n'
                << std::endl;
    }

    if (this->width <= 0 || this->rowsCount <= 0)
    {
        return;
    }

    for (int i = 0; i < this->columnTitles.getSize(); ++i)
    {
        oStream << VERTICAL_DELIM << " ";
        printStr(this->columnTitles.getValues()[i].getValue(), oStream);
        mySetW(this->width - this->columnTitles.getValues()[i].getSize() + 1, DEFAULT_CHAR, oStream);
    }

    oStream << VERTICAL_DELIM << std::endl;

    for (int i = 0; i < this->columnTitles.getSize(); ++i)
    {
        oStream << VERTICAL_DELIM << " ";

        if (isFileStream)
        {
            if (this->indentation[i] == Indentation::left || this->indentation[i] == Indentation::middle)
            {
                oStream << ":";
            }
        }

        mySetW(this->width, HORIZONTAL_DELIM, oStream);

        if (isFileStream)
        {
            if (this->indentation[i] == Indentation::right || this->indentation[i] == Indentation::middle)
            {
                oStream << ":";
            }
        }
        oStream << " ";
    }

    oStream << VERTICAL_DELIM << std::endl;
}

void Table::printTo(std::ostream &oStream, bool isFileStream) const
{
    displayHeaders(oStream, isFileStream);

    for (int i = 0; i < this->rowsCount; ++i)
    {
        printLine(i, oStream);
    }
    if (!isFileStream)
    {
        oStream << std::endl;
    }
}

void Table::changeTitleName(const char *title, const char *newTitle)
{
    int idx = getColumnIdx(title);

    if (idx < 0 || idx >= this->columnTitles.getSize())
    {
        return;
    }
    this->columnTitles.setValue(newTitle, idx);

    setWidth(this->columnTitles);
}

void Table::changeCellValue(const int rowNumber, const char *columnTitle, const char *newValue)
{
    if (!columnTitle || myStrLen(columnTitle) >= MAX_FIELD_SIZE || rowNumber < 0 || rowNumber > this->rowsCount)
    {
        return;
    }

    int columnIdx = getColumnIdx(columnTitle);

    if (columnIdx < 0)
    {
        return;
    }

    this->rows[rowNumber - 1].setValue(newValue, columnIdx);

    setWidth(this->rows[rowNumber - 1]);
}

void Table::changeCellValue(const char *columnTitle, const char *changeValue, const char *newValue)
{
    if (!columnTitle || myStrLen(columnTitle) >= MAX_FIELD_SIZE || myStrLen(changeValue) >= MAX_FIELD_SIZE)
    {
        return;
    }

    int size = this->columnTitles.getSize();

    int columnIdx = getColumnIdx(columnTitle);

    if (columnIdx < 0)
    {
        return;
    }

    for (int j = 0; j < this->rowsCount; ++j)
    {
        if (myStrCmp(this->rows[j].getValues()[columnIdx].getValue(), changeValue))
        {
            this->rows[j].setValue(newValue, columnIdx);
            setWidth(this->rows[j]);

            return;
        }
    }
}

int Table::getColumnIdx(const char *columnTitle) const
{
    int size = this->columnTitles.getSize();

    for (int i = 0; i < size; ++i)
    {
        if (myStrCmp(this->columnTitles.getValues()[i].getValue(), columnTitle))
        {
            return i;
        }
    }
    return -1;
}

void Table::printLine(const int rowIdx, std::ostream &oStream) const
{
    for (int i = 0; i < this->columnTitles.getSize(); ++i)
    {
        oStream << VERTICAL_DELIM;
        if (this->indentation[i] == Indentation::left)
        {
            oStream << " ";
            printStr(this->rows[rowIdx].getValues()[i].getValue(), oStream);
            mySetW(this->width - this->rows[rowIdx].getValues()[i].getSize() + 1, DEFAULT_CHAR, oStream);
            continue;
        }
        if (this->indentation[i] == Indentation::middle)
        {
            int valueSize = this->rows[rowIdx].getValues()[i].getSize();

            int diff = (this->width - valueSize + 2);

            bool extraSpace = false;

            if (diff % 2 != 0)
            {
                extraSpace = true;
                --diff;
            }

            diff /= 2;

            if (diff == 0)
            {
                diff = 2;
            }

            mySetW(diff + extraSpace, DEFAULT_CHAR, oStream);
            printStr(this->rows[rowIdx].getValues()[i].getValue(), oStream);
            mySetW(diff, DEFAULT_CHAR, oStream);
            continue;
        }
        if (this->indentation[i] == Indentation::right)
        {
            mySetW(this->width - this->rows[rowIdx].getValues()[i].getSize() + 1, DEFAULT_CHAR, oStream);
            printStr(this->rows[rowIdx].getValues()[i].getValue(), oStream);
            oStream << " ";
            continue;
        }
        break;
    }
    oStream << VERTICAL_DELIM << std::endl;
}

void Table::displayWhere(const char *columnTitle, const char *rowValue)
{
    if (!columnTitle || myStrLen(columnTitle) >= MAX_FIELD_SIZE || myStrLen(rowValue) >= MAX_FIELD_SIZE)
    {
        return;
    }

    displayHeaders(std::cout, false);

    int columnIdx = getColumnIdx(columnTitle);

    if (columnIdx < 0)
    {
        return;
    }

    for (int i = 0; i < this->rowsCount; ++i)
    {
        if (myStrCmp(this->rows[i].getValues()[columnIdx].getValue(), rowValue))
        {
            printLine(i, std::cout);
        }
    }
    std::cout << std::endl;
}

int Table::getColumnCount() const
{
    return this->columnTitles.getSize();
}
