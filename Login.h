/*
 * Login.h
 *
 *  Created on: Jan 26, 2023
 *      Author: Ruben Rios
 *      		Cheyenne Westbrook
 *      		David McMahon
 *      		Connor Schaffer
 *
 */


using namespace std;

#ifndef LOGIN_H_
#define LOGIN_H_

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "User.h"
#include "Book.h"
#include "md5.h"
#include "Database.h"




class Login
{
private:
	bool _IsValidated;

public:
	vector<User> Users;

	Login(){
		_IsValidated = false;

	}
	~Login(){

	}

	void LoadUsers(string filename){

		char c;
		string line = "";
		vector<string> userDetails;

		cout << "Loading Users..." << endl;
		try {
			ifstream reader = ifstream();
			reader.open(filename);
			while (reader.get(c)) {
				if (c == '\n') {
					userDetails.push_back(line);
					User u = User(userDetails[0], userDetails[1]);
					Users.push_back(u);
					//cout << u.getName() << u.getPassword() << endl;
					userDetails.clear();
					line = "";
				}
				else {
					if (c == ',') {
						userDetails.push_back(line);
						line = "";
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

		cout << "Users are Loaded!" << endl;
		//DB.LogFile("Successful database operation");
	}
	void Validate() {
		string userName;
		string password;
		cout << "Enter User Name" << endl;
		cin >> userName;
		cout << "Enter Password" << endl;
		cin >> password;

		string hashedPassword = md5(password);

		for (User u : Users) {
			//cout << u.getName() << "," << u.getPassword() << endl;
			if ((userName.compare(u.getName()) == 0 && hashedPassword.compare(u.getHashedPassword()) == 0)) { // Fix
				_IsValidated = true;


			}

		}
	}
	bool IsValidated() {
		return _IsValidated;
	}

	void AddUser(string filename, string name, string password) {
		fstream fs = fstream();
		try {
			fs.open(filename, std::ios_base::app);
			fs << name << "," << password << "\n";
			fs.close();
		}
		catch (int e) {
			cout << "Error adding user" << endl;
		}
	}

	void ChangePassword(string filename, string name, string password) {
		fstream fs = fstream();
		try {
			fs.open(filename);

			for (User u : Users) {
				if (u.getName().compare(name) == 0) {
					u.setPassword(password);
					cout << u.getName() << "'s Password Changed to : " << u.getPassword() << endl;
				}
				fs << u.getName() << "," << u.getPassword() << "\n";
			}
			fs.close();
		}
		catch (int e) {
			cout << "Error: Password not changed" << endl;
		}

	}

};




#endif /* LOGIN_H_ */
