#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <limits>
#include <cstdlib>
#include <conio.h>

#include "Book.hpp"
#include "User.hpp"
#include "LibraryExceptions.hpp"

/**
 * @class Library
 * @brief Представлява основната система за управление на библиотеката.
 * 
 * Класът "Library" съхранява всички книги и потребители в системата.
 * Той е отговорен за отварянето и запазването на файлове, влизането/излизането
 * на потребителите, търсенето, сортирането, добавянето и премахването на книги 
 * и управлението на потребителите.
 * 
 * Класът поддържа два типа потребители 
 *  -Администратори, които могат да добавят/ премахват потребители и книги.
 *  -Клиенти, които могат да преглеждат, търсят и сортират книги.
 */

class Library
{
    std::vector<Book> books;                ///< Съхранява всички книги, заредени в библиотеката.
    std::map<std::string, User *> users;    ///< Запазва всички потребители по потребителско име.
    User *currentUser = nullptr;            ///< Указател към текущо влезлия потребител. 
    std::string currentFilePath = "";       ///< Път до текущо отворения файл.
    bool isFileOpen = false;                ///< Показва дали даден файл е отворен в момента.

    /**
     * @brief Преобразува низ в малки букви.
     * 
     * Използва се главно за сортиране или сравнение без отчитане на 
     * малки и големи букви.
     * 
     * @param str Низът, който ще бъде преобразуван.
     * @return Версията на низа с малки букви.
     */
    static std::string toLower(std::string str);

    /**
     * @brief Изчиства всички заредени данни от системата.
     * 
     * Изтрива динамично разпределени потребители, изчиства колекцията от книги
     * и нулира текущия файл и текущия потребител.
     */
    void clearData();

    /**
     * @brief Проверява дали даден файл е отворен в момента.
     * 
     * @throws LibraryExceptions, ако в момента няма отворен файл.
     */
    void requireOpenFile() const;

    /**
     * @brief Проверява дали има влязъл потребител.
     * 
     * @throws LibraryExceptions, ако в момента няма влязъл потребител.
     */
    void requireLogIn() const;

    /**
     * @brief Проверява дали текущият потребител е администратор.
     *
     * Първо проверява дали има влязъл потребите. След това проверява
     * дали потребителят има администраторски права.
     * 
     * @throws LibraryExceptions, ако няма влязъл потребител или потребителят няма администраторски права.
     */
    void requireAdmin() const;

    /**
     * @brief Разделя низ на части по зададен разделител.
     *
     * Използва се при четене на данни от файл, например при разделяне
     * на редове с информация за книги и потребители.
     *
     * @param str Низът, който ще бъде разделен.
     * @param delimiter Символът, по който се разделя низът.
     * @return Вектор от отделните части на низа.
     */
    std::vector<std::string> split(const std::string &str, char delimiter) const;

    /**
     * @brief Извлича името на файла от пълен път.
     * 
     * Например, от път "folder/library.txt"  връща само "library.txt".
     * 
     * @param path Пълен или относителен път до файл.
     * @return Името на файла без директориите преди него
     */
    static std::string baseName(const std::string &path);

    /**
     * @brief Създава потребител admin по подразбиране, ако такъв липсва.
     * 
     * Ако в системата няма потребител с име "admin", методът създава
     * администраторски акаунт по подразбиране.
     */
    void ensureDefaultAdmin();

    /**
    * @brief Чете парола от конзолата, без да я показва.
    * 
    * Всеки въведен символ се показва като '*'.
    * Поддържа изтриване с Backspace.
    * 
    * @return Въведената парола като низ.
    */
    static std::string hiddenPasswordInput();

public:
    /**
     * @brief Създава обект от тип Library.
     */
    Library() = default; // no magic numbers

    /**
     * @brief Унищожава обекта Library.
     *
     * Освобождава динамично заделената памет за потребителите.
     */
    ~Library();

    /**
     * @brief Забранява копирането на Library.
     *
     * Класът притежава динамично заделени User обекти чрез указатели.
     * Копирането е забранено, за да не се получи двойно изтриване.
     */
    Library(const Library& other) = delete;

    /**
     * @brief Забранява присвояването на Library.
     *
     * Причината е, че класът управлява динамична памет чрез User*.
     */
    Library& operator=(const Library& other) = delete;

    /**
     * @brief Отваря файл с данни за библиотеката.
     * 
     * Ако файлът съществува, зарежда книгите и потребителите от него.
     * Ако файлът не съшествува, създава нов празен файл.
     * 
     * @param path Път до файла, който трябва да бъде отворен.
     * @throws LibraryExceptions Ако файлът не може да бъде създаден.
     */
    void open(const std::string &path);

    /**
     * @brief Затваря текущо отворения файл.
     * 
     * Изчиства заредените данни и нулира информацията за текущия файл.
     * 
     * @throws LibraryExceptions, ако няма отворен файл.
     */
    void close();

    /**
     * @brief Запазва текущите данни в отворения файл.
     * 
     * Записва всички книги и потребители в текущо отворения файл.
     * 
     * @throws LibraryExceptions, ако няма отворен файл или файлът не може
     * да бъде отворен за запис.
     */
    void save();

    /**
     * @brief Запазва текущите данни в нов файл.
     * 
     * Променя текущия път към файла и записва данните в новия файл.
     * @param path Път до новия файл.
     * @throws LibraryExceptions Ако няма отворен файл или записът е неуспешен.
     */
    void saveAs(const std::string &path);

    // raylib ne poddurja bulgarsi za help
    /**
     * @brief Извежда помощна информация за командите.
     * 
     * Показва наличните команди за файлове, потребители и книги.
     */
    void help();

    /**
     * @brief Стартира конзолния режим на програмата.
     * 
     * Чете команди от конзолата и изпълнява съответните действия.
     */
    void run();

    /**
     * @brief Прекратява изпълнението на програмата.
     */
    void exit();

    /**
     * @brief Вход в сисстемата чрез подадени потребителско име и парола.
     * 
     * Този метод е създаден основно за Raylib интерфейса, където данните 
     * се въвеждат втекстови полета, а не директни през конзолата.
     * 
     * @param inputUsername Въведено потребителско име.
     * @param inputPassword Въведена парола.
     * @param message Съобщение с резултата от опита за вход.
     * 
     * @return true Ако входът е успешен.
     * @return false Ако потребителското име или паролата са грешни.
     */
    bool logInCredentials(const std::string &inputUsername, const std::string &inputPassword, std::string &message);

    /**
     * @brief Проверява дали текущият потребител е администратор.
     * 
     * Използва се най-вече в графични я интерфейс, за да се покзват 
     * администраторските бутони само за администратори.
     * 
     * @return true Ако има влязъл потребител и той е администратор.
     * @return false Ако няма влязъл потребител или потребителят не е администратор.
     */
    bool isCurrentUserAdmin() const;

    /**
     * @brief Добавя нова книга чрез подадени данни.
     * 
     * Методът епредназначен за Raylib интерфейса. Само администратор
     * може да добавя книги.
     * 
     * @param title Заглавие на книгата.
     * @param author Автор на книгата.
     * @param genre Жанр на книгата.
     * @param description Описание на книгата.
     * @param year Година на издаване.
     * @param tags Тагове на книгата.
     * @param rating Рейтинг на книгата.
     * @param isbn ISBN на книгата.
     * @param message Съобщение с резултата от операцията.
     * 
     * @return true Ако книгата е добавена успешно.
     * @return false Ако книга със същия ISBN вече съществува.
     * 
     * @throws LibraryExceptions Ако няма влязъл потребител или потребителят не е администратор
     */
    bool booksAddGui(const std::string &title,
                     const std::string &author,
                     const std::string &genre,
                     const std::string &description,
                     int year,
                     const std::vector<std::string> &tags,
                     double rating,
                     const std::string &isbn,
                     std::string &message);
    
    /**
     * @brief Вход в системата през конзолата.
     * 
     * Чете потребителски име и парола от конзолата.
     * Паролата се скрива чрез символа '*'.
     */
    void logIn();

    /**
     * @brief Изход от текущия потребителски акаунт.
     * 
     * @throws LibraryExceptions Ако няма влязъл потребител.
     */
    void logOut();

    /**
     * @brief Добавя нов потребител в системата.
     * 
     * Само администратор може да добавя нови потребители.
     *
     * @param newUsername Потребителско име на новия потребител.
     * @param newPassword Парола на новия потребител.
     * @throws LibraryExceptions Ако няма влязъл потребител или потребителят не е администратор.
     */
    void addUser(const std::string &newUsername, const std::string &newPassword);

    /**
     * @brief Премахва потребител от системата.
     * 
     * Само администратор може да премахва потребители.
     * Текущо влезналият потребител не може да премахне собствения си акаунт.
     * 
     * @param usernameToRemove Потребителско име на потребителя, който трябва да бъде премахнат.
     * @throws LibraryExceptions Ако няма влязъл потребител или потребителят не е администратор.
     */
    void removeUser(const std::string &usernameToRemove);

    /**
     * @brief Извежда всички книги в библиотеката.
     * 
     * Отпечатва пълна информация за всяка книга.
     */
    void booksAll();

    /**
     * @brief Извежда подробна информация за книга по ISBN.
     * @param searchISBN ISBN на книгата, която се търси.
     * @throws LibraryExceptions Ако няма влязъл потребител.
     */
    void booksView(const std::string &searchISBN);

    /**
     * @brief Търся книга по зададен критерий.
     * 
     *  Поддържаните критерии са "title", "author" и "tags".
     * 
     * @param option Критерий за търсене.
     * @param keyword Търсена стойност.
     * 
     * @throws LibraryExceptions Ако няма влязъл потребител.
     */
    void booksFind(const std::string &option, const std::string &keyword);

    /**
     * @brief Сортира книгите по зададен критерий.
     * 
     * Поддържаните критерии са "title", "author", "year" и "rating".
     * Редът може да бъде "asc" или "desc",
     * 
     * @param option Критерий за сортиране.
     * @param order Ред на сортиране.
     * @throws LibraryExceptions Ако няма влязъл потребител.
     */
    void booksSort(const std::string &option, const std::string &order);

    /**
     * @brief Добавя нова книга чрез конзолно въвеждане.
     * 
     * Само администратор може да добавя книги.
     * 
     * @throws LibraryExceptions Ако няма влязъл потребител или потребителят не е администратор.
     */
    void booksAdd();

    /**
     * @brief Премахва книга от системата по ISBN
     * 
     * Само администратор може да премахва книги.
     * 
     * @param targetISBN ISBN на книгата, която трябва да бъде премахната.
     */
    void booksRemove(const std::string &targetISBN);
};