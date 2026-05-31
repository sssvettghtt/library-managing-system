#pragma once

#include <iostream>
#include <vector>
#include <string>

/**
 * @class Book 
 * @brief Представлява книга в библиотечната система.
 * 
 * Класът съхранява основна информация за една книга:
 * заглавие, автор, жанр, описание, ISBN,година на издаване, тагове и рейтинг.
 * 
 * Обектите от тип Book се използват в класа Library за съхранение,
 * търсене, сортиране, извеждане и записване на книги във файл.
 */

class Book{
    std::string title, author, genre, description, isbn; ///< Заглавие, автор, жанр, Кратко описание и уникален идентификатор на книга
    int year;                                           ///< Година на издаване на книгата. 
    std::vector<std::string> tags;                      ///< Списък с тагове, описваш книгата.
    double rating;                                      ///< Рейтинг на книгата в интервал от 0 до 5

    public:
    /**
     * @brief Създава нов обект от тип Book.
     * 
     * Конструкторът инициализира всички основни характеристики на книгата.
     * 
     * @param a Автор на книгата.
     * @param t Заглавие на книгата.
     * @param g Жанр на книгата.
     * @param d Описание на книгата.
     * @param y Година на издаване.
     * @param tg Списък с тагове.
     * @param r Рейтинг на книгата.
     * @param id ISBN идентификатор на книгата.
     */
    Book(const std::string& a, const std::string& t, const std::string& g, const std::string& d, 
        int y,const std::vector<std::string>& tg,  double r, const std::string& id);

    /**
     * @brief Връща заглавието на книгата.
     * 
     * @return Заглавието  на книгата
     */
    std::string getTitle()const;

    /**
     * @brief Връща автора на книгата.
     * 
     * @return Авторът  на книгата.
     */
    std::string getAuthor()const;

    /**
     * @brief Връща жанрът на книгата.
     * 
     * @return Жанрът на книгата.
     */
    std::string getGenre()const;

    /**
     * @brief Връща описанието на книгата.
     * 
     * @return Описание на книгата.
     */
    std::string getDescription()const;

    /**
     * @brief Връща ISBN идентификатора на книгата.
     * 
     * @return ISBN на книгата.
     */
    std::string getISBN()const;

    /**
     * @brief Връща годината на издаване.
     * 
     * @return Година на издаване на книгата.
     */
    int getYear()const;
    /**
     * @brief Връща рейтинга на книгата.
     * 
     * @return Рейтинг на книгата.
     */
    double getRating()const;

    /**
     * @brief Връща таговете на книгата.
     * 
     * @return Вектор с таговете на книгата.
     */
    std::vector<std::string> getTags()const;

/**
 * @brief Променя рейтинга на книгата.
 * 
 * Рейтингът се променя само ако подадената стойност е
 * в интервала от 0 до 5 включително.   
 * 
 * @param r Нов рейтинг на книгата.
 */
    void setRating(double r);

/**
 * @brief извежда кратка информация за книгата.
 * 
 * Извежда заглавие, автор, жанр и ISBN.
 */
    void printShort()const;

/**
 * @brief Извежда пълна информация за книгата.
 * 
 * Извежда заглавие, автор, жанр, година, рейтинг
 * ISBN, описание и тагове.
 */
    void printInFull()const;

/**
 * @brief Записва книгата във файл.
 * 
 * Данните се записват във формат, подходящ заповторни зареждане 
 * от класа Library.
 * 
 * Форматът е:
 * BOOK|title|author|genre|description|year|rating|isbn|tags
 * 
 * @param file Изходен поток към файла.
 */
    void writeToFile(std::ostream& file)const;
};