#pragma once

#include <iostream>
#include <vector>
#include <string>

class Book{
    std::string title, author, genre, description, isbn;
    int year;
    std::vector<std::string> tags;
    double rating;

    public:
    Book(const std::string& a, const std::string& t, const std::string& g, const std::string& d, 
        int y,const std::vector<std::string>& tg,  double r, const std::string& id);

    std::string getTitle()const;
    std::string getAuthor()const;
    std::string getGenre()const;
    std::string getDescription()const;
    std::string getISBN()const;
    int getYear()const;
    double getRating()const;
    std::vector<std::string> getTags()const;

    void setRating(double r);

    void printShort()const;
    void printInFull()const;

    void writeToFile(std::ostream& filr)const;
};