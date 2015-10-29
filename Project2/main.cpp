#include <iostream>
#include <string>
#include <my_global.h>
#include <mysql.h>
#include <termios.h>
#include <curses.h>

using namespace std;

string myget_passwd();
char mygetch();

int buildMyTables(MYSQL &, MYSQL *);
void addCar(MYSQL &, MYSQL *, MYSQL_RES *, MYSQL_RES *);
void addManufacturer(MYSQL &, MYSQL *);
void addDealer(MYSQL &, MYSQL *);


int main()
{
	// mysql connection and query variables
	MYSQL *conn, // actual mysql connection
		mysql;   // local mysql data
	MYSQL_RES *res; // mysql query results
	MYSQL_RES *res2;
	MYSQL_ROW row;

		//Strings for mysql hostname, userid, ...
	string db_host;
	string db_user;
	string db_password;
	string db_name;

		//set up the client (this machine) to use mysql
	cout << "initializing client DB subsystem ..."; 
	cout.flush();
	mysql_init(&mysql);
	cout << "Done!" << endl;
	
	db_host = "0.0.0.0";
	db_user = "alexknipfer";
	db_password = "";
	db_name = "alexknipfer";

		//get user credentials and mysql server info
	/*cout << "Enter MySQL database hostname (or IP adress):";
	cin >> db_host;

	cout << "Enter MySQL database username (also the database name):";
	cin >> db_user;

	cout << "Enter MySQL database password:";
	db_password=myget_passwd();*/

		//could also prompt for this, if desired
	db_name = db_user;

		//go out and connect to the mysql server
	cout << "Connecting to remote DB ..."; 
	cout.flush();
	conn = mysql_real_connect(&mysql, 
		                  db_host.c_str(), db_user.c_str(), 
				  db_password.c_str(), db_name.c_str(),
				  0,0,0); // last three are usually 0's

		//if we couldn't setup the connection ...
	if (conn==NULL)
	{
			//print out the error message as to why ...
		cout << mysql_error(&mysql) << endl;
		return 1; // ... and exit the program. 
	}
	
	else
	{
		cout << "DB connection established" << endl;
	}
	
	char tag;			//reads in "a", "l", or "f"
	char typeToAdd;		//determines what to add, list, or find
	
		//build 3 tables (cars, dealers, manufacturers)
	buildMyTables(mysql, conn);
	
	cout << ">>>";		//indicates for user to input
	cin >> tag;			//read in add, list, or find (a, l, or f)
	
		//continue to ask for input unless the user types a "q" then terminate
	while(tag != 'q')
	{
			//reads in what to add (car, dealer, or manufacturer)
		cin >> typeToAdd;
		
			//add something if tag is 'a'
		if(tag == 'a')
		{
				//determine what to add based on what user entered
			switch(typeToAdd)
			{
					//add car if type entered was 'c'
				case 'c':
					addCar(mysql, conn, res, res2);
					break;
					
					//add manufacturer if type entered was 'm'
				case 'm':
					addManufacturer(mysql, conn);
					break;
					
					//add dealer if type entered was 'd'
				case 'd':
					addDealer(mysql, conn);
					break;
			}
		}
		
			//list something if tag is 'l'
		if(tag == 'l')
		{
				//determine what to list based on what user entered
			switch(typeToAdd)
			{
					//list cars if type entered was 'c'
				case 'c':
					//listCars();
					cout << "list cars" << endl;
					break;
					
					//list dealers if type entered was 'd'
				case 'd':
					//listDealers();
					cout << "list dealers" << endl;
					break;
			}
		}
		
			//find something if tag is 'f'
		if(tag == 'f')
		{
				//determine what to find based on what user entered
			switch(typeToAdd)
			{
					//find manufacturer if type entered was 'm'
				case 'm':
					//findManufacturer();
					cout << "find manufacturers" << endl;
					break;
			}
		}
		
			//prompt for user input again
		cout << ">>>";
		cin >> tag;
	}
	
	
	
	
	
		//clean up the connection
	mysql_close(conn);

		//SUCCESS!!!
	return 0;
}

//******************************************************************************

char mygetch()
{
  struct termios oldt, newt;
  int ch;

	//Get the current attributes of the terminal, and back them up
  tcgetattr( STDIN_FILENO, &oldt );
  
	//Make a new copy of the current terminal
  newt = oldt;

	//tell the new terminal not to echo, 
	//and to let me process spec. chars
  newt.c_lflag &= ~( ICANON | ECHO );
  tcsetattr( STDIN_FILENO, TCSANOW, &newt );

	//Call getting a character, curses style
  ch = getchar();  

	//Restore the old (echoing, usual spec. chars) terminal
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt );

  return ch;
}

//******************************************************************************

string myget_passwd()
{
	//reads in a password, echoing it to the screen as *'s
	
	string passwd;
  
		//Dump any existing output text (prompt?)
	cout.flush();
  
		//If the last character pressed was a newline, 
		//we need to extract it ...
	char check=cin.get();
  
		//... if it wasn't we shoudld put it back.
	if (check!=10 && check!=13)  
		cin.unget();
  
	for(;;)
	{
		char ch;
		ch=mygetch(); // get single character
      
		if (ch==127)  // if the user typed a backspace
		{
		
			if (passwd.length()==0) //attempt to go beyond beginning of pwd.
				continue;
	  
			cout << "\b \b"; // back over last typed char, replace with space
			passwd.erase(passwd.size()-1,1); // remove char from passwd
			continue; // go get next character
		}

		if (ch==10 || ch==13) // if done (return pressed) ...
			break;           //  stop reading chars!
      
		//if you get here, the character should be part of the password.
		cout << "*"; cout.flush();// dump * to screen
		passwd+=ch;   // addd char to password
	}
  
	cin.sync(); // flush anything else in the buffer (remaining newline)
  
	cout << endl;  // simulate the enter that the user pressed
  
	return passwd;
}

//******************************************************************************

int buildMyTables(MYSQL &mysql, MYSQL *conn)
{
	string carTable;	//car table
	string manuTable;	//manufacuter table
	string dealerTable;	//dealer table
	
	int carQuery;		//store query for creating car table
	int manuQuery;		//store manufacturer query for creating manufacturer table
	int dealerQuery;	//store dealer query for creating dealer table

		//create "query" for creating a car table
	carTable  = "create table if not exists carTable (vin char(100), miles integer, dealer char(50), cost integer, primary key(vin))";
	cout.flush();
	
		//store query into carQuery
	carQuery = mysql_query(conn, carTable.c_str());
	
	// if the query didn't work ...
	if (carQuery!=0)
	{
		// ... explain why ...
		cout << mysql_error(&mysql) << endl;
		return 1;  // ... and exit program
	}
	
		//create "query" for creating manufacturer table	
	manuTable  = "create table if not exists manuTable (manuAbb char(3), manu char(50), primary key(manuAbb))";
	cout.flush();
	
		//store query into manuQuery
	manuQuery = mysql_query(conn, manuTable.c_str());
	
		// if the query didn't work ...
	if (manuQuery!=0)
	{
		// ... explain why ...
		cout << mysql_error(&mysql) << endl;
		return 1;  // ... and exit program
	}
	
		//create "query" for creating dealer table
	dealerTable  = "create table if not exists dealerTable (dealerName char(50), zipCode integer, phoneNumber char(10), primary key(dealerName))";
	cout.flush();
	
		//store query into dealerQuery
	dealerQuery = mysql_query(conn, dealerTable.c_str());
	
		// if the query didn't work ...
	if (dealerQuery!=0)
	{
		// ... explain why ...
		cout << mysql_error(&mysql) << endl;
		return 1;  // ... and exit program
	}	
	
	return 0;
}

//******************************************************************************

void addCar(MYSQL &mysql, MYSQL *conn, MYSQL_RES *res, MYSQL_RES *res2)
{
		//This function adds a car to the car table given
		//a VIN, mileage, dealer, and cost from user
		
		
	string carVIN;			//car VIN number
	string carMiles;			//car mileage
	string carDealer;		//car dealer
	string carCost;			//car cost
	string manuAbbreviation; //manufacturer from VIN
	
	string addCarInsert;
	int addCarQuery;
	
	MYSQL_ROW row;
	
		//read in VIN, miles, dealer, and cost from user
	cin >> carVIN;			
	cin >> carMiles;
	cin >> carDealer;
	cin >> carCost;
	
	
	manuAbbreviation = carVIN.substr(0,3);
	
	//***** The following selects dealers from dealer table *******************/
	string dealersQuery = "select dealerName from dealerTable;";
	
	int dealerQuery = mysql_query(conn, dealersQuery.c_str());
			//get the query result(s)
	res = mysql_store_result(conn);
	int totalRowsDealer = mysql_num_rows(res);
	
		//if the query didn't work ...
	if (dealerQuery !=0)
	{
			// ... explain why ...
		cout << mysql_error(&mysql) << endl;
		
		return;  // ... and exit program
	}
	
	//************* The following selects manu abbreviations from manuTable****/
	string manuQuery = "select manuAbb from manuTable;";
	int manufacturerQuery = mysql_query(conn,manuQuery.c_str());
	res2 = mysql_store_result(conn);
	int totalRowsManu = mysql_num_rows(res2);
	
	if (manufacturerQuery !=0)
	{
			// ... explain why ...
		cout << mysql_error(&mysql) << endl;
		
		return;  // ... and exit program
	}
	//*************************************************************************/

	if (totalRowsDealer == 0 && totalRowsManu == 0)
	{
		cout << "Please add a dealer and manufacturer before adding a car" << endl;	
	}
	
	else if(totalRowsDealer == 0)
	{
		cout << "Please add a dealer first, dealer doesn't exist" << endl;
	}
	
	else if(totalRowsManu == 0)
	{
		cout << "Please add manufacturer first, doesn't exist" << endl;
	}
	
	else
	{
			//go through each line (row) of the answer table
		for(row=mysql_fetch_row(res);
		row!=NULL;
		row=mysql_fetch_row(res))
		{
			if(row[0] == carDealer)
			{
				addCarInsert = "insert into carTable values(\"";
				addCarInsert += carVIN + "\"," + "\"" + carMiles + "\"," + "\"" + carDealer + "\"," + "\"" + carCost + "\")";
		
				addCarQuery = mysql_query(conn, addCarInsert.c_str());
	
					// if the query didn't work ...
				if (addCarQuery != 0)
				{
						// ... explain why ...
					cout << mysql_error(&mysql) << endl;
					return;  // ... and exit program
				}		
			}
		
			else
			{
				cout << "Please add dealer first" << endl;
			}
		}
		
					//go through each line (row) of the answer table
		/*for(row=mysql_fetch_row(res2);
		row!=NULL;
		row=mysql_fetch_row(res2))
		{
			if(row[0] == manuAbbreviation)
			{
				addCarInsert = "insert into carTable values(\"";
				addCarInsert += carVIN + "\"," + "\"" + carMiles + "\"," + "\"" + carDealer + "\"," + "\"" + carCost + "\")";
		
				addCarQuery = mysql_query(conn, addCarInsert.c_str());
	
					// if the query didn't work ...
				if (addCarQuery != 0)
				{
						// ... explain why ...
					cout << mysql_error(&mysql) << endl;
					return;  // ... and exit program
				}		
			}
		
			else
			{
				cout << "Please add manufacturer first" << endl;
			}
		}*/
	}
}

void addManufacturer(MYSQL &mysql, MYSQL *conn)
{
	string abbreviation;
	string manuName;
	
	string addManuInsert;
	int addManuQuery;
	
	cin >> abbreviation;
	cin >> manuName;
	
	addManuInsert = "insert into manuTable values(\"";
	addManuInsert += abbreviation + "\"," + "\"" + manuName + "\")";
	
	addManuQuery = mysql_query(conn, addManuInsert.c_str());
	
		// if the query didn't work ...
	if (addManuQuery != 0)
	{
		// ... explain why ...
		cout << mysql_error(&mysql) << endl;
		return;  // ... and exit program
	}
}

void addDealer(MYSQL &mysql, MYSQL *conn)
{
	string dealerName;
	string dealerZip;
	string dealerPhone;
	
	string addDealerInsert;
	int addDealerQuery;
	
	cin >> dealerName;
	cin >> dealerZip;
	cin >> dealerPhone;
	
	addDealerInsert = "insert into dealerTable values(\"";
	addDealerInsert += dealerName + "\"," + "\"" + dealerZip + "\"," + "\"" + dealerPhone + "\")";
	
	addDealerQuery = mysql_query(conn, addDealerInsert.c_str());
	
		// if the query didn't work ...
	if (addDealerQuery != 0)
	{
		// ... explain why ...
		cout << mysql_error(&mysql) << endl;
		return;  // ... and exit program
	}
	
	
}

