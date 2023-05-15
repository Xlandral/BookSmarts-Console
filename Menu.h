/*
 * Menu.h
 *
 *  Created on: Feb 16, 2023
 *      Author: David McMahon
 *      	Cheyenne Westbrook
 *      	Ruben Rios
 *      	Connor Schaffer
 */

#include <string>


#ifndef MENU_H_
#define MENU_H_

//static void Menu() {

//}
//Print Functions
void PrintMainMenu(){
	cout << "Please enter a selection from the menu:" << endl;
	cout << " 1 - Search Database" << endl;
	cout << " 2 - Add Book to Database" << endl;
	cout << " 3 - Create Shopping list" << endl;
	cout << " 4 - Print Shopping list" << endl;
	cout << " 5 - Delete List From Inventory" << endl;
	cout << " 6 - Export Shopping List" << endl;
	cout << " 7 - Add Shopper" << endl;
	cout << " 8 - Sales Receipt" << endl;
	cout << " 9 - Admin Menu" << endl;
	cout << " 0 - QUIT" << endl;
}

void PrintAdminMenu () {
	cout << "Enter a selection from the Menu" << endl;
	cout << " 1 - Add User" << endl;
	cout << " 2 - Change Password" << endl;
	cout << " 3 - Import File" << endl;
	cout << " 4 - Create Database" << endl;
	cout << " 5 - Object Stats" << endl;
	cout << " 6 - Delete all from Database" << endl;
	cout << " 7 - View Profile" << endl;
	cout << " 8 - Add Card On File" << endl;
	cout << " 0 - Exit Admin Menu" << endl;

}

int GetMenuOption() {
	int integer;
	while(!(cin >> integer)){
		cout << "Enter an option from the menu please" << endl;
		cin.clear();
		cin.ignore(123, '\n');
	}
	return integer;
}



#endif /* MENU_H_ */

