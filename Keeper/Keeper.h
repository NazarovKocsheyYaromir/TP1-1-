#ifndef KEEPER_H
#define KEEPER_H

#include "../Base/Base.h"

// Контейнер для хранения указателей на объекты производных от Base классов.
// Реализован на "сыром" динамическом массиве указателей (без STL контейнеров).
class Keeper {
private:
    Base** items;
    int count;
    int capacity;

    void ensureCapacity();
    static Base* createByType(int type);

public:
    Keeper();
    Keeper(const Keeper& other);
    Keeper& operator=(const Keeper& other);
    ~Keeper();

    // Добавляет объект в контейнер (Keeper становится владельцем указателя)
    void add(Base* item);
    // Удаляет объект по индексу
    void remove(int index);
    // Доступ к объекту по индексу
    Base* get(int index) const;
    int getCount() const;

    void printAll() const;
    void clear();

    void saveToFile(const char* filename) const;
    void loadFromFile(const char* filename);

    Base& operator[](int index) const;
};

#endif // KEEPER_H
