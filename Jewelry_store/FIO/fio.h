#pragma once
#include <string>

class FIO {
private:
    std::string _surname;
    std::string _name;
    std::string _patronymic;

public:
    FIO(const std::string& surname, const std::string& name, const std::string& patronymic);
    FIO(const FIO& other);

    std::string getSurname() const { return _surname; }
    std::string getName() const { return _name; }
    std::string getPatronymic() const { return _patronymic; }

    void setSurname(const std::string& surname);
    void setName(const std::string& name);
    void setPatronymic(const std::string& patronymic);
};