#include "SciFiWriter.h"
#include "../Exceptions/Exceptions.h"
#include <iostream>
#include <cstring>

using namespace std;

SciFiWriter::SciFiWriter() : Base(), filmsMade(false) {
    cout << "[SciFiWriter] Вызван конструктор по умолчанию." << endl;
}

SciFiWriter::SciFiWriter(const char* fio, char** works, int count, bool filmsMadeIn)
    : Base(fio, works, count), filmsMade(filmsMadeIn) {
    cout << "[SciFiWriter] Вызван параметризованный конструктор (ФИО: " << getFio() << ")." << endl;
}

SciFiWriter::SciFiWriter(const SciFiWriter& other)
    : Base(other), filmsMade(other.filmsMade) {
    cout << "[SciFiWriter] Вызван конструктор копирования (ФИО: " << getFio() << ")." << endl;
}

SciFiWriter& SciFiWriter::operator=(const SciFiWriter& other) {
    cout << "[SciFiWriter] Вызван оператор присваивания." << endl;
    if (this != &other) {
        Base::operator=(other);
        filmsMade = other.filmsMade;
    }
    return *this;
}

SciFiWriter::~SciFiWriter() {
    cout << "[SciFiWriter] Вызван деструктор (ФИО: " << getFio() << ")." << endl;
}

void SciFiWriter::setFilmsMade(bool value) { filmsMade = value; }
bool SciFiWriter::getFilmsMade() const { return filmsMade; }

void SciFiWriter::print() const {
    cout << "----- Фантаст -----" << endl;
    cout << "ФИО: " << getFio() << endl;
    cout << "Экранизации по книгам: " << (filmsMade ? "да" : "нет") << endl;
    cout << "Произведения (" << getWorksCount() << "):" << endl;
    for (int i = 0; i < getWorksCount(); ++i) cout << "  " << (i + 1) << ". " << getWork(i) << endl;
    cout << "--------------------" << endl;
}

Base* SciFiWriter::clone() const {
    return new SciFiWriter(*this);
}

int SciFiWriter::getType() const { return SCIFI_TYPE; }
const char* SciFiWriter::getTypeName() const { return "Фантаст"; }

void SciFiWriter::saveToFile(std::ofstream& out) const {
    Base::saveToFile(out);
    out.write(reinterpret_cast<const char*>(&filmsMade), sizeof(bool));
    if (out.fail()) throw FileException("Ошибка записи данных SciFiWriter в файл!");
}

void SciFiWriter::loadFromFile(std::ifstream& in) {
    Base::loadFromFile(in);
    in.read(reinterpret_cast<char*>(&filmsMade), sizeof(bool));
    if (in.fail()) throw FileException("Ошибка чтения данных SciFiWriter из файла!");
}

void SciFiWriter::inputData() {
    Base::inputData();
    char answer[16];
    cout << "Сняты ли фильмы по книгам? (да/нет): ";
    cin.getline(answer, 16);
    if (strcmp(answer, "да") == 0 || strcmp(answer, "yes") == 0) filmsMade = true;
    else if (strcmp(answer, "нет") == 0 || strcmp(answer, "no") == 0) filmsMade = false;
    else throw InputException("Ожидался ответ 'да' или 'нет'!");
}

void SciFiWriter::editMenu() {
    int choice;
    do {
        cout << "\n--- Редактирование фантаста (" << getFio() << ") ---" << endl;
        cout << "1. Изменить ФИО" << endl;
        cout << "2. Изменить признак экранизации" << endl;
        cout << "3. Добавить произведение" << endl;
        cout << "4. Удалить произведение" << endl;
        cout << "5. Показать текущие данные" << endl;
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
                char answer[16];
                cout << "Сняты ли фильмы по книгам? (да/нет): ";
                cin.getline(answer, 16);
                if (strcmp(answer, "да") == 0) setFilmsMade(true);
                else if (strcmp(answer, "нет") == 0) setFilmsMade(false);
                else throw InputException("Ожидался ответ 'да' или 'нет'!");
                break;
            }
            case 3:
                cout << "Название произведения: ";
                cin.getline(buffer, 256);
                addWork(buffer);
                break;
            case 4: {
                int idx; cout << "Индекс произведения для удаления (от 0): "; cin >> idx; cin.ignore(10000, '\n');
                removeWork(idx);
                break;
            }
            case 5:
                print();
                break;
            case 0:
                break;
            default:
                cout << "Некорректный пункт меню." << endl;
        }
    } while (choice != 0);
}
