#ifndef MYDATASTORE_H
#define MYDATASTORE_H

#include "datastore.h"
#include <vector>
#include <map>
#include <set>
#include <string>

class MyDataStore : public DataStore {
public:
    MyDataStore(); // constructor
    ~MyDataStore(); // destructor

    bool userExists(const std::string& username) const; // Add this declaration

    void addProduct(Product* p) override; // add product and update keyword index
    void addUser(User* u) override; // add user and create an empty cart for them
    std::vector<Product*> search(std::vector<std::string>& terms, int type) override; // search using AND OR
    void addToCart(std::string username, Product* p); // add product to user cart
    std::vector<Product*> viewCart(std::string username); // get current cart for user
    void buyCart(std::string username); // process purchase of items in cart
    void dump(std::ostream& ofile) override; // dump products and users to file

private:
    std::vector<Product*> products_; // list of products
    std::vector<User*> users_; // list of users
    std::map<std::string, std::set<Product*> > keywordMap_; // map keyword to products
    std::map<std::string, std::vector<Product*> > userCarts_; // map username to cart items

    void addKeywords(Product* p); // update keyword index for product
};

#endif