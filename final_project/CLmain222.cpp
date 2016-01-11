#include <iostream>
#include <sstream>
#include <string.h>
#include <string>
#include <my_sys.h>
#include <ctype.h>
#include <my_global.h>
#include <mysql.h>
#include <cstdio>

#define SERVER "54.183.221.235" // Public IP
#define USER "root"
#define PASSWORD "RayJose"
#define DATABASE "project"
#define PORT "3306"

#include <gmp.h>
extern "C"{
#include "paillier.h"
}
using namespace std;
//using namespace sql;

int main() {
	MYSQL *connection; // Creates a pointer to the MySQL instance
	MYSQL_RES *result;	// Stores resulted output.
	result = NULL; // Set result as default NULL
	
	const char *hex_key;
	hex_key = "8dfd1c1beb4a6d9357f8579e39ad9aa5";
	paillier_pubkey_t* pubkey = paillier_pubkey_from_hex((char *)hex_key);
	paillier_ciphertext_t* my_enc_text;
	
	int num_fields;
	int num_rows;

	connection = mysql_init(NULL); // Initialises a MYSQL object suitable for mysql_real_connect() function

	// Checks for fault testing
	if(!connection) { // If instance fails then it exits with a fault
		fprintf(stderr, "MySQL Initialization Failed.\n");
	    mysql_close(connection);
	    return 1;
	}

	// Connection to specific database
	connection = mysql_real_connect(connection, "54.183.221.235", "ubuntu", "RayJose", "project", 3306, NULL, 0);

	// Prints if connection to mysql server was established or not
	if(connection) {
		printf("Connection Succeeded\n");
	}
	else {
		printf("Connection Failed!\n");
		return 1;
	}

	// Parser and Send Queries to mySQL project database
	while(true) {
		string command, c1, c2, c3, c4, c5, c6, c7, c8;
		int case_num;
		int length;

		char line[10000];
		cin.get(line, 1000); // Retrieve input command line

		string str(line); // Change the type of the command line from characters into a string
		length = str.length(); //Length of the string line

		
		command = strtok(&str[0]," "); // Splits line str into tokens in order to access the command
		c1 = strtok(NULL," "); //Access the second part of the command
		
		// Here it will check which command to work with in the swith case statments
		if(command == "INSERT") {case_num = 1;}
		else if((command == "SELECT") && (c1 != "SUM")) {case_num = 2;}
		else if((command == "SELECT") && (c1 == "*")) {case_num = 3;}
		else if((command == "SELECT") && (c1 == "SUM")) {case_num = 4;}
		else if((command == "SELECT") && (c1 == "AVG")) {case_num = 5;}
		else if(command == "exit") {case_num=6;}
		
		char *ptr1, *ptr2, *ptr3; // Pointers in order to access the token value
		int value1, value2, value3;
		char *str_enc_text;
		string emp_id, emp_age;
		ostringstream EMP_ID, EMP_AGE;
		paillier_plaintext_t* my_enc_value;
		
		switch(case_num) {
		case 1: //INSERT
		  {
		    ptr2 = strtok(NULL," ");
		    value2 = atoi(ptr2); // Parses the string ptr2 interpreting its content as a value of type int
		    ptr3 = strtok(NULL," ");
		    value3 = atoi(ptr3);
		    ptr1 = strtok(&c1[0], " ");
		    value1 = atoi(ptr1);
		    
		    EMP_ID << value1;
		    emp_id = EMP_ID.str();
		    EMP_AGE << value2;
		    emp_age = EMP_AGE.str();
		    //emp_id = std::to_string(value1);
		    //emp_age = std::to_string(value2);
		    
		    //CODE...not sure if have to change value1 into a string to add into 
		    //the query statement or just add the int value in after the quotes or between.
		    //If the id already exists in the table, the insert should fail with a message.
		    //The salary needs to be encrypted before 
		    //sending the insert command to the MySQL server which is value3
		    
		    my_enc_value = paillier_plaintext_from_str(ptr3);
		    my_enc_text = paillier_enc(NULL, pubkey, my_enc_value, paillier_get_rand_devurandom);
		    str_enc_text = mpz_get_str(NULL, 10, my_enc_text->c);
		    
		    /*		    string str_q; 
		    str_q  = "INSERT INTO Employees VALUES(" + emp_id + ", " + emp_age + ", "  + ")";
		    const char *cstr_q = str_q.c_str();
		    
		    mysql_query(connection, cstr_q);
		    result = mysql_store_result(connection);
		    if(result == NULL){
		      fprintf(stderr, "ID Already Exists in Table.\n");	
		      }*/
		    break;
		  }
		case 2: //SELECT
		  {
		    ptr1 = strtok(&c1[0], " ");
		    value1 = atoi(ptr1);
		    
		    //CODE...
		    //If the employee does not exist show a failure message.
		    /*
		    string str_qq;
		    str_qq = "SELECT id, age, salary FROM Employees WHERE id = "; // + value1.c_str());
		    const char *cstr_qq = str_qq.c_str();
		    
		    mysql_query(connection, cstr_qq);
		    result = mysql_store_result(connection);
		    
		    if(result == NULL){
		      fprintf(stderr, "Employee Doesn't Exist in Table.\n");	
		      }*/
		    break;
		  }
		case 3: //SELECT *
		  {
		    //CODE...
		    //If there are no employees in the table display an appropriate message stating this fact.
		    
		    mysql_query(connection,"SELECT id, age, salary FROM Employees");
		    result = mysql_store_result(connection);
		    
		    if(result == NULL){
		      fprintf(stderr, "No Employees Exist in Table.\n");	
		    }
		    break;
		  }
		case 4: //SELECT SUM
		  {		  
		    //CODE...
		    //If there are no employees, the displayed sum should be NULL.
		    //mysql_query(connection,"SELECT * FROM Employees");
		    //result = mysql_store_result(connection);					
		    break;
		  }
		case 5: //SELECT AVG
		  {
		    //CODE...
		    //If there are no employees the displayed avg should be NULL.
		    //result = mysql_store_result(connection);
		  }		
		case 6: //exit 
		  {
		    mysql_close(connection); // Close connection and shutdown
		    return 0;
		  }
		default:
		  {
		    cout << "Error...Bad Input...Quitting" << endl;
		    break;
		  }
		  
		  
		  mysql_free_result(result); // Frees the momory allocated for result
		  result = NULL; // Set result back to default NULL
		  
		}
		
	}
	
	mysql_free_result(result); // Free the value of result
	mysql_close(connection); // Close connection and shutdown
	return 0;
	
}
