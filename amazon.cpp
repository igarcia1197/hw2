#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "product.h"
#include "db_parser.h"
#include "product_parser.h"
#include "util.h"
#include "mydatastore.h"  // use our derived datastore

using namespace std;

// simple sorter for products based on name
struct ProdNameSorter {
    bool operator()(Product* p1, Product* p2) {
        return (p1->getName() < p2->getName());
    }
};

void displayProducts(vector<Product*>& hits);

int main(int argc, char* argv[])
{
    if(argc < 2) {
        cerr << "Please specify a database file" << endl;
        return 1;
    }

    // create our derived datastore object
    MyDataStore ds;

    // set up parsers for products and users
    ProductSectionParser* productSectionParser = new ProductSectionParser;
    productSectionParser->addProductParser(new ProductBookParser);
    productSectionParser->addProductParser(new ProductClothingParser);
    productSectionParser->addProductParser(new ProductMovieParser);
    UserSectionParser* userSectionParser = new UserSectionParser;

    // set up the overall parser and add sections
    DBParser parser;
    parser.addSectionParser("products", productSectionParser);
    parser.addSectionParser("users", userSectionParser);

    // parse the database file into ds
    if( parser.parse(argv[1], ds) ) {
        cerr << "Error parsing!" << endl;
        return 1;
    }

    // print menu
    cout << "=====================================" << endl;
    cout << "Menu:" << endl;
    cout << "  AND term term ..." << endl;
    cout << "  OR term term ..." << endl;
    cout << "  ADD username search_hit_number" << endl;
    cout << "  VIEWCART username" << endl;
    cout << "  BUYCART username" << endl;
    cout << "  QUIT new_db_filename" << endl;
    cout << "====================================" << endl;

    vector<Product*> hits;  // store last search results
    bool done = false;
    while(!done) {
        cout << "\nEnter command:" << endl;
        string line;
        getline(cin, line);
        stringstream ss(line);
        string cmd;
        if(ss >> cmd) {
            if(cmd == "AND") {
                // get search terms in lower-case
                vector<string> terms;
                string term;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 0);  // AND search
                displayProducts(hits);
            }
            else if(cmd == "OR") {
                // get search terms in lower-case
                vector<string> terms;
                string term;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 1);  // OR search
                displayProducts(hits);
            }
            else if (cmd == "ADD") {
                string username;
                int index;
                if (ss >> username >> index) {
                    string extra;
                    if (ss >> extra) {
                        cout << "Invalid request" << endl;
                    } else {
                        if (index < 1 || index > (int)hits.size() || !ds.userExists(username)) {
                            cout << "Invalid request" << endl;
                        } else {
                            ds.addToCart(username, hits[index-1]);
                        }
                    }
                } else {
                    cout << "Invalid request" << endl;
                }
            }
            else if (cmd == "VIEWCART") {
                string username;
                if (ss >> username) {
                    string extra;
                    if (ss >> extra) {
                        cout << "Invalid username" << endl;
                    } else {
                        if (!ds.userExists(username)) {
                            cout << "Invalid username" << endl;
                        } else {
                            vector<Product*> cart = ds.viewCart(username);
                            if (cart.empty()) {
                                cout << "Cart is empty" << endl;
                            } else {
                                for (size_t i = 0; i < cart.size(); ++i) {
                                    cout << i+1 << ". " << cart[i]->displayString() << endl;
                                }
                            }
                        }
                    }
                } else {
                    cout << "Invalid username" << endl;
                }
            }
            else if (cmd == "BUYCART") {
                string username;
                if (ss >> username) {
                    string extra;
                    if (ss >> extra) {
                        cout << "Invalid username" << endl;
                    } else {
                        if (!ds.userExists(username)) {
                            cout << "Invalid username" << endl;
                        } else {
                            ds.buyCart(username);
                        }
                    }
                } else {
                    cout << "Invalid username" << endl;
                }
            }

            else if(cmd == "QUIT") {
                // dump updated database to file
                string filename;
                if(ss >> filename) {
                    ofstream ofile(filename.c_str());
                    ds.dump(ofile);
                    ofile.close();
                }
                done = true;
            }
            else {
                cout << "Unknown command" << endl;
            }
        }
    }
    return 0;
}

void displayProducts(vector<Product*>& hits)
{
    int resultNo = 1;
    if(hits.begin() == hits.end()) {
        cout << "No results found!" << endl;
        return;
    }
    sort(hits.begin(), hits.end(), ProdNameSorter());
    for(vector<Product*>::iterator it = hits.begin(); it != hits.end(); ++it) {
        cout << "Hit " << setw(3) << resultNo << endl;
        cout << (*it)->displayString() << endl;
        cout << endl;
        resultNo++;
    }
}

