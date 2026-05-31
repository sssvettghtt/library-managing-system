#include "Book.hpp"

/**
 * @brief Инициализира всички полета на книгата.
 */
Book::Book(const std::string &a, const std::string &t, const std::string &g, const std::string &desc, int y, const std::vector<std::string> &tg,
         double r, const std::string &id)
        : author(a), title(t), genre(g), description(desc), year(y), tags(tg), rating(r), isbn(id) {}

    std::string Book::getTitle() const { return title; }
    std::string Book::getAuthor() const { return author; }
    std::string Book::getGenre() const { return genre; }
    std::string Book::getDescription() const { return description; }
    std::string Book::getISBN() const { return isbn; }
    int Book::getYear() const { return year; }
    double Book::getRating() const { return rating; }
    std::vector<std::string> Book::getTags() const { return tags; }

 /**
 * @brief Променя рейтинга само ако стойността е валидна.
 */
    void Book::setRating(double r)
    {
        if (r >= 0 && r <= 5)
            rating = r;
    }

    void Book::printShort() const
    {
        std::cout << title << " by " << author << "[" << genre << "] ISBN: " << isbn << std::endl;
    }

/**
 * @brief Извежда пълната информация за книгата в конзолата.
 */
    void Book::printInFull() const
    {
        std::cout << "Title: " << title
                  << "\n Author: " << author
                  << "\n Genre: " << genre
                  << "\n Year: " << year
                  << "\n Rating: " << rating
                  << "\n ISBN: " << isbn
                  << "\n Description: " << description
                  << "\n Tags: ";
        for (const std::string &tag : tags)
        {
            std::cout << tag << " ";
        }
        std::cout << std::endl;
    }

/**
 * @brief Записва книгата във файл във формат BOOK|... .
 */
    void Book::writeToFile(std::ostream &file) const
    {
        file << "BOOK|"
             << title << "|"
             << author << "|"
             << genre << "|"
             << description << "|"
             << year << "|"
             << rating << "|"
             << isbn << "|";

        for (size_t i = 0; i < tags.size(); i++)
        {
            file << tags[i];
            if (i != tags.size() - 1)
                file << ", ";
        }

        file << std::endl;
    }