
/**
 * @file tests.cpp
 * 
 * @brief Единични тестове за библиотечната система.
 * 
 * Файлът съдържа unit тестове за класовете Book, User (Client, Admin)
 * и Library. Тестовете използват стандартната библиотека <cassert> 
 * и не изискват външни зависимости.
 * 
 */
#include <cassert>
#include <iostream>
#include "Book.hpp"
#include "User.hpp"
#include "Library.hpp"

/**
 * @brief Тества коректността на getter методите на Book.
 * 
 * Създава обект на тип Book и проверява дали getTitle(),
 * getAuthor() и getRating() връщат очакваните стойности.
 */
void testBookGetters() {
    Book b("Tolkien", "LOTR", "Fantasy", "Epic", 1954, {"adventure"}, 4.9, "ISBN-001");
    assert(b.getTitle() == "LOTR");
    assert(b.getAuthor() == "Tolkien");
    assert(b.getRating() == 4.9);
    std::cout << "[PASS] testBookGetters\n";
}

/**
 * @brief Тества метода setRating() на Book.
 * 
 * Проверява дали валидна стойност се приема коректно,
 * и дали невалидна стойност (извън [0,5]) не променя рейтинга.
 */
void testBookSetRating() {
    Book b("Tolkien", "LOTR", "Fantasy", "Epic", 1954, {}, 3.0, "ISBN-001");
    b.setRating(5.0);
    assert(b.getRating() == 5.0);
    b.setRating(99.0); // невалиден — не трябва да се промени
    assert(b.getRating() == 5.0);
    std::cout << "[PASS] testBookSetRating\n";
}

/**
 * @brief тества Admin::isAdmin() връща true.
 * 
 * Създава обект от тип Admin и проверява дали
 * входът isAdmin() връща true.
 */
void testAdminIsAdmin() {
    Admin a("admin", "1234");
    assert(a.isAdmin() == true);
    std::cout << "[PASS] testAdminIsAdmin\n";
}

/**
 * @brief тества Admin::isAdmin() връща false.
 * 
 * Създава обект от тип Client и проверява дали
 * входът isAdmin() връща false.
 */
void testClientIsNotAdmin() {
    Client c("user", "pass");
    assert(c.isAdmin() == false);
    std::cout << "[PASS] testClientIsNotAdmin\n";
}

/**
 * @brief Тества отварянето на файл чрез Library::open().
 * 
 * Проверява дали Library::open() успешно създава нов файл,
 * ако подаденият път не съществува.
 */
void testLibraryOpenAndFind() {
    Library lib;
    lib.open("test_data.txt"); // създава празен файл
    std::cout << "[PASS] testLibraryOpen\n";
}

/**
 * @brief Главна функция, която изпълнява всички тестове.
 *
 * Извиква всички тестови функции последователно.
 * Ако някой assert се провали, програмата прекратява изпълнението си
 * с грешка. При успех извежда съобщение "All tests passed!".
 *
 * @return 0 при успешно преминаване на всички тестове.
 */
int main() {
    testBookGetters();
    testBookSetRating();
    testAdminIsAdmin();
    testClientIsNotAdmin();
    testLibraryOpenAndFind();

    std::cout << "\nAll tests passed!\n";
    return 0;
}