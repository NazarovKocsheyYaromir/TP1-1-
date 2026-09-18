#include "Keeper.h"
#include "../Exceptions/Exceptions.h"
#include "../Poet/Poet.h"
#include "../Novelist/Novelist.h"
#include "../SciFiWriter/SciFiWriter.h"
#include <fstream>
#include <iostream>

using namespace std;

void Keeper::ensureCapacity() {
    if (count < capacity) return;
    int newCapacity = (capacity == 0) ? 4 : capacity * 2;
    Base** newItems = new Base*[newCapacity];
    for (int i = 0; i < count; ++i) newItems[i] = items[i];
    delete[] items;
    items = newItems;
    capacity = newCapacity;
}

Base* Keeper::createByType(int type) {
    switch (type) {
        case POET_TYPE: return new Poet();
        case NOVELIST_TYPE: return new Novelist();
        case SCIFI_TYPE: return new SciFiWriter();
        default: throw UnknownTypeException("Неизвестный тип объекта при загрузке из файла!");
    }
}

Keeper::Keeper() : items(nullptr), count(0), capacity(0) {
    cout << "[Keeper] Вызван конструктор по умолчанию." << endl;
}

Keeper::Keeper(const Keeper& other) : items(nullptr), count(0), capacity(0) {
    cout << "[Keeper] Вызван конструктор копирования." << endl;
    if (other.count > 0) {
        capacity = other.capacity;
        items = new Base*[capacity];
        for (int i = 0; i < other.count; ++i) items[i] = other.items[i]->clone();
        count = other.count;
    }
}

Keeper& Keeper::operator=(const Keeper& other) {
    cout << "[Keeper] Вызван оператор присваивания." << endl;
    if (this != &other) {
        clear();
        delete[] items;
        items = nullptr;
        capacity = 0;
        if (other.count > 0) {
            capacity = other.capacity;
            items = new Base*[capacity];
            for (int i = 0; i < other.count; ++i) items[i] = other.items[i]->clone();
            count = other.count;
        }
    }
    return *this;
}

Keeper::~Keeper() {
    cout << "[Keeper] Вызван деструктор. Уничтожается " << count << " объект(ов)." << endl;
    clear();
    delete[] items;
}

void Keeper::add(Base* item) {
    if (!item) throw InputException("Попытка добавить пустой (nullptr) объект!");
    ensureCapacity();
    items[count++] = item;
}

void Keeper::remove(int index) {
    if (count == 0) throw EmptyContainerException("Невозможно удалить объект: контейнер пуст!");
    if (index < 0 || index >= count) throw InvalidIndexException("Некорректный индекс объекта в Keeper!");
    delete items[index];
    for (int i = index; i < count - 1; ++i) items[i] = items[i + 1];
    count--;
}

Base* Keeper::get(int index) const {
    if (index < 0 || index >= count) throw InvalidIndexException("Некорректный индекс объекта в Keeper!");
    return items[index];
}

int Keeper::getCount() const { return count; }

void Keeper::printAll() const {
    if (count == 0) throw EmptyContainerException("Контейнер пуст, нечего показывать!");
    for (int i = 0; i < count; ++i) {
        cout << "\n[" << i << "] ";
        items[i]->print();
    }
}

void Keeper::clear() {
    for (int i = 0; i < count; ++i) delete items[i];
    count = 0;
}

void Keeper::saveToFile(const char* filename) const {
    ofstream out(filename, ios::binary | ios::trunc);
    if (!out.is_open()) throw FileException("Не удалось открыть файл для записи!");
    out.write(reinterpret_cast<const char*>(&count), sizeof(int));
    for (int i = 0; i < count; ++i) {
        int type = items[i]->getType();
        out.write(reinterpret_cast<const char*>(&type), sizeof(int));
        items[i]->saveToFile(out);
    }
    out.close();
    cout << "[Keeper] Данные сохранены в файл: " << filename << endl;
}

void Keeper::loadFromFile(const char* filename) {
    ifstream in(filename, ios::binary);
    if (!in.is_open()) throw FileException("Не удалось открыть файл для чтения!");

    int newCount;
    in.read(reinterpret_cast<char*>(&newCount), sizeof(int));
    if (in.fail() || newCount < 0) throw FileException("Файл повреждён или имеет неверный формат!");

    Keeper temp;
    for (int i = 0; i < newCount; ++i) {
        int type;
        in.read(reinterpret_cast<char*>(&type), sizeof(int));
        if (in.fail()) throw FileException("Ошибка чтения типа объекта из файла!");
        Base* obj = createByType(type);
        try {
            obj->loadFromFile(in);
        } catch (...) {
            delete obj;
            throw;
        }
        temp.add(obj);
    }
    in.close();

    *this = temp;
    cout << "[Keeper] Данные загружены из файла: " << filename << " (" << count << " объект(ов))." << endl;
}

Base& Keeper::operator[](int index) const {
    if (index < 0 || index >= count) throw InvalidIndexException("Некорректный индекс объекта в Keeper!");
    return *items[index];
}
