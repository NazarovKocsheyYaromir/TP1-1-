#include "Base.h"
#include "../Exceptions/Exceptions.h"
#include <cstring>

using namespace std;

void Base::copyFio(const char* src) {
    delete[] fio;
    if (!src) src = "";
    fio = new char[strlen(src) + 1];
    strcpy(fio, src);
}

void Base::clearWorks() {
    if (works) {
        for (int i = 0; i < worksCount; ++i) delete[] works[i];
        delete[] works;
    }
    works = nullptr;
    worksCount = 0;
}

void Base::copyWorks(char* const* srcWorks, int count) {
    clearWorks();
    worksCount = count;
    if (count > 0) {
        works = new char*[count];
        for (int i = 0; i < count; ++i) {
            works[i] = new char[strlen(srcWorks[i]) + 1];
            strcpy(works[i], srcWorks[i]);
        }
    } else {
        works = nullptr;
    }
}

Base::Base() : fio(nullptr), works(nullptr), worksCount(0) {
    copyFio("Не указано");
    cout << "[Base] Вызван конструктор по умолчанию." << endl;
}

Base::Base(const char* fioIn, char** worksArr, int count)
    : fio(nullptr), works(nullptr), worksCount(0) {
    copyFio(fioIn);
    copyWorks(worksArr, count);
    cout << "[Base] Вызван параметризованный конструктор (ФИО: " << fio << ")." << endl;
}

Base::Base(const Base& other) : fio(nullptr), works(nullptr), worksCount(0) {
    copyFio(other.fio);
    copyWorks(other.works, other.worksCount);
    cout << "[Base] Вызван конструктор копирования (ФИО: " << fio << ")." << endl;
}

Base& Base::operator=(const Base& other) {
    cout << "[Base] Вызван оператор присваивания." << endl;
    if (this != &other) {
        copyFio(other.fio);
        copyWorks(other.works, other.worksCount);
    }
    return *this;
}

Base::~Base() {
    cout << "[Base] Вызван деструктор (ФИО: " << (fio ? fio : "") << ")." << endl;
    delete[] fio;
    clearWorks();
}

void Base::setFio(const char* newFio) {
    if (!newFio || strlen(newFio) == 0)
        throw InputException("ФИО не может быть пустым!");
    copyFio(newFio);
}

const char* Base::getFio() const { return fio; }

void Base::addWork(const char* work) {
    if (!work || strlen(work) == 0)
        throw InputException("Название произведения не может быть пустым!");
    char** newWorks = new char*[worksCount + 1];
    for (int i = 0; i < worksCount; ++i) newWorks[i] = works[i];
    newWorks[worksCount] = new char[strlen(work) + 1];
    strcpy(newWorks[worksCount], work);
    delete[] works;
    works = newWorks;
    worksCount++;
}

void Base::removeWork(int index) {
    if (index < 0 || index >= worksCount)
        throw InvalidIndexException("Некорректный индекс произведения!");
    delete[] works[index];
    for (int i = index; i < worksCount - 1; ++i) works[i] = works[i + 1];
    worksCount--;
    if (worksCount == 0) {
        delete[] works;
        works = nullptr;
    }
}

int Base::getWorksCount() const { return worksCount; }

const char* Base::getWork(int index) const {
    if (index < 0 || index >= worksCount)
        throw InvalidIndexException("Некорректный индекс произведения!");
    return works[index];
}

void Base::saveToFile(std::ofstream& out) const {
    int len = static_cast<int>(strlen(fio));
    out.write(reinterpret_cast<const char*>(&len), sizeof(int));
    out.write(fio, len);

    out.write(reinterpret_cast<const char*>(&worksCount), sizeof(int));
    for (int i = 0; i < worksCount; ++i) {
        int wlen = static_cast<int>(strlen(works[i]));
        out.write(reinterpret_cast<const char*>(&wlen), sizeof(int));
        out.write(works[i], wlen);
    }
    if (out.fail()) throw FileException("Ошибка записи данных Base в файл!");
}

void Base::loadFromFile(std::ifstream& in) {
    int len;
    in.read(reinterpret_cast<char*>(&len), sizeof(int));
    if (in.fail() || len < 0) throw FileException("Ошибка чтения ФИО из файла!");
    char* buf = new char[len + 1];
    in.read(buf, len);
    buf[len] = '\0';
    copyFio(buf);
    delete[] buf;

    int count;
    in.read(reinterpret_cast<char*>(&count), sizeof(int));
    if (in.fail() || count < 0) throw FileException("Ошибка чтения количества произведений!");
    clearWorks();
    worksCount = count;
    if (count > 0) {
        works = new char*[count];
        for (int i = 0; i < count; ++i) {
            int wlen;
            in.read(reinterpret_cast<char*>(&wlen), sizeof(int));
            if (in.fail() || wlen < 0) throw FileException("Ошибка чтения произведения из файла!");
            char* wbuf = new char[wlen + 1];
            in.read(wbuf, wlen);
            wbuf[wlen] = '\0';
            works[i] = wbuf;
        }
    }
}

void Base::inputData() {
    char buffer[256];
    cout << "Введите ФИО: ";
    cin.getline(buffer, 256);
    setFio(buffer);

    int count;
    cout << "Введите количество произведений: ";
    cin >> count;
    if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); throw InputException("Ожидалось целое число!"); }
    if (count < 0) throw InputException("Количество произведений не может быть отрицательным!");
    cin.ignore(10000, '\n');
    for (int i = 0; i < count; ++i) {
        cout << "Произведение " << (i + 1) << ": ";
        cin.getline(buffer, 256);
        addWork(buffer);
    }
}

std::ostream& operator<<(std::ostream& os, const Base& b) {
    b.print();
    return os;
}
