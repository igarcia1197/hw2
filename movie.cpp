#include "movie.h"
#include "util.h"
#include <sstream>

Movie::Movie(const std::string& category, const std::string& name, double price, int qty,
             const std::string& genre, const std::string& rating)
    : Product(category, name, price, qty), genre_(genre), rating_(rating)
{
}

std::set<std::string> Movie::keywords() const {
    std::set<std::string> keys = parseStringToWords(name_);
    keys.insert(convToLower(genre_));
    return keys;
}

std::string Movie::displayString() const {
    std::ostringstream oss;
    oss << name_ << "\n";
    oss << "Genre: " << genre_ << " Rating: " << rating_ << "\n";
    oss << price_ << " " << qty_ << " left.";
    return oss.str();
}

void Movie::dump(std::ostream& os) const {
    os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n"
       << genre_ << "\n" << rating_ << std::endl;
}
