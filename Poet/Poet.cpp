#include "Poet.h"
#include "../Exceptions/Exceptions.h"
#include <iostream>
#include <string>

using namespace std;

Poet::Poet() : Base(), birthYear(0), deathYear(0) {
    cout << "[Poet] Вызван конструктор по умолчанию." << endl;
}

Poet::Poet(const char* fio, char** works, int count, int birthYearIn, int deathYearIn)
    : Base(fio, works, count), birthYear(birthYearIn), deathYear(deathYearIn) {
    if (deathYearIn != 0 && deathYearIn < birthYearIn)
        throw InputException("Год смерти не может быть раньше года рождения!");
    cout << "[Poet] Вызван параметризованный конструктор (ФИО: " << getFio() << ")." << endl;
}

Poet::Poet(const Poet& other)
    : Base(other), birthYear(other.birthYear), deathYear(other.deathYear) {
    cout << "[Poet] Вызван конструктор копирования (ФИО: " << getFio() << ")." << endl;
}

Poet& Poet::operator=(const Poet& other) {
    cout << "[Poet] Вызван оператор присваивания." << endl;
    if (this != &other) {
        Base::operator=(other);
        birthYear = other.birthYear;
        deathYear = other.deathYear;
    }
    return *this;
}

Poet::~Poet() {
    cout << "[Poet] Вызван деструктор (ФИО: " << getFio() << ")." << endl;
}

void Poet::setBirthYear(int y) {
    if (y < 0) throw InputException("Год рождения указан некорректно!");
    birthYear = y;
}

void Poet::setDeathYear(int y) {
    if (y != 0 && y < birthYear) throw InputException("Год смерти не может быть раньше года рождения!");
    deathYear = y;
}

int Poet::getBirthYear() const { return birthYear; }
int Poet::getDeathYear() const { return deathYear; }

void Poet::print() const {
    cout << "----- Поэт -----" << endl;
    cout << "ФИО: " << getFio() << endl;
    cout << "Годы жизни: " << birthYear << " - " << (deathYear == 0 ? "н.в." : to_string(deathYear)) << endl;
    cout << "Основные произведения (" << getWorksCount() << "):" << endl;
    for (int i = 0; i < getWorksCount(); ++i) cout << "  " << (i + 1) << ". " << getWork(i) << endl;
    cout << "----------------" << endl;
}

Base* Poet::clone() const {
    return new Poet(*this);
}

int Poet::getType() const { return POET_TYPE; }
const char* Poet::getTypeName() const { return "Поэт"; }

void Poet::saveToFile(std::ofstream& out) const {
    Base::saveToFile(out);
    out.write(reinterpret_cast<const char*>(&birthYear), sizeof(int));
    out.write(reinterpret_cast<const char*>(&deathYear), sizeof(int));
    if (out.fail()) throw FileException("Ошибка записи данных Poet в файл!");
}

void Poet::loadFromFile(std::ifstream& in) {
    Base::loadFromFile(in);
    in.read(reinterpret_cast<char*>(&birthYear), sizeof(int));
    in.read(reinterpret_cast<char*>(&deathYear), sizeof(int));
    if (in.fail()) throw FileException("Ошибка чтения данных Poet из файла!");
}

void Poet::inputData() {
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
}

void Poet::editMenu() {
    int choice;
    do {
        cout << "\n--- Редактирование поэта (" << getFio() << ") ---" << endl;
        cout << "1. Изменить ФИО" << endl;
        cout << "2. Изменить год рождения" << endl;
        cout << "3. Изменить год смерти" << endl;
        cout << "4. Добавить произведение" << endl;
        cout << "5. Удалить произведение" << endl;
        cout << "6. Показать текущие данные" << endl;
        cout << "0. Назад" << endl;
        cout << "Выбор: ";
        cin >> choice;
        if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); throw InputException("Ожидалось целое число!"); }
        cin.ignore(10000, '\n');
        char buffer[256];
        switch (choice) {
            case 1:
                cout << "Новое ФИО: ";
                cin.getline(buffer, 256);
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
                cout << "Название произведения: ";
                cin.getline(buffer, 256);
                addWork(buffer);
                break;
            case 5: {
                int idx; cout << "Индекс произведения для удаления (от 0): "; cin >> idx; cin.ignore(10000, '\n');
                removeWork(idx);
                break;
            }
            case 6:
                print();
                break;
            case 0:
                break;
            default:
                cout << "Некорректный пункт меню." << endl;
        }
    } while (choice != 0);
}
