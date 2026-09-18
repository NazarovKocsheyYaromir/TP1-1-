#include "Novelist.h"
#include "../Exceptions/Exceptions.h"
#include <iostream>
#include <cstring>
#include <string>

using namespace std;

void Novelist::copyBiography(const char* src) {
    delete[] biography;
    if (!src) src = "";
    biography = new char[strlen(src) + 1];
    strcpy(biography, src);
}

Novelist::Novelist() : Base(), birthYear(0), deathYear(0), biography(nullptr) {
    copyBiography("Биография не указана");
    cout << "[Novelist] Вызван конструктор по умолчанию." << endl;
}

Novelist::Novelist(const char* fio, char** works, int count, int birthYearIn, int deathYearIn, const char* bio)
    : Base(fio, works, count), birthYear(birthYearIn), deathYear(deathYearIn), biography(nullptr) {
    if (deathYearIn != 0 && deathYearIn < birthYearIn)
        throw InputException("Год смерти не может быть раньше года рождения!");
    copyBiography(bio);
    cout << "[Novelist] Вызван параметризованный конструктор (ФИО: " << getFio() << ")." << endl;
}

Novelist::Novelist(const Novelist& other)
    : Base(other), birthYear(other.birthYear), deathYear(other.deathYear), biography(nullptr) {
    copyBiography(other.biography);
    cout << "[Novelist] Вызван конструктор копирования (ФИО: " << getFio() << ")." << endl;
}

Novelist& Novelist::operator=(const Novelist& other) {
    cout << "[Novelist] Вызван оператор присваивания." << endl;
    if (this != &other) {
        Base::operator=(other);
        birthYear = other.birthYear;
        deathYear = other.deathYear;
        copyBiography(other.biography);
    }
    return *this;
}

Novelist::~Novelist() {
    cout << "[Novelist] Вызван деструктор (ФИО: " << getFio() << ")." << endl;
    delete[] biography;
}

void Novelist::setBirthYear(int y) {
    if (y < 0) throw InputException("Год рождения указан некорректно!");
    birthYear = y;
}

void Novelist::setDeathYear(int y) {
    if (y != 0 && y < birthYear) throw InputException("Год смерти не может быть раньше года рождения!");
    deathYear = y;
}

void Novelist::setBiography(const char* bio) {
    if (!bio) throw InputException("Биография не может быть пустой!");
    copyBiography(bio);
}

int Novelist::getBirthYear() const { return birthYear; }
int Novelist::getDeathYear() const { return deathYear; }
const char* Novelist::getBiography() const { return biography; }

void Novelist::print() const {
    cout << "----- Романист -----" << endl;
    cout << "ФИО: " << getFio() << endl;
    cout << "Годы жизни: " << birthYear << " - " << (deathYear == 0 ? "н.в." : to_string(deathYear)) << endl;
    cout << "Краткая биография: " << biography << endl;
    cout << "Произведения (" << getWorksCount() << "):" << endl;
    for (int i = 0; i < getWorksCount(); ++i) cout << "  " << (i + 1) << ". " << getWork(i) << endl;
    cout << "---------------------" << endl;
}

Base* Novelist::clone() const {
    return new Novelist(*this);
}

int Novelist::getType() const { return NOVELIST_TYPE; }
const char* Novelist::getTypeName() const { return "Романист"; }

void Novelist::saveToFile(std::ofstream& out) const {
    Base::saveToFile(out);
    out.write(reinterpret_cast<const char*>(&birthYear), sizeof(int));
    out.write(reinterpret_cast<const char*>(&deathYear), sizeof(int));
    int blen = static_cast<int>(strlen(biography));
    out.write(reinterpret_cast<const char*>(&blen), sizeof(int));
    out.write(biography, blen);
    if (out.fail()) throw FileException("Ошибка записи данных Novelist в файл!");
}

void Novelist::loadFromFile(std::ifstream& in) {
    Base::loadFromFile(in);
    in.read(reinterpret_cast<char*>(&birthYear), sizeof(int));
    in.read(reinterpret_cast<char*>(&deathYear), sizeof(int));
    int blen;
    in.read(reinterpret_cast<char*>(&blen), sizeof(int));
    if (in.fail() || blen < 0) throw FileException("Ошибка чтения данных Novelist из файла!");
    char* buf = new char[blen + 1];
    in.read(buf, blen);
    buf[blen] = '\0';
    copyBiography(buf);
    delete[] buf;
}

void Novelist::inputData() {
    Base::inputData();
    int by, dy;
    cout << "Год рождения: ";
    cin >> by;
    if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); throw InputException("Ожидалось целое число!"); }
    cout << "Год смерти (0, если автор жив): ";
    cin >> dy;
    if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); throw InputException("Ожидалось целое число!"); }
    cin.ignore(10000, '\n');
    setBirthYear(by);
    setDeathYear(dy);
    char buffer[512];
    cout << "Краткая биография: ";
    cin.getline(buffer, 512);
    setBiography(buffer);
}

void Novelist::editMenu() {
    int choice;
    do {
        cout << "\n--- Редактирование романиста (" << getFio() << ") ---" << endl;
        cout << "1. Изменить ФИО" << endl;
        cout << "2. Изменить год рождения" << endl;
        cout << "3. Изменить год смерти" << endl;
        cout << "4. Изменить биографию" << endl;
        cout << "5. Добавить произведение" << endl;
        cout << "6. Удалить произведение" << endl;
        cout << "7. Показать текущие данные" << endl;
        cout << "0. Назад" << endl;
        cout << "Выбор: ";
        cin >> choice;
        if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); throw InputException("Ожидалось целое число!"); }
        cin.ignore(10000, '\n');
        char buffer[512];
        switch (choice) {
            case 1:
                cout << "Новое ФИО: ";
                cin.getline(buffer, 512);
                setFio(buffer);
                break;
            case 2: {
                int y; cout << "Новый год рождения: "; cin >> y; cin.ignore(10000, '\n');
                setBirthYear(y);
                break;
            }
            case 3: {
                int y; cout << "Новый год смерти (0 - если жив): "; cin >> y; cin.ignore(10000, '\n');
                setDeathYear(y);
                break;
            }
            case 4:
                cout << "Новая биография: ";
                cin.getline(buffer, 512);
                setBiography(buffer);
                break;
            case 5:
                cout << "Название произведения: ";
                cin.getline(buffer, 512);
                addWork(buffer);
                break;
            case 6: {
                int idx; cout << "Индекс произведения для удаления (от 0): "; cin >> idx; cin.ignore(10000, '\n');
                removeWork(idx);
                break;
            }
            case 7:
                print();
                break;
            case 0:
                break;
            default:
                cout << "Некорректный пункт меню." << endl;
        }
    } while (choice != 0);
}
