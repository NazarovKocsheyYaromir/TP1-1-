#ifndef BASE_H
#define BASE_H

#include <fstream>
#include <iostream>

// Коды типов производных классов - используются при сохранении/загрузке
// содержимого Keeper из файла (простой аналог RTTI на своих условиях).
enum PublicationType {
    POET_TYPE = 1,
    NOVELIST_TYPE = 2,
    SCIFI_TYPE = 3
};

// Абстрактный базовый класс "Печатное издание".
// Хранит общие для всех авторов данные: ФИО и список произведений.
class Base {
protected:
    char* fio;
    char** works;
    int worksCount;

    void copyFio(const char* src);
    void clearWorks();
    void copyWorks(char* const* srcWorks, int count);

public:
    Base();
    Base(const char* fioIn, char** worksArr, int count);
    Base(const Base& other);
    virtual Base& operator=(const Base& other);
    virtual ~Base();

    virtual void setFio(const char* newFio);
    virtual const char* getFio() const;

    virtual void addWork(const char* work);
    virtual void removeWork(int index);
    virtual int getWorksCount() const;
    virtual const char* getWork(int index) const;

    // Печать сведений об объекте
    virtual void print() const = 0;
    // Создание копии объекта на куче (для копирования в Keeper)
    virtual Base* clone() const = 0;
    // Тип объекта (для сохранения/загрузки)
    virtual int getType() const = 0;
    virtual const char* getTypeName() const = 0;

    // Сериализация в файл / восстановление из файла.
    virtual void saveToFile(std::ofstream& out) const;
    virtual void loadFromFile(std::ifstream& in);

    // Ввод данных объекта с клавиатуры, заполнение полей дочерних классов
    virtual void inputData();
    // Интерактивное меню редактирования конкретного объекта
    virtual void editMenu() = 0;

    friend std::ostream& operator<<(std::ostream& os, const Base& b);
};

#endif // BASE_H
