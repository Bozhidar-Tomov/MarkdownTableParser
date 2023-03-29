#pragma once
#include "constants.h"

struct Cell
{
private:
    char str[MAX_FIELD_SIZE + 1];
    int size;

public:
    Cell();
    Cell(const char *);

    // bool operator==(const Cell &other) const;

    void setValue(const char *);
    const char *getValue() const;
    int getSize() const;
};
