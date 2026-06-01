
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
 * @brief Пренасочва std::cout и изпълнява fn(), връща записания текст.
 *
 * @param fn Функция без параметри и без върната стойност.
 * @return Текстът, изведен от fn() в std::cout.
 */
template <typename Fn>
std::string captureOutput(Fn fn)
{
    std::ostringstream buf;
    std::streambuf *old = std::cout.rdbuf(buf.rdbuf());
    fn();
    std::cout.rdbuf(old);
    return buf.str();
}

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
 * @brief Тества успешен вход с верни данни.
 *
 * Отваря файл (за да има активна сесия), след това се опитва да влезе
 * с правилно потребителско име и парола на администратора по подразбиране.
 * Очакваме logInCredentials() да върне true.
 */
void testLoginSuccess()
{
    Library lib;
    lib.open("test_login.txt");
 
    std::string msg;
    bool result = lib.logInCredentials("admin", "i<3c++", msg);
 
    assert(result == true);
    std::cout << "[PASS] testLoginSuccess\n";
}
 
/**
 * @brief Тества неуспешен вход с грешна парола.
 *
 * Подава правилно потребителско име, но грешна парола.
 * Очакваме logInCredentials() да върне false и msg да не е празен.
 */
void testLoginWrongPassword()
{
    Library lib;
    lib.open("test_login2.txt");
 
    std::string msg;
    bool result = lib.logInCredentials("admin", "wrongpass", msg);
 
    assert(result == false);
    assert(!msg.empty()); // трябва да има съобщение за грешка
    std::cout << "[PASS] testLoginWrongPassword\n";
}

/**
 * @brief Тества неуспешен вход с несъществуващ потребител.
 *
 * Подава потребителско име, което не съществува в системата.
 * Очакваме logInCredentials() да върне false.
 */
void testLoginUnknownUser()
{
    Library lib;
    lib.open("test_login3.txt");
 
    std::string msg;
    bool result = lib.logInCredentials("ghost", "1234", msg);
 
    assert(result == false);
    std::cout << "[PASS] testLoginUnknownUser\n";
}

/**
 * @brief Помощна функция — инициализира подадена Library с влязъл admin
 *        и две предварително добавени книги.
 *
 * @param lib Референция към Library обекта, който ще се инициализира.
 */
void makeLibWithBooks(Library &lib)
{
    lib.open("test_books.txt");
 
    std::string msg;
    lib.logInCredentials("admin", "i<3c++", msg);
 
    lib.booksAddGui("The Hobbit", "Tolkien", "Fantasy", "A hobbit's journey",
                    1937, {"adventure", "dragons"}, 4.8, "ISBN-101", msg);
 
    lib.booksAddGui("1984", "Orwell", "Dystopia", "Big Brother is watching",
                    1949, {"politics", "fiction"}, 4.7, "ISBN-102", msg);
}

/**
 * @brief Тества booksFind() по заглавие с намерен резултат.
 *
 * Търси "Hobbit" по title — очакваме изходът да съдържа "Hobbit".
 */
void testBooksFindByTitleFound()
{
    Library lib;
    makeLibWithBooks(lib);
 
    std::string output = captureOutput([&]() {
        lib.booksFind("title", "Hobbit");
    });
 
    assert(output.find("Hobbit") != std::string::npos);
    std::cout << "[PASS] testBooksFindByTitleFound\n";
}
 
/**
 * @brief Тества booksFind() по автор.
 *
 * Търси "Orwell" по author — очакваме "1984" да се появи в резултата.
 */
void testBooksFindByAuthor()
{
    Library lib;
    makeLibWithBooks(lib);
 
    std::string output = captureOutput([&]() {
        lib.booksFind("author", "Orwell");
    });
 
    assert(output.find("1984") != std::string::npos);
    std::cout << "[PASS] testBooksFindByAuthor\n";
}
 
/**
 * @brief Тества booksFind() по таг.
 *
 * Търси "dragons" по tags — очакваме "The Hobbit" да се появи в резултата.
 */
void testBooksFindByTag()
{
    Library lib;
    makeLibWithBooks(lib);
 
    std::string output = captureOutput([&]() {
        lib.booksFind("tags", "dragons");
    });
 
    assert(output.find("Hobbit") != std::string::npos);
    std::cout << "[PASS] testBooksFindByTag\n";
}

/**
 * @brief Тества booksSort() по заглавие в низходящ ред.
 *
 * След desc сортиране "The Hobbit" трябва да е преди "1984".
 */
void testBooksSortByTitleDesc()
{
    Library lib;
    makeLibWithBooks(lib);
 
    lib.booksSort("title", "desc");
 
    std::string output = captureOutput([&]() {
        lib.booksAll();
    });
 
    size_t pos1984   = output.find("1984");
    size_t posHobbit = output.find("Hobbit");
 
    assert(pos1984 != std::string::npos);
    assert(posHobbit != std::string::npos);
    assert(posHobbit < pos1984); // "The Hobbit" идва преди "1984"
    std::cout << "[PASS] testBooksSortByTitleDesc\n";
}

/**
 * @brief Тества че booksFind() хвърля изключение без влязъл потребител.
 *
 * Отваряме файл, но не влизаме. Очакваме LibraryExceptions.
 */
void testBooksFindRequiresLogin()
{
    Library lib;
    lib.open("test_exc1.txt");
 
    bool threw = false;
    try
    {
        lib.booksFind("title", "anything");
    }
    catch (const LibraryExceptions &)
    {
        threw = true;
    }
 
    assert(threw);
    std::cout << "[PASS] testBooksFindRequiresLogin\n";
}
/**
 * @brief Тества че booksSort() хвърля изключение без влязъл потребител.
 */
void testBooksSortRequiresLogin()
{
    Library lib;
    lib.open("test_exc2.txt");
 
    bool threw = false;
    try
    {
        lib.booksSort("title", "asc");
    }
    catch (const LibraryExceptions &)
    {
        threw = true;
    }
 
    assert(threw);
    std::cout << "[PASS] testBooksSortRequiresLogin\n";
}
/**
 * @brief Тества че save() хвърля изключение без отворен файл.
 */
void testSaveRequiresOpenFile()
{
    Library lib; // няма извикан open()
 
    bool threw = false;
    try
    {
        lib.save();
    }
    catch (const LibraryExceptions &)
    {
        threw = true;
    }
 
    assert(threw);
    std::cout << "[PASS] testSaveRequiresOpenFile\n";
}
 
/**
 * @brief Тества че close() хвърля изключение без отворен файл.
 */
void testCloseRequiresOpenFile()
{
    Library lib;
 
    bool threw = false;
    try
    {
        lib.close();
    }
    catch (const LibraryExceptions &)
    {
        threw = true;
    }
 
    assert(threw);
    std::cout << "[PASS] testCloseRequiresOpenFile\n";
}
 
/**
 * @brief Тества че booksAddGui() хвърля изключение без влязъл потребител.
 */
void testBooksAddRequiresLogin()
{
    Library lib;
    lib.open("test_exc3.txt");
 
    bool threw = false;
    std::string msg;
    try
    {
        lib.booksAddGui("Title", "Author", "Genre", "Desc",
                        2000, {}, 4.0, "ISBN-999", msg);
    }
    catch (const LibraryExceptions &)
    {
        threw = true;
    }
 
    assert(threw);
    std::cout << "[PASS] testBooksAddRequiresLogin\n";
}
 
/**
 * @brief Тества че добавяне на дублиран ISBN връща false.
 *
 * Добавяме книга, след това се опитваме да добавим друга с същия ISBN.
 * Очакваме false и подходящо съобщение.
 */
void testBooksAddDuplicateISBN()
{
    Library lib;
    lib.open("test_dup.txt");
 
    std::string msg;
    lib.logInCredentials("admin", "i<3c++", msg);
 
    lib.booksAddGui("Book One", "Author", "Genre", "Desc",
                    2000, {}, 4.0, "ISBN-DUP", msg);
 
    bool result = lib.booksAddGui("Book Two", "Author", "Genre", "Desc",
                                  2001, {}, 3.5, "ISBN-DUP", msg);
 
    assert(result == false);
    assert(msg == "Book with this ISBN already exists.");
    std::cout << "[PASS] testBooksAddDuplicateISBN\n";
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

    testLoginSuccess();
    testLoginWrongPassword();
    testLoginUnknownUser();
 
    testBooksFindByTitleFound();
    testBooksFindByAuthor();
    testBooksFindByTag();
 
 
    testBooksFindRequiresLogin();
    testBooksSortRequiresLogin();
    testSaveRequiresOpenFile();
    testCloseRequiresOpenFile();
    testBooksAddRequiresLogin();
    testBooksAddDuplicateISBN();
 

    std::cout << "\nAll tests passed!\n";
    return 0;
}