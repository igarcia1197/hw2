#include "mydatastore.h"
#include "util.h"
#include "product.h"
#include "user.h"
#include <algorithm>
#include <iostream>
using namespace std;

MyDataStore::MyDataStore() {
    // empty constructor
}

MyDataStore::~MyDataStore() {
    // delete all products
    for (size_t i = 0; i < products_.size(); i++)
        delete products_[i];
    // delete all users
    for (size_t i = 0; i < users_.size(); i++)
        delete users_[i];
}

bool MyDataStore::userExists(const std::string& username) const {
    std::string uname = convToLower(username);
    return userCarts_.find(uname) != userCarts_.end();
}
void MyDataStore::addProduct(Product* p) {
    products_.push_back(p); // add product to list
    addKeywords(p); // update keyword map for product
}

void MyDataStore::addUser(User* u) {
    users_.push_back(u); // add user to list
    string uname = convToLower(u->getName()); // make username lower case
    userCarts_[uname] = vector<Product*>(); // create empty cart for user
}

void MyDataStore::addKeywords(Product* p) {
    set<string> keys = p->keywords(); // get keywords from product
    for (set<string>::iterator it = keys.begin(); it != keys.end(); ++it)
        keywordMap_[*it].insert(p); // add product under each keyword
}

vector<Product*> MyDataStore::search(vector<string>& terms, int type) {
    vector<Product*> results;
    set<Product*> hitSet;
    if (terms.empty())
        return results; // no terms means empty results

    if (type == 0) { // AND search: use the first term as is terms are already lower-case
        if (keywordMap_.find(terms[0]) != keywordMap_.end())
            hitSet = keywordMap_[terms[0]];
        else
            return results;  // first term not found

        for (size_t i = 1; i < terms.size(); i++) {
            if (keywordMap_.find(terms[i]) != keywordMap_.end()) {
                set<Product*> temp = keywordMap_[terms[i]];
                set<Product*> newSet;
                for (auto it = hitSet.begin(); it != hitSet.end(); ++it)
                    if (temp.find(*it) != temp.end())
                        newSet.insert(*it); // keep products that appear in both sets
                hitSet = newSet;
            } else {
                hitSet.clear(); // term not found so no matches
                break;
            }
        }
    } else { // OR search: union over all terms
        for (size_t i = 0; i < terms.size(); i++) {
            if (keywordMap_.find(terms[i]) != keywordMap_.end()) {
                hitSet.insert(keywordMap_[terms[i]].begin(), keywordMap_[terms[i]].end());
            }
        }
    }
    
    for (auto it = hitSet.begin(); it != hitSet.end(); ++it)
        results.push_back(*it); // convert set to vector
    return results;
}



void MyDataStore::addToCart(std::string username, Product* p) {
    std::string uname = convToLower(username);
    if (userCarts_.find(uname) != userCarts_.end()) {
        userCarts_[uname].push_back(p);
    }
}

std::vector<Product*> MyDataStore::viewCart(std::string username) {
    std::string uname = convToLower(username);
    if (userCarts_.find(uname) != userCarts_.end()) {
        return userCarts_[uname];
    }
    return std::vector<Product*>();
}



void MyDataStore::buyCart(std::string username) {
    std::string uname = convToLower(username);
    User* user = nullptr;
    for (User* u : users_) {
        if (convToLower(u->getName()) == uname) {
            user = u;
            break;
        }
    }
    if (!user) return;

    std::vector<Product*>& cart = userCarts_[uname];
    std::vector<Product*> newCart;
    for (Product* p : cart) {
        if (p->getQty() > 0 && user->getBalance() >= p->getPrice()) {
            p->subtractQty(1);
            user->deductAmount(p->getPrice());
        } else {
            newCart.push_back(p);
        }
    }
    userCarts_[uname] = newCart;
}

void MyDataStore::dump(ostream& ofile) {
    // Write the opening products tag
    ofile << "<products>" << std::endl;

    // Loop through every product and call its dump mthod
    for (size_t i = 0; i < products_.size(); i++) {
        products_[i]->dump(ofile);
    }

    // Write the closing products tag
    ofile << "</products>" << std::endl;

    // Write the opening user tag
    ofile << "<users>" << std::endl;

    // Loop through every user and call it method
    for (size_t i = 0; i < users_.size(); i++) {
        users_[i]->dump(ofile);
    }

    // Write the closing users tag
    ofile << "</users>" << std::endl;
}
