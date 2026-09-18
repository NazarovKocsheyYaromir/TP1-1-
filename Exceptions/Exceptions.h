#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <cstring>

// Базовый класс всех исключений программы.
// Хранит текстовое сообщение.
class KeeperException {
protected:
    char* message;
public:
    explicit KeeperException(const char* msg) {
        message = new char[strlen(msg) + 1];
        strcpy(message, msg);
    }
    KeeperException(const KeeperException& other) {
        message = new char[strlen(other.message) + 1];
        strcpy(message, other.message);
    }
    KeeperException& operator=(const KeeperException& other) {
        if (this != &other) {
            delete[] message;
            message = new char[strlen(other.message) + 1];
            strcpy(message, other.message);
        }
        return *this;
    }
    virtual const char* what() const {
        return message;
    }
    virtual ~KeeperException() {
        delete[] message;
    }
};

// Ошибка выделения динамической памяти
class MemoryAllocationException : public KeeperException {
public:
    explicit MemoryAllocationException(const char* msg = "Ошибка выделения памяти!")
        : KeeperException(msg) {}
};

// Некорректный индекс при обращении к элементу
class InvalidIndexException : public KeeperException {
public:
    explicit InvalidIndexException(const char* msg = "Некорректный индекс элемента!")
        : KeeperException(msg) {}
};

// Ошибка при работе с файлом
class FileException : public KeeperException {
public:
    explicit FileException(const char* msg = "Ошибка при работе с файлом!")
        : KeeperException(msg) {}
};

// Некорректные данные, введённые пользователем
class InputException : public KeeperException {
public:
    explicit InputException(const char* msg = "Некорректный ввод данных!")
        : KeeperException(msg) {}
};

// Контейнер пуст, а операция требует наличия элементов
class EmptyContainerException : public KeeperException {
public:
    explicit EmptyContainerException(const char* msg = "Контейнер пуст!")
        : KeeperException(msg) {}
};

// Неизвестный (некорректный) тип объекта - производного от Base
class UnknownTypeException : public KeeperException {
public:
    explicit UnknownTypeException(const char* msg = "Неизвестный тип объекта!")
        : KeeperException(msg) {}
};

#endif // EXCEPTIONS_H
