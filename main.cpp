#include <iostream>
#include "Base/Base.h"
#include "Poet/Poet.h"
#include "Novelist/Novelist.h"
#include "SciFiWriter/SciFiWriter.h"
#include "Keeper/Keeper.h"
#include "Exceptions/Exceptions.h"

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// Настраивает консоль Windows на кодировку UTF-8, чтобы кириллица
// отображалась и вводилась корректно.
void setupConsole() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

int readInt(const char* prompt) {
    int value;
    cout << prompt;
    cin >> value;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(10000, '\n');
        throw InputException("Ожидалось целое число!");
    }
    cin.ignore(10000, '\n');
    return value;
}

void printMainMenu() {
    cout << "\n=========== МЕНЮ ===========" << endl;
    cout << "1. Добавить объект" << endl;
    cout << "2. Удалить объект" << endl;
    cout << "3. Показать все объекты" << endl;
    cout << "4. Редактировать объект" << endl;
    cout << "5. Показать количество объектов" << endl;
    cout << "6. Сохранить в файл" << endl;
    cout << "7. Загрузить из файла" << endl;
    cout << "8. Очистить контейнер" << endl;
    cout << "0. Выход" << endl;
    cout << "=============================" << endl;
    cout << "Ваш выбор: ";
}

void addObject(Keeper& k) {
    cout << "\nВыберите тип объекта:\n" << endl;
    cout << "1. Поэт" << endl;
    cout << "2. Романист" << endl;
    cout << "3. Фантаст" << endl;
    int type = readInt("Ваш выбор: ");

    Base* obj = nullptr;
    switch (type) {
        case POET_TYPE: obj = new Poet(); break;
        case NOVELIST_TYPE: obj = new Novelist(); break;
        case SCIFI_TYPE: obj = new SciFiWriter(); break;
        default:
            throw UnknownTypeException("Такого типа объекта не существует!");
    }

    try {
        obj->inputData();
    } catch (...) {
        delete obj;
        throw;
    }

    k.add(obj);
    cout << "Объект успешно добавлен." << endl;
}

int main() {
    setupConsole();

    Keeper keeper;

    int choice;
    do {
        try {
            printMainMenu();
            choice = readInt("");
            switch (choice) {
                case 1:
                    addObject(keeper);
                    break;
                case 2: {
                    int idx = readInt("Индекс объекта для удаления (от 0): ");
                    keeper.remove(idx);
                    cout << "Объект удалён." << endl;
                    break;
                }
                case 3:
                    keeper.printAll();
                    break;
                case 4: {
                    int idx = readInt("Индекс объекта для редактирования (от 0): ");
                    Base& obj = keeper[idx];
                    obj.editMenu();
                    break;
                }
                case 5:
                    cout << "Количество объектов в контейнере: " << keeper.getCount() << endl;
                    break;
                case 6: {
                    char filename[256];
                    cout << "Введите имя файла для сохранения: ";
                    cin.getline(filename, 256);
                    keeper.saveToFile(filename);
                    break;
                }
                case 7: {
                    char filename[256];
                    cout << "Введите имя файла для загрузки: ";
                    cin.getline(filename, 256);
                    keeper.loadFromFile(filename);
                    break;
                }
                case 8:
                    keeper.clear();
                    cout << "Контейнер очищен." << endl;
                    break;
                case 0:
                    cout << "Завершение работы программы..." << endl;
                    break;
                default:
                    cout << "Некорректный пункт меню, попробуйте снова." << endl;
            }
        } catch (const KeeperException& ex) {
            cout << "[ОШИБКА] " << ex.what() << endl;
        } catch (const std::exception& ex) {
            cout << "[ОШИБКА] " << ex.what() << endl;
        }
    } while (choice != 0);

    return 0;
}
