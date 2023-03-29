#include "Cell.h"
#include "Utils.h"
#include <iostream>

Cell::Cell()
{
    this->str[0] = {'\0'};
    this->size = 0;
}

Cell::Cell(const char *value)
{
    setValue(value);
}

// bool Cell::operator==(const Cell &other) const
// {
//     return myStrCmp(this->value, other.value) && this->size == other.size;
// }

void Cell::setValue(const char *value)
{
    if (!value)
    {
        return;
    }

    int size = myStrLen(value);

    if (size >= MAX_FIELD_SIZE)
    {
        return;
    }

    myStrCpy(this->str, value);
    this->size = size;
}

const char *Cell::getValue() const
{
    return this->str;
}

int Cell::getSize() const
{
    return this->size;
}
