#ifndef NOVELIST_H
#define NOVELIST_H

#include "../Base/Base.h"

// Романист: ФИО, годы жизни, несколько произведений, краткая биография.
class Novelist : public Base {
private:
    int birthYear;
    int deathYear;
    char* biography;

    void copyBiography(const char* src);

public:
    Novelist();
    Novelist(const char* fio, char** works, int count, int birthYear, int deathYear, const char* biography);
    Novelist(const Novelist& other);
    using Base::operator=; // явно объявляем, что базовый operator= тоже виден
    Novelist& operator=(const Novelist& other);
    ~Novelist() override;

    //геттер сеттеры
    void setBirthYear(int y);
    void setDeathYear(int y);
    void setBiography(const char* bio);
    int getBirthYear() const;
    int getDeathYear() const;
    const char* getBiography() const;

    void print() const override;
    Base* clone() const override;
    int getType() const override;
    const char* getTypeName() const override;

    void saveToFile(std::ofstream& out) const override;
    void loadFromFile(std::ifstream& in) override;

    void inputData() override;
    void editMenu() override;
};

#endif // NOVELIST_H
