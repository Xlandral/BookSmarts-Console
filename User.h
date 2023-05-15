/*
 * Users.h
 *
 *  Created on: Jan 26, 2023
  *      Author: Ruben Rios
 *      		Cheyenne Westbrook
 *      		David McMahon
 *      		Connor Schaffer
 *
 */

#ifndef USER_H_
#define USER_H_
#include "md5.h"

class User{

private:
	//bool _isValidated;
	string _userName;
	string _password;
	string _hashedPassword;

public:
	User(string userName, string password){
		//_isValidated = false;
		_userName = userName;
		_password = password;
		_hashedPassword = md5(password);
		//cout << "Hashed Password for user" << _userName << ":" << _hashedPassword << endl;
	}
	string getName() {
		return _userName;
	}
	string getPassword() {
		return _password;
	}
	string getHashedPassword() {
		return _hashedPassword;
	}
	void setPassword(string password) {
		_password = password;
	}
};




#endif /* USER_H_ */

