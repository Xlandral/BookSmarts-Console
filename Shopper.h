/*
 * Shopper.h
 *
 *  Created on: Feb 25, 2023
 *      Author: david
 */


#ifndef SHOPPER_H_
#define SHOPPER_H_
#include <regex>

using namespace std;

class Shopper {

private:
	string _name;
	string _email;
	double _totalSpent;

public:
	Shopper(string name, string email, double total) {
	        _name = name;

	        // Validate email format
	        if (regex_match(email, regex("[\\w._%+-]+@[\\w.-]+\\.[A-Za-z]{2,}"))) {
	            _email = email;
	        } else {
	            cout << "Invalid email format. Please enter a valid email." << endl;
	            // Set default email value to empty string
	            _email = "";
	        }

	        // Set total spent to two decimal places
	        _totalSpent = round(total * 100.0) / 100.0;
	    }
	string GetName() {
		return _name;
	}
	string GetEmail() {
		return _email;
	}
	double GetTotal() {
		return _totalSpent;
	}
};



#endif /* SHOPPER_H_ */
