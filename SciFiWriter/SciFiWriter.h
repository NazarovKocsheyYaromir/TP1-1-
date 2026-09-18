#ifndef SCIFIWRITER_H
#define SCIFIWRITER_H

#include "../Base/Base.h"

// Фантаст: ФИО, несколько произведений, сняты ли фильмы по книгам.
class SciFiWriter : public Base {
private:
    bool filmsMade;

public:
    SciFiWriter();
    SciFiWriter(const char* fio, char** works, int count, bool filmsMade);
    SciFiWriter(const SciFiWriter& other);
    using Base::operator=; // явно объявляем, что базовый operator= тоже виден
    SciFiWriter& operator=(const SciFiWriter& other);
    ~SciFiWriter() override;

    void setFilmsMade(bool value);
    bool getFilmsMade() const;

    void print() const override;
    Base* clone() const override;
    int getType() const override;
    const char* getTypeName() const override;

    void saveToFile(std::ofstream& out) const override;
    void loadFromFile(std::ifstream& in) override;

    void inputData() override;
    void editMenu() override;
};

#endif // SCIFIWRITER_H
