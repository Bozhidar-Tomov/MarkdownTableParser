#pragma once
#include "constants.h"
#include "Row.h"
#include "Indentation.h"

class Table
{
    Row rows[MAX_ROW_COUNT];
    Row columnTitles;
    Indentation indentation[MAX_ROW_COUNT];
    int rowsCount;
    int width;

    void setWidth(const Row &);

public:
    Table();

    bool loadFromFile();
    void saveToFile() const;
    void print() const;
    void addRow(const Row &);
    void addTitles(const Row &);
    void addIndent(const Indentation &, const int);
    void changeTitleName(const char *, const int);
    void changeCellValue(const int, const char *, const char *);
    void changeCellValue(const char *, const char *, const char *);
};