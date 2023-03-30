#pragma once
#include "constants.h"
#include "Row.h"
#include "Indentation.h"
#include <ostream>

class Table
{
    Row rows[MAX_ROW_COUNT];
    Row columnTitles;
    Indentation indentation[MAX_ROW_COUNT];
    int rowsCount;
    int width;

public:
    Table();

    bool loadFromFile();
    bool saveToFile() const;
    void printTo(std::ostream &, bool) const;
    void addRow(const Row &);
    void addTitles(const Row &);
    void addIndent(const Indentation &, const int);
    void changeTitleName(const char *, const int);
    void changeCellValue(const int, const char *, const char *);
    void changeCellValue(const char *, const char *, const char *);
    void displayWhere(const char *, const char *);

private:
    void setWidth(const Row &);
    void displayHeaders(std::ostream &oStream, bool) const;
    int getColumnIdx(const char *) const;
    void printLine(const int, std::ostream &) const;
};