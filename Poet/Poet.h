#ifndef POET_H
#define POET_H

#include "../Base/Base.h"

// Поэт: ФИО, годы жизни, несколько основных произведений.
class Poet : public Base {
private:
    int birthYear;
    int deathYear;

public:
    Poet();
    Poet(const char* fio, char** works, int count, int birthYear, int deathYear);
    Poet(const Poet& other);
    using Base::operator=; // явно объявляем, что базовый operator= тоже виден
    Poet& operator=(const Poet& other);
    ~Poet() override;

    void setBirthYear(int y);
    void setDeathYear(int y);
    int getBirthYear() const;
    int getDeathYear() const;

    void print() const override;
    Base* clone() const override;
    int getType() const override;
    const char* getTypeName() const override;

    void saveToFile(std::ofstream& out) const override;
    void loadFromFile(std::ifstream& in) override;

    void inputData() override;
    void editMenu() override;
};

#endif // POET_H
