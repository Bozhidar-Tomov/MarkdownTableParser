#pragma once
#include "constants.h"
#include "Row.h"
#include <ostream>

enum class Indentation
{
    left,
    middle,
    right,
};

class Table
{
    Row rows[MAX_ROW_COUNT];
    Row columnTitles;
    Indentation indentation[MAX_COLUMN_COUNT];
    int rowsCount;
    int width;

public:
    Table();

    bool loadFromFile(const char *);
    bool saveToFile(const char *) const;
    void printTo(std::ostream &, bool) const;

    void addRow(const Row &);
    void addTitles(const Row &);
    void addIndent(const Indentation &, const int);

    void changeTitleName(const char *, const char *);
    void changeCellValue(const int, const char *, const char *);
    void changeCellValue(const char *, const char *, const char *);

    void displayWhere(const char *, const char *);
    int getColumnCount() const;

private:
    int getColumnIdx(const char *) const;
    void setWidth(const Row &);

    void displayHeaders(std::ostream &oStream, bool) const;
    void printLine(const int, std::ostream &) const;
};