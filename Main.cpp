//Kyle Heitman
//infix to postfix and postfix to infix

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

struct node { //Stack node
	char ch;
	node *next;
};

void init(node *top) { //Initialize the first node
	top -> next = NULL;
}

bool isEmpty(node *top) { //Check to see if the stack is empty
	if (top->next==NULL) {
		return true;
	} else {
		return false; 
	}
}

void push (node *&top, char c) { //Push the character onto the stack
	node *temp = new node;
	temp->ch = c; 
	temp->next = top;
	top = temp;
}

char pop (node *&top) { //Pop the top node from the stack
	char retval;
	if(isEmpty(top)) {
		cout << "empty" << endl;
		return 0;
	} else {
		node *temp =  top;
		retval = top->ch;
		top = top->next;
		delete temp;
		return retval;
	}
}

char viewTop (node *top) { //View the top of the stack
	char retval;
	if(isEmpty(top)) {
		return 0;
	} else {
		retval = top->ch;
		return retval;
	}
}
 
int priority(char c) { //Check the priority of the character
	int temp = 0;

	if (c == '^') //highest priority
        temp = 1;
	else  if (c == '*' || c == '/')
        temp = 2;
    else  if (c == '+' || c == '-')
        temp = 3;
	else if (c == '(')
		temp = 4;
    return temp;
}

void inToPostFix(node *&top, string infix) //Convert infix to postfix
{
	stringstream ot;

	for (int i = 0; i < infix.length(); i++) {
		if (infix[i] == '-' || infix[i] == '+' || infix[i] == '*' || infix[i] == '/' || infix[i] == '^') {
			if(isEmpty(top) || priority(infix[i]) < priority(viewTop(top))) { //If the top is empty and the priority of the operator is lower
				push(top,infix[i]);                                           //than that of the top element push it onto the top of the stack
			} else {                                                          
				while(priority(infix[i]) >= priority(viewTop(top)) && !isEmpty(top)) { //Else while the top is not empty and it is still greater
					ot << viewTop(top);                                                //than or equal to the top, pop the top element
					pop(top);
				}
				push(top,infix[i]);
			}
		} else if (infix[i] == ')') { //If a right parentheses is encountered keep popping the stack until a left one is encountered
			while(viewTop(top) != '(' && !isEmpty(top)) { //and then pop the left parentheses
				ot << viewTop(top);
				pop(top);
			}
			pop(top);
		} else if (infix[i] == '(') { //Just push the left parentheses onto the stack and wait for a right parenthesis 
			push(top, infix[i]);
		} else if (infix[i] == ' ') {
			ot << " ";
		} else {
			ot << infix[i]; //send the operand to the string stream
		}
	}
	while (!isEmpty(top)) { //Keep popping the stack and storing it in the stringstream
		ot << viewTop(top);
		pop(top);
	}
		cout << ot.str() << endl;
}

void postToInfix(node *&top, string infix) //post fix to infix
{
	stringstream ot;
	int counter = 0; //used to store the size of the first operand
	//int i=0; i < ot.str().size(); i++ ---iterate through stringstream forward
	//int i=ot.str().size() - 1; i >= 0; i-- --- iterate backwards

	for(int i=0; i < infix.length(); i++) { //Loop through the length of the expression
		if (infix[i] == '-' || infix[i] == '+' || infix[i] == '*' || infix[i] == '/' || infix[i] == '^') {
			ot << ')'; // Encapsulate the two operands with the operator in the middle and push it back onto the stack
			if(viewTop(top) == ')') { //If a right parentheses is encountered pop the first operand off (which is the size of counter)
				for(int l=0; l < counter; l++) {
					ot << pop(top);
				}
			} else {
				ot << pop(top); //If no parentheses just pop the top operand off
			}
			ot << infix[i]; //Put the operator in the middle of the two operands
			if(viewTop(top) == ')') { //Get the second operand
				while(viewTop(top) != '(') {
					ot << pop(top);
				}
				ot << pop(top);
			} else {
				ot << pop(top);
			}
			ot << '(';
			for(int i=ot.str().size() - 1; i > -1; i--) { //Push the whole stringstream onto the stack backwards
				push(top, ot.str().at(i));
			}
		} else { 
			push(top, infix[i]); //Push the operand onto the stack
		}
		counter = ot.str().length(); //store the size of the first operand that is on the stack
		ot.clear(); //Clear the stringstream for use in the next iteration
		ot.str(string());
	}
	ot.clear(); //Clear the stringstream
	ot.str(string());
	while (!isEmpty(top)) { //Pop the whole stack into the stringstream
		ot << pop(top);
	}
	for (int i=ot.str().size() - 1; i > -1; i--) { //Cout the stringstream 
		cout << ot.str().at(i);
	}
	cout << endl;
}
int main()
{
	struct node *top = new node; //Create the top node
	string infix;                //string to store our input data
	char choice;               
	bool flag = true;

	init(top); //initialize the top element

	cout << "|------------------------------------------------------------------|" << endl;
	cout << "|Infix to postfix: Type 1 and hit enter, then input your expression|" << endl;
	cout << "|Postfix to infix: Type 2 and hit enter, then input your expression|" << endl;
	cout << "|Do not use spaces, ex: a*b/c, not a * b / c. As sine exoressions  |" << endl;
	cout << "|may not turn out correct                                          |" << endl;
	cout << "|Type 3 and hit enter to exit                                      |" << endl;
	cout << "|------------------------------------------------------------------|" << endl;

	while (flag) {
		cout << "Choice: ";
		cin >> choice;
		switch(choice) {
		case'1': cout << "Enter your expression: ";
				 cin.ignore(256,'\n'); //Clears the buffer because sometimes the enter key registers as the value and messes things up
				 getline(cin, infix);
				 inToPostFix(top, infix);
				 break;
		case'2': cout << "Enter your expression: ";
				 cin.ignore(256,'\n');
				 getline(cin, infix);
				 postToInfix(top, infix);
				 break;
		case'3': cout << "Goodbye" << endl;
				 exit(EXIT_SUCCESS);
		case'4': inToPostFix(top,"a*b/c");
			     break;
		case'5': inToPostFix(top,"a+b*c+(d*e+f)*g");
			     break;
		case'6': postToInfix(top,"52+83-*4/");
			     break;
		case'7': postToInfix(top, "ab*c/");
				 break;

		}
		
	}

	return 0;
}