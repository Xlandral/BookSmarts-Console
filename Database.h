/*
 * Database.h
 *
 *  Created on: Jan 25, 2023
 *      Author: Ruben Rios
 *      	Cheyenne Westbrook
 *      	David McMahon
 *      	Connor Schaffer
 *
 */
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <list>
#include <cmath>
#include <limits>
#include "sqlite3.h"
#include "Book.h"
#include "User.h"
#include "Shopper.h"

#ifndef DATABASE_H_
#define DATABASE_H_

using namespace std;

class Database{

public:
	list<Book> BookList;
	list<Book> UserList;
	list<User> UserDetails;




	Database(){

	}
	~Database(){

	}

	static int callback(void *data, int argc, char **argv, char **azColName){
	   int i;
	   fprintf(stderr, "%s: ", (const char*)data);

	   for(i = 0; i<argc; i++){
	      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	   }

	   printf("\n");
	   return 0;
	}



	void CreateShoppersTable() {
	        sqlite3 *ppDb;
	        char *zErrMsg = 0;
	        string createTable;
	        int rc;
	        const char* data = "Callback function called";
	        try {
	            sqlite3_open("Shoppers.db", &ppDb);
	            createTable = "CREATE TABLE IF NOT EXISTS SHOPPERS(" \
	                          "NAME TEXT NOT NULL," \
	                          "EMAIL TEXT NOT NULL," \
	                          "TOTAL_SPENT REAL);";
	            rc = sqlite3_exec(ppDb, createTable.c_str(), callback, (void*)data, &zErrMsg);
	            if( rc != SQLITE_OK ) {
	                fprintf(stderr, "SQL error: %s\n", zErrMsg);
	                sqlite3_free(zErrMsg);
	            } else {
	                fprintf(stdout, "Table created successfully\n");
	            }
	            sqlite3_close(ppDb);
	        }
	        catch (const exception& e) {
	            cout << "Error creating shoppers table, try again maybe?!" << endl;
	        }
	    }
	void loadShoppersTable() {
	    sqlite3 *ppDb;
	    sqlite3_stmt *stmt;
	    int rc;

	    try {
	        rc = sqlite3_open("Shoppers.db", &ppDb);
	        if (rc) {
	            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(ppDb));
	            sqlite3_close(ppDb);
	            exit(1);
	        }

	        string selectQuery = "SELECT * FROM SHOPPERS";
	        rc = sqlite3_prepare_v2(ppDb, selectQuery.c_str(), -1, &stmt, NULL);

	        if (rc == SQLITE_OK) {
	            while (sqlite3_step(stmt) == SQLITE_ROW) {
	                string name = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
	                string email = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
	                //double total_spent = sqlite3_column_double(stmt, 2);

	                // Do whatever you want with the data here, such as print it to the console or store it in a data structure.
	            }
	        } else {
	            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(ppDb));
	        }

	        sqlite3_finalize(stmt);
	        sqlite3_close(ppDb);
	    } catch (const exception& e) {
	        cout << "Error loading shoppers table, try again maybe?!" << endl;
	    }
	}



	void AddShopper() {
	    sqlite3* ppDb;
	    char* zErrMsg = 0;
	    int rc;
	    string name;
	    string email;
	    double total_spent;
	    cin.ignore(1, '\n');
	    cout << "Enter the shopper's name: " << endl;
	    getline(cin, name);
	    while (name.empty()) {
	        cout << "Must enter name. Enter the shopper's name: " << endl;
	        getline(cin, name);
	    }

	    cout << "Enter the shopper's email: " << endl;
	    getline(cin, email);
	    while (email.empty() || email.find("@") == string::npos) {
	        if (email.empty()) {
	            cout << "Must enter email. Enter the shopper's email: " << endl;
	        } else {
	            cout << "Invalid email. Please enter a valid email: " << endl;
	        }
	        getline(cin, email);
	    }

	    cout << "Enter the total spent: ";
	    while (!(cin >> total_spent) || total_spent < 0 || total_spent > 99999.99 || cin.peek() != '\n') {
	        cin.clear();
	        cin.ignore(numeric_limits<streamsize>::max(), '\n');
	        cout << "Invalid input. Enter the total spent (max 99999.99): ";
	    }

	    cout << "Total spent: " << fixed << setprecision(2) << total_spent << endl;

	    try {
	        rc = sqlite3_open("Shoppers.db", &ppDb);
	        if (rc) {
	            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(ppDb));
	            sqlite3_close(ppDb);
	            exit(1);
	        }

	        // Check if shopper already exists
	        string selectQuery = "SELECT COUNT(*) FROM SHOPPERS WHERE NAME = '" + name + "';";
	        sqlite3_stmt* stmt;
	        rc = sqlite3_prepare_v2(ppDb, selectQuery.c_str(), -1, &stmt, NULL);
	        if (rc != SQLITE_OK) {
	            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(ppDb));
	            sqlite3_finalize(stmt);
	            sqlite3_close(ppDb);
	            exit(1);
	        }

	        rc = sqlite3_step(stmt);
	        if (rc == SQLITE_ROW) {
	            int count = sqlite3_column_int(stmt, 0);
	            if (count > 0) {
	                cout << "Shopper already exists" << endl;
	                sqlite3_finalize(stmt);
	                sqlite3_close(ppDb);
	                return;
	            }
	        }

	        sqlite3_finalize(stmt);

	        // Add shopper to database
	        string insertQuery = "INSERT INTO SHOPPERS (NAME, EMAIL, TOTAL_SPENT) " \
	                             "VALUES ('" + name + "', '" + email + "', " + to_string(total_spent) + ");";
	        rc = sqlite3_exec(ppDb, insertQuery.c_str(), NULL, 0, &zErrMsg);
	        if (rc != SQLITE_OK) {
	            fprintf(stderr, "SQL error: %s\n", zErrMsg);
	            sqlite3_free(zErrMsg);
	        } else {
	            fprintf(stdout, "Shopper added successfully\n");
	        }

	        sqlite3_close(ppDb);
	    } catch (const exception& e) {
	        cout << "Error adding shopper, try again maybe?!" << endl;
	    }
	}

	bool IsShopperInDatabase() {
	    sqlite3* ppDb;
	    int rc = sqlite3_open("Shoppers.db", &ppDb);
	    if (rc != SQLITE_OK) {
	        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(ppDb));
	        sqlite3_close(ppDb);
	        exit(1);
	    }

	    string name;
	    cin.ignore(1,'\n');
	    cout << "Enter a name to search for: ";
	    getline(cin, name);

	    string selectQuery = "SELECT * FROM SHOPPERS WHERE NAME = '" + name + "';";
	    sqlite3_stmt* selectStmt;
	    rc = sqlite3_prepare_v2(ppDb, selectQuery.c_str(), -1, &selectStmt, nullptr);
	    if (rc != SQLITE_OK) {
	        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(ppDb));
	        sqlite3_close(ppDb);
	        return false;
	    }

	    rc = sqlite3_step(selectStmt);
	    if (rc != SQLITE_ROW) {
	        sqlite3_finalize(selectStmt);
	        sqlite3_close(ppDb);
	        return false;
	    }

	    sqlite3_finalize(selectStmt);
	    sqlite3_close(ppDb);
	    return true;
	}





	void CreateReceipt() {
	    double total = 0.0;
	    int numBooks = UserList.size();
	    cout << "*****************************" << endl;
	    cout << "      Sales Receipt          " << endl;
	    cout << "*****************************" << endl;
	    cout << "Purchased: " << numBooks << " books." << endl;
	    for (Book b : UserList) {

	        cout << "MSRP: $" << b.GetMSRP() << endl;
	        total += b.GetMSRP() * 1.06; // Add the price after tax to the total
	    }
	    double tax = total - (total / 1.06); // Calculate the tax amount
	    cout << "Tax: $" << tax << endl;
	    cout << "Total after tax: $" << total << endl;

	    string name;
	    cin.ignore(1, '\n');
	    cout << "Enter Shoppers Name: " << endl;
	    getline(cin, name);
	    sqlite3* db;
	        char* error;
	        int rc = sqlite3_open("Shoppers.db", &db);
	        if (rc) {
	            cout << "Error opening database: " << sqlite3_errmsg(db) << endl;
	            return;
	        }

	        string updateQuery = "UPDATE SHOPPERS SET TOTAL_SPENT = TOTAL_SPENT + " + to_string(total) + " WHERE Name = '" + name + "'";
	        rc = sqlite3_exec(db, updateQuery.c_str(), NULL, 0, &error);
	        if (rc) {
	            cout << "SQL error: " << error << endl;
	            sqlite3_free(error);
	        }

	        sqlite3_close(db);
	}






	void CreateUserListDatabase() {
	        sqlite3* ppDb;
	        char* zErrMsg = 0;
	        string createTable;
	        int rc;
	        const char* data = "Callback function called";
	        try {
	            sqlite3_open("UserList.db", &ppDb);
	            createTable = "CREATE IF NOT EXISTS TABLE USER("  \
	                "NAME TEXT     NOT NULL," \
	                "ISBN  TEXT     NOT NULL," \
					"TITLE TEXT 	NOT NULL," \
					"AUTHOR TEXT    NOT NULL);";
	            rc = sqlite3_exec(ppDb, createTable.c_str(), callback, (void*)data, &zErrMsg);
	            if (rc != SQLITE_OK) {
	                fprintf(stderr, "SQL error: %s\n", zErrMsg);
	                sqlite3_free(zErrMsg);
	            }
	            else {
	                fprintf(stdout, "Table created successfully\n");
	            }
	            sqlite3_close(ppDb);
	        }
	        catch (const exception& e) {
	            cout << "Error creating Database, try again maybe?!" << endl;
	        }

	    }

	void LoadUserListDatabase() {
		        sqlite3* ppDb;
		        char* zErrMsg = 0;
		        string insertRecord;
		        int rc;
		        const char* data = "Callback function called";
		        try {
		            sqlite3_open("UserList.db", &ppDb);
		            for (Book b : UserList) {
		                insertRecord = "INSERT INTO USER (USERNAME, PASSWORD) VALUES(\""
		                    + b.GetISBN() + "\",\""
		                    + b.GetTitle() + "\",\""
							+ b.GetAuthor();

		                rc = sqlite3_exec(ppDb, insertRecord.c_str(), callback, (void*)data, &zErrMsg);
		                if (rc != SQLITE_OK) {
		                    fprintf(stderr, "SQL error: %s\n", zErrMsg);
		                    sqlite3_free(zErrMsg);
		                }
		                else {
		                    //fprintf(stdout, "Table created successfully\n");
		                }
		            }
		            cout << "Database ready!" << endl;
		            sqlite3_close(ppDb);

		        }
		        catch (const exception& e) {
		            cout << "Error loading books, try again maybe?!" << endl;
		        }
		    }

	void CreateLibraryDatabase(){
		sqlite3 *ppDb;
		char *zErrMsg = 0;
		string createTable;
		int rc;
		const char* data = "Callback function called";
		try {
			sqlite3_open("Library.db", &ppDb);
			createTable = "CREATE TABLE IF NOT EXISTS LIBRARY("  \
			      "ISBN TEXT     NOT NULL," \
			      "TITLE	TEXT    NOT NULL," \
			      "AUTHOR	TEXT     NOT NULL," \
			      "PUBLISHER	TEXT," \
			      "PUB_DATE		INT," \
				  "DESCRIPTION 	TEXT," \
				  "GENRE        TEXT," \
				  "PRICE 		REAL," \
				  "MSRP         REAL,"\
				  "QUANTITY_ON_HAND		INT );";
			rc = sqlite3_exec(ppDb, createTable.c_str(), callback, (void*)data , &zErrMsg);
			if( rc != SQLITE_OK ) {
			      fprintf(stderr, "SQL error: %s\n", zErrMsg);
			      sqlite3_free(zErrMsg);
			   } else {
			      fprintf(stdout, "Database created successfully\n");
			   }
			sqlite3_close(ppDb);
		}
		catch (const exception& e) {
			cout << "Error creating Database, try again maybe?!" << endl;
		}


	}
	void LoadLibraryDatabase() {
	    sqlite3 *ppDb;
	    char *zErrMsg = 0;
	    string insertRecord;
	    int rc;
	    const char* data = "Callback function called";
	    try {
	        sqlite3_open("Library.db", &ppDb);
	        ifstream inputFile("books.csv");
	        if (inputFile.good()) {
	            string line;
	            while (getline(inputFile, line)) {
	                vector<string> bookInfo;
	                stringstream ss(line);
	                string token;
	                while (getline(ss, token, ',')) {
	                    bookInfo.push_back(token);
	                }
	                if (bookInfo.size() == 9) {
	                    insertRecord = "INSERT INTO LIBRARY (ISBN,TITLE,AUTHOR,PUB_DATE,PUBLISHER,PRICE) VALUES(\""
	                                + bookInfo[0] + "\",\""
	                                + bookInfo[1] + "\",\""
	                                + bookInfo[2] + "\",\""
	                                + bookInfo[3] + "\",\""
	                                + bookInfo[4] + "\",\""
	                                + bookInfo[5] + "\",\""
	                                + bookInfo[6] + "\");";
	                    rc = sqlite3_exec(ppDb, insertRecord.c_str(), callback, (void*)data , &zErrMsg);
	                    if (rc != SQLITE_OK) {
	                        fprintf(stderr, "SQL error: %s\n", zErrMsg);
	                        sqlite3_free(zErrMsg);
	                        break;
	                    }
	                }
	            }
	            inputFile.close();
	            cout << "Database ready!" << endl;
	        } else {
	            cout << "Error opening books.csv file" << endl;
	        }
	        sqlite3_close(ppDb);
	    } catch (int e) {
	        cout << "Error loading books, try again maybe?!" << endl;
	    }
	}

	void LoadDatabase() {
		sqlite3 *ppDb;
		char *zErrMsg = 0;
		string insertRecord;
		int rc;
		const char* data = "Callback function called";
		try {
			sqlite3_open("Library.db", &ppDb);
			for (Book b : BookList) {
				insertRecord = "INSERT INTO LIBRARY (ISBN,TITLE,AUTHOR,PUBLISHER,PUB_DATE,DESCRIPTION,GENRE,PRICE,QUANTITY_ON_HAND) VALUES(\""
							+ b.GetISBN() + "\",\""
							+ b.GetTitle() + "\",\""
							+ b.GetAuthor() + "\",\""
							+ b.GetPublicationYear() + "\",\""
							+ b.GetPublisher() + "\",\""
							+ b.GetDescription() + "\",\""
							+ b.GetGenre() + "\",\""
							+ to_string(b.GetMSRP()) + "\",\""
							+ to_string(b.GetQuantityOnHand()) + "\");";


				rc = sqlite3_exec(ppDb, insertRecord.c_str(), callback, (void*)data , &zErrMsg);
				if( rc != SQLITE_OK ) {
					  fprintf(stderr, "SQL error: %s\n", zErrMsg);
					  sqlite3_free(zErrMsg);
				   } else {
					  //fprintf(stdout, "Table created successfully\n");
				   }
			}
			cout << "Database ready!" << endl;
			sqlite3_close(ppDb);
		}
		catch (int e) {
			cout << "Error loading books, try again maybe?!" << endl;
		}
	}

	void ClearDatabase(){
		sqlite3 *ppDb;
		char *zErrMsg = 0;
		string createTable;
		int rc;
		const char* data = "Callback function called";
		try {
			sqlite3_open("Library.db", &ppDb);
			createTable = "DELETE FROM LIBRARY;";
			rc = sqlite3_exec(ppDb, createTable.c_str(), callback, (void*)data , &zErrMsg);
			if( rc != SQLITE_OK ) {
				  fprintf(stdout, "SQL error: %s\n", zErrMsg);
				  sqlite3_free(zErrMsg);
			   } else {
				  fprintf(stdout, "Database cleared successfully\n");
			   }
			sqlite3_close(ppDb);
		}
		catch (const exception& e) {
			cout << "Error clearing Database, try again maybe?!" << endl;
		}

	}
	void DisplayProfile(string name) {
	    sqlite3* db;
	    int rc = sqlite3_open("Shoppers.db", &db);

	    if (rc) {
	        cerr << "Error opening database: " << sqlite3_errmsg(db) << endl;
	        sqlite3_close(db);
	        exit(1);
	    }

	    string sql = "SELECT EMAIL, TOTAL_SPENT FROM SHOPPERS WHERE name = ?";
	    sqlite3_stmt* stmt;
	    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

	    if (rc) {
	        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
	        sqlite3_close(db);
	        exit(1);
	    }

	    rc = sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

	    if (rc) {
	        cerr << "Error binding parameter: " << sqlite3_errmsg(db) << endl;
	        sqlite3_finalize(stmt);
	        sqlite3_close(db);
	        exit(1);
	    }

	    rc = sqlite3_step(stmt);

	    if (rc == SQLITE_ROW) {
	        string email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
	        double total_spent = sqlite3_column_double(stmt, 1);

	        cout << "Name: " << name << endl;
	        cout << "Email: " << email << endl;
	        cout << "Total Spent: $" << total_spent << endl;
	    } else {
	        cerr << "Error retrieving profile for shopper " << name << endl;
	    }

	    sqlite3_finalize(stmt);
	    sqlite3_close(db);
	}


	void LoadFile(string filename, int numRecords){
		char c;
		string line = "";
		vector<string> BookDetails;
		int count = 0;
		cout << "Loading Books..." << endl;
		try {
			ifstream reader = ifstream();
			reader.open(filename);
			while (reader.get(c) && count < numRecords) {
				if (c == '\n') {
					BookDetails.push_back(line);
					Book b = Book(BookDetails[0], BookDetails[1], BookDetails[2], BookDetails[3], BookDetails[4], BookDetails[5]);
					BookList.push_back(b);
					count++;
					BookDetails.clear();
					line = "";
					//cout << b.GetTitle() << endl;

				}
				else {
					if (c == ',') {
						BookDetails.push_back(line);
						line = "";
					}
					else if (c == '"'){
						line += "'";
					}
					else {
						line += c;
					}
				}
			}
			reader.close();
			}
			catch (int e) {
				cout << "Error: " << e << endl;
			}
	}
	void PrintUserList() {
	    cout << "*** User Book List ***" << endl;

	    // Sort the list by MSRP in descending order
	    UserList.sort([](const Book& b1, const Book& b2) {
	        return b1.GetMSRP() > b2.GetMSRP();
	    });

	    for (Book b : UserList) {
	        cout << b.GetISBN() << endl;
	        cout << b.GetTitle() << endl;
	        cout << b.GetAuthor() << endl;
	        cout << fixed << setprecision(2) << b.GetMSRP() << endl << endl;
	    }

	    cout << "User List contains: " << UserList.size() << " books" << endl << endl;
	}
	void RemoveListFromInventory(){
		cout << "Do you want the following list to be removed: " << endl;
		for (Book b: UserList){
			cout << b.GetISBN() << endl;
			cout << b.GetTitle() << endl;
			cout << b.GetAuthor() << endl;
			cout << fixed << setprecision(2) << b.GetMSRP() << endl << endl;
		}
		cout << "User List contains: " << UserList.size() << " books" << endl << endl;
		 string userInput;
		    while (true) {
		        cout << "Enter 'yes' to confirm or 'no' to cancel: ";
		        cin >> userInput;
		        if (userInput == "yes") {
		        	if (userInput == "yes") {
		        	    // Remove books from inventory
		        	    ifstream file("books.csv");
		        	    ofstream temp("temp.csv");

		        	    string line;
		        	    while (getline(file, line)) {
		        	        stringstream ss(line);
		        	        string isbn;
		        	        getline(ss, isbn, ',');
		        	        if (find_if(UserList.begin(), UserList.end(), [&](Book b){return b.GetISBN() == isbn;}) == UserList.end()) {
		        	            temp << line << endl;
		        	        }
		        	    }

		        	    file.close();
		        	    temp.close();

		        	    remove("books.csv");
		        	    rename("temp.csv", "books.csv");

		        	    cout << "Books removed from inventory." << endl;
		        	    break;
		        	}


		        } else if (userInput == "no") {

		            return;
		        } else {
		            cout << "Invalid input, please enter 'yes' or 'no'." << endl;
		        }
		    }
		}

	void ExportToDatabase() {
	    sqlite3 *ppDb;
	    char *zErrMsg = 0;
	    int rc;

	    rc = sqlite3_open("UserList.db", &ppDb);
	    if (rc) {
	        cerr << "Error opening database: " << sqlite3_errmsg(ppDb) << endl;
	        sqlite3_close(ppDb);
	        return;
	    }

	    // create table if it doesn't exist
	    const char* createTable = "CREATE TABLE IF NOT EXISTS USERLIST("
	        "ISBN TEXT PRIMARY KEY NOT NULL,"
	        "TITLE TEXT NOT NULL,"
	        "AUTHOR TEXT NOT NULL,"
	        "MSRP REAL NOT NULL"
	        ");";
	    rc = sqlite3_exec(ppDb, createTable, NULL, 0, &zErrMsg);
	    if (rc != SQLITE_OK) {
	        cerr << "Error creating table: " << zErrMsg << endl;
	        sqlite3_free(zErrMsg);
	        sqlite3_close(ppDb);
	        return;
	    }

	    // insert user's books into the table
	    for (Book b : UserList) {
	        string insertRecord = "INSERT INTO USERLIST (ISBN,TITLE,AUTHOR,MSRP) VALUES('"
	            + b.GetISBN() + "','"
	            + b.GetTitle() + "','"
	            + b.GetAuthor() + "',"
	            + to_string(b.GetMSRP()) + ");";
	        rc = sqlite3_exec(ppDb, insertRecord.c_str(), NULL, 0, &zErrMsg);
	        if (rc != SQLITE_OK) {
	            cerr << "Error inserting record: " << zErrMsg << endl;
	            sqlite3_free(zErrMsg);
	            sqlite3_close(ppDb);
	            return;
	        }
	    }

	    sqlite3_close(ppDb);
	    cout << "Export to database successful!" << endl;
	}

	void ExportCSVFile() {
	    ofstream CSVfile;
	    try {
	        CSVfile.open("src/UserBookList.csv");
	    } catch(int e) {
	        cout << "Error: " << e << endl;
	    }

	    if (!CSVfile.is_open()) {
	        cout << "Error: Failed to open file" << endl;
	        return;
	    }

	    if (UserList.empty()) {
	        cout << "UserList is empty" << endl;
	        return;
	    }

	    CSVfile << "ISBN,title,author,MSRP\n";
	    for(Book b : UserList) {
	        CSVfile << b.GetISBN() << ",";
	        CSVfile << b.GetTitle() << ",";
	        CSVfile << b.GetAuthor() << ",";
	        CSVfile << b.GetMSRP() << "\n";
	    }
	    CSVfile.close();
	}

	void AddCardOnFile() {
		string shopper_name, card_number, expiration_date, cvv;

		    cout << "Enter shopper name: ";
		    cin.ignore(1,'\n');
		    getline(cin, shopper_name);

		    sqlite3* db;
		    sqlite3_open("Shoppers.db", &db);

		    // Check if the shopper exists
		    sqlite3_stmt* exists_stmt;
		    string exists_query = "SELECT * FROM SHOPPERS WHERE NAME = ?";
		    sqlite3_prepare_v2(db, exists_query.c_str(), exists_query.length() + 1, &exists_stmt, nullptr);
		    sqlite3_bind_text(exists_stmt, 1, shopper_name.c_str(), shopper_name.length(), SQLITE_STATIC);
		    if (sqlite3_step(exists_stmt) == SQLITE_ROW) {
		        // Shopper found, add credit card on file
		        cout << "Shopper found. Enter credit card information:\n";
		        cout << "Card number (16 digits): ";
		        getline(cin, card_number);
		        cout << "Expiration date (MM/YY): ";
		        getline(cin, expiration_date);
		        cout << "CVV (3 digits): ";
		        getline(cin, cvv);

		        // Insert credit card information into CREDITCARDS table
		        sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS CREDITCARDS (ID INTEGER PRIMARY KEY AUTOINCREMENT, NAME TEXT, CARD_NUMBER TEXT, EXPIRATION_DATE TEXT, CVV TEXT)", nullptr, nullptr, nullptr);
		        sqlite3_stmt* insert_stmt;
		        string insert_query = "INSERT INTO CREDITCARDS (NAME, CARD_NUMBER, EXPIRATION_DATE, CVV) VALUES (?, ?, ?, ?)";
		        sqlite3_prepare_v2(db, insert_query.c_str(), insert_query.length() + 1, &insert_stmt, nullptr);
		        sqlite3_bind_text(insert_stmt, 1, shopper_name.c_str(), shopper_name.length(), SQLITE_STATIC);
		        sqlite3_bind_text(insert_stmt, 2, card_number.c_str(), card_number.length(), SQLITE_STATIC);
		        sqlite3_bind_text(insert_stmt, 3, expiration_date.c_str(), expiration_date.length(), SQLITE_STATIC);
		        sqlite3_bind_text(insert_stmt, 4, cvv.c_str(), cvv.length(), SQLITE_STATIC);
		        sqlite3_step(insert_stmt);
		        sqlite3_finalize(insert_stmt);

		        cout << "Credit card information added successfully.\n";
		    } else {
		        // Shopper not found
		        cerr << "Shopper not found.\n";
		    }

		    sqlite3_finalize(exists_stmt);
		    sqlite3_close(db);
		}

	void LogFile(string message){
			fstream fs = fstream();
			try{
				fs.open("Log.csv", std::ios_base::app);
				fs << message << "\n";
				fs.close();
			}
			catch (const exception& e){
				cout << "Error logging to file" << endl;
			}
	}
	void SearchTitles(string title) {
		for (Book b : BookList) {
			string bookTitle = b.GetTitle();
			//if (b.GetTitle().find(title) != string::npos) {
			if(bookTitle.find(title) != string::npos){
				cout << b.GetISBN() << endl;
				cout << b.GetTitle() << endl;
				cout << b.GetAuthor() << endl;
				cout << b.GetPublisher() << endl;
				cout << b.GetPublicationYear() << endl;
				cout << fixed << setprecision(2) << b.GetMSRP() << endl;
				cout << b.GetQuantityOnHand() << endl << endl;

			}

		}
	}

	void ImportFile() {
	    string db_filename;
	    string file_path;

	    cout << "Enter database filename: ";
	    cin.ignore(1,'\n');
	    getline(cin, db_filename);

	    cout << "Enter file path to import: ";
	    getline(cin, file_path);

	    // Check if the input file exists
	    ifstream infile(file_path);
	    if (!infile.good()) {
	        cerr << "Error: File does not exist.\n";
	        return;
	    }

	    sqlite3* db;
	    int rc = sqlite3_open(db_filename.c_str(), &db);
	    if (rc != SQLITE_OK) {
	        cerr << "Error opening database: " << sqlite3_errmsg(db) << endl;
	        sqlite3_close(db);
	        return;
	    }

	    string line;
	    ifstream file(file_path);
	    while (getline(file, line)) {
	        rc = sqlite3_exec(db, line.c_str(), NULL, 0, NULL);
	        if (rc != SQLITE_OK) {
	            cerr << "Error importing file: " << sqlite3_errmsg(db) << endl;
	            sqlite3_close(db);
	            return;
	        }
	    }

	    cout << "File imported successfully.\n";
	    sqlite3_close(db);
	}


	bool IsISBNinDatabase(string isbn) {
		for (Book b : BookList) {
			if (isbn.compare(b.GetISBN()) == 0)
				return true;
		}
		return false;
	}
	void AddBookToListByISBN(string isbn) {
		for (Book b : BookList) {
			if (isbn.compare(b.GetISBN()) == 0) {
				UserList.push_back(b);
				//BookList.remove(b);
				break;
			}
		}

	}
	void AddBook(string ISBN, string title, string author, string publisher, string pubYear, string MSRP) {
		Book newBook = Book(ISBN, title, author, publisher, pubYear, MSRP);
		BookList.push_back(newBook);

	}
	void AddBook(string ISBN, string title, string author, string publisher, string pubYear, string description, string genre, string MSRP) {
		Book newBook = Book(ISBN, title, author, publisher, pubYear, description, genre, MSRP);
				BookList.push_back(newBook);
	}
	void AddToList(Book b) {
		UserList.push_back(b);
	}
	int GetDatabaseCount(){
		return BookList.size();
	}


};






#endif /* DATABASE_H_ */
