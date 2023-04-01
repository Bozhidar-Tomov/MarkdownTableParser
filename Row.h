#pragma once
#include "constants.h"
#include "Cell.h"

struct Row
{
private:
    Cell values[MAX_COLUMN_COUNT];
    int size;

public:
    Row();
    Row(const Cell *, int);

    int getSize() const;
    const Cell *getValues() const;

    void setRow(const Cell *, int);
    void setValue(const char *, const int);

    void addValue(const Cell &);
    void addValue(const Cell &, int);

    void removeValue();
    void removeValue(int);
};
