#include <iostream>
#include "../FIO/fio.h"

FIO::FIO(const std::string& surname, const std::string& name, const std::string& patronymic)
    : _surname(surname), _name(name), _patronymic(patronymic) {
}

FIO::FIO(const FIO& other)
    : _surname(other._surname), _name(other._name), _patronymic(other._patronymic) {
}

void FIO::setSurname(const std::string& surname) { _surname = surname; }
void FIO::setName(const std::string& name) { _name = name; }
void FIO::setPatronymic(const std::string& patronymic) { _patronymic = patronymic; }