#include "Row.h"
#include "Utils.h"
#include "constants.h"

Row::Row()
{
    this->values[0] = Cell();
    this->size = 0;
}

Row::Row(const Cell *values, int size)
{
    setRow(values, size);
}

int Row::getSize() const
{
    return this->size;
}

const Cell *Row::getValues() const
{
    return this->values;
}

void Row::setRow(const Cell *values, int size)
{
    if (values == nullptr || size >= MAX_FIELD_SIZE)
    {
        Row();
        return;
    }

    this->size = size;

    for (int i = 0; i < size; ++i)
    {
        this->values[i] = values[i];
    }
}

void Row::setValue(const char *value, const int idx)
{
    if (idx < 0 || idx >= this->size)
    {
        return;
    }
    this->values[idx].setValue(value);
}

// append
void Row::addValue(const Cell &value)
{
    if (this->size >= MAX_FIELD_SIZE)
    {
        return;
    }

    this->values[this->size++] = value;
}

// at index
void Row::addValue(const Cell &value, int idx)
{
    if (idx < 0 || this->size >= MAX_FIELD_SIZE || idx >= this->size)
    {
        return;
    }

    for (int i = this->size; i > idx + 1; --i)
    {
        this->values[i] = this->values[i - 1];
    }

    this->values[idx] = value;
}

// at end
void Row::removeValue()
{
    --this->size;
}

// at index
void Row::removeValue(int idx)
{
    if (idx < 0 || idx >= this->size)
    {
        return;
    }

    for (int i = idx; i < this->size - 1; ++i)
    {
        this->values[i] = this->values[i + 1];
    }

    --this->size;
}
