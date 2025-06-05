#include <iostream>
#include "../Document/docs.h"

Document::Document() : _type(0), _series(0), _number(0) {}

Document::Document(int type, int series, int number) {
    if (type < 0 || series < 0 || number < 0) {
        throw std::invalid_argument("Document::Document: invalid document values");
    }
    _type = type;
    _series = series;
    _number = number;
}

Document::Document(const Document& other) : _type(other._type), _series(other._series), _number(other._number) {}

void Document::setType(int type) {
    if (type < 0) throw std::invalid_argument("Document::setType: invalid type");
    _type = type;
}

void Document::setSeries(int series) {
    if (series < 0) throw std::invalid_argument("Document::setSeries: invalid series");
    _series = series;
}

void Document::setNumber(int number) {
    if (number < 0) throw std::invalid_argument("Document::setNumber: invalid number");
    _number = number;
}