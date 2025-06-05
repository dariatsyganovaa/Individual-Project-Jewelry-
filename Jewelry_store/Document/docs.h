#pragma once
#include <stdexcept>

class Document {
private:
    int _type;
    int _series;
    int _number;

public:
    Document();
    Document(int type, int series, int number);
    Document(const Document& other);

    int getType() const { return _type; }
    int getSeries() const { return _series; }
    int getNumber() const { return _number; }

    void setType(int type);
    void setSeries(int series);
    void setNumber(int number);
};