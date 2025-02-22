#include "book.h"
#include "util.h"
#include <sstream>

Book::Book(const std::string& category, const std::string& name, double price, int qty, 
           const std::string& isbn, const std::string& author)
    : Product(category, name, price, qty), isbn_(isbn), author_(author)
{
}

std::set<std::string> Book::keywords() const {
    std::set<std::string> keys = parseStringToWords(name_);
    std::set<std::string> authorKeys = parseStringToWords(author_);
    keys.insert(authorKeys.begin(), authorKeys.end());
    keys.insert(convToLower(isbn_));
    return keys;
}

std::string Book::displayString() const {
    std::ostringstream oss;
    oss << name_ << "\n";
    oss << "Author: " << author_ << " ISBN: " << isbn_ << "\n";
    oss << price_ << " " << qty_ << " left.";
    return oss.str();
}

void Book::dump(std::ostream& os) const {
    os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" 
       << isbn_ << "\n" << author_ << std::endl;
}
