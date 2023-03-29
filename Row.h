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

    void setRow(const Cell *, int);
    void addValue(const Cell &);
    void addValue(const Cell &, int);
    void removeValue();
    void removeValue(int);
    int getSize() const;
    void setValue(const char *title, const int);
    const Cell *getValues() const;
};
