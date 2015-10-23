//*********************************************************************
//*                                                                   *
//*     FILE NAME: airlineDatabase.cpp            Project #2          *
//*                                                                   *
//*     AUTHOR: __________________________                            *
//*                 Tyler Knipfer                                     *
//*                                                                   *
//*     COURSE #: CSC 30500          DUE DATE: November 22nd, 2013    *
//*                                                                   *
//*********************************************************************

//**********************Program Description****************************
//* The purpose of this program is to simulate an airline, city, and  *
//* and flight tracking system. You can add cities, airlines, and     *
//* flights to the database. After all information is added to the    *
//* database you can then search for a certain flight that you want.  *
//* This program is using MySQL for the database part. You need to    *
//* have a mysql server to connect to along with the username and     *
//* password. You also need to create a database before running the   *
//* program equal to the username of you mysql account.               *
//*********************************************************************

#include <iostream>
#include <string>
#include <my_global.h>
#include <mysql.h>
#include <termios.h>
#include <curses.h>

using namespace std;

string myget_passwd();
char mygetch();
void createTables(MYSQL &, MYSQL *, MYSQL_RES *);
void addCity(MYSQL &, MYSQL *, MYSQL_RES *);
void listCities(MYSQL &, MYSQL *, MYSQL_RES *);
void addAirline(MYSQL &, MYSQL *, MYSQL_RES *);
void listAirlines(MYSQL &, MYSQL *, MYSQL_RES *);
void addFlight(MYSQL &, MYSQL *, MYSQL_RES *);
void listFlights(MYSQL &, MYSQL *, MYSQL_RES *);
void findFlight(MYSQL &, MYSQL *, MYSQL_RES *);

int main()
{
	// mysql connection and query variables
	MYSQL *conn, // actual mysql connection
		mysql;   // local mysql data
	MYSQL_RES *res; // mysql query results

		//Strings for mysql hostname, userid, ...
	string db_host;
	string db_user;
	string db_password;
	string db_name;
	
		//User flight data.
	char commandCode;
	char decisionLetter;
	string departureCity;
	string destinationCity;
	int numConnections;


		//set up the client (this machine) to use mysql
	cout << "initializing client DB subsystem ..."; 
	cout.flush();
	mysql_init(&mysql);
	cout << "Done!" << endl;

		//get user credentials and mysql server info
	cout << "Enter MySQL database hostname (or IP adress):";
	cin >> db_host;

	cout << "Enter MySQL database username (also the database name):";
	cin >> db_user;

	cout << "Enter MySQL database password:";
	db_password=myget_passwd();

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
		cout << "DB connection established" << endl;
		
		//Set up tables for the airline information.
	createTables(mysql, conn, res);

		//Create a prompt for user to enter data.
	cout << ">>> ";
	
		//Read in the first command code from the user
		//to see what needs to be done with the data.
	cin >> commandCode;
	
		//Continue looping until they enter 'q' to quit the program.
	while(commandCode != 'q')
	{
			//Check if the users wishes to add an airline to the
			//database.
		if(commandCode == 'a')
		{			
				//Get the decision letter from the user.
			cin >> decisionLetter;
			
				//Check what they enter.
			switch(decisionLetter)
			{
					//Add a city name to the database.
				case 'c':
					addCity(mysql, conn, res);
					break;
				
					//Add an airline name to the database.
				case 'a':
					addAirline(mysql, conn, res);
					break;
				
					//Add a flight to the database.
				case 'f':
					addFlight(mysql, conn, res);
					break;
			}
		}
		
			//We want to list something from the database.
		else if(commandCode == 'l')
		{
				//Get the decision letter from the user.
			cin >> decisionLetter;
			
			//Check what they enter.
			switch(decisionLetter)
			{
					//List all the city names.
				case 'c':
					listCities(mysql, conn, res);
					break;
				
					//List all the airline names.
				case 'a':
					listAirlines(mysql, conn, res);
					break;
				
					//List all the flights including airline name,
					//departure city, arrival city, and the cost.
				case 'f':
					listFlights(mysql, conn, res);
					break;
			}
		}
		
			//Find a flight that the user requests.
		else if(commandCode == 'f')
		{
			findFlight(mysql, conn, res);
		}
		
			//Prompt the user to enter a command.
		cout << ">>> ";
		
			//Read in the next command code.
		cin >> commandCode;
		
	}

		//clean up the connection
	mysql_close(conn);

		//SUCCESS!!!
	return 0;
}

//********************************************************************//

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

//********************************************************************//

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

//********************************************************************//

void createTables(MYSQL &mysql, MYSQL *conn, MYSQL_RES *res)
{
		//Tables needed for database.
	int cityTable;
	int airlineTable;
	int flightTable;
	int cityCopy;
	int flightTableCopy;
	string createCityTable;
	string createAirlineTable;
	string createFlightTable;
	string createCityCopy;
	string createFlightCopy;
	 
		//Create the city table.
	createCityTable = "create table if not exists cityTable (CityCode";
	createCityTable += " varchar(20), CityName varchar(100),";
	createCityTable += "primary key (CityCode))"; 
	
		//Send the query.
	cout.flush(); 
	cityTable = mysql_query(conn, createCityTable.c_str());

		//if the query didn't work ...
	if (cityTable != 0)
	{
			// ... explain why ...
		cout << mysql_error(&mysql) << endl;
		
		return;  // ... and exit program
	}

		//get the query result(s)
	res = mysql_store_result(conn);
	
		//clean up the query
	mysql_free_result(res);

		//Create the airline table.
	createAirlineTable = "create table if not exists airlineTable(";
	createAirlineTable += "AirlineCode varchar(20),";
	createAirlineTable += "AirlineName varchar(100),";
	createAirlineTable += "primary key(AirlineCode))";
	
		//Send the query.
	cout.flush(); 
	airlineTable = mysql_query(conn, createAirlineTable.c_str());

		//if the query didn't work ...
	if (airlineTable != 0)
	{
			// ... explain why ...
		cout << mysql_error(&mysql) << endl;
		
		return;  // ... and exit program
	}

		//get the query result(s)
	res = mysql_store_result(conn);
	
		//clean up the query
	mysql_free_result(res);
	
		//Create the flight table.
	createFlightTable = "create table if not exists flightTable";
	createFlightTable += "(AirlineCode varchar(100),";
	createFlightTable += "DepartureCity varchar(100),";
	createFlightTable += "DestinationCity varchar(100),";
	createFlightTable += "CostOfFlight int)";;
	
		//Send the query.
	cout.flush(); 
	flightTable = mysql_query(conn, createFlightTable.c_str());

		//if the query didn't work ...
	if (flightTable != 0)
	{
			// ... explain why ...
		cout << mysql_error(&mysql) << endl;
		
		return;  // ... and exit program
	}

		//get the query result(s)
	res = mysql_store_result(conn);
	
		//clean up the query
	mysql_free_result(res);
	
		//Create a copy of the city table to use to find flights.
	createCityCopy = "create table if not exists cityCopyTable (CityCode";
	createCityCopy += " varchar(20), CityName varchar(100),";
	createCityCopy += "primary key (CityCode))"; 
	
		//Send the query.
	cout.flush(); 
	cityCopy = mysql_query(conn, createCityCopy.c_str());

		//if the query didn't work ...
	if (cityCopy != 0)
	{
			// ... explain why ...
		cout << mysql_error(&mysql) << endl;
		
		return;  // ... and exit program
	}

		//get the query result(s)
	res = mysql_store_result(conn);
	
		//clean up the query
	mysql_free_result(res);
	
	//Create the flight copy table.
	createFlightCopy = "create table if not exists flightTableCopy";
	createFlightCopy += "(AirlineCode varchar(100),";
	createFlightCopy += "DepartureCity varchar(100),";
	createFlightCopy += "DestinationCity varchar(100),";
	createFlightCopy += "CostOfFlight int)";;
	
		//Send the query.
	cout.flush(); 
	flightTableCopy = mysql_query(conn, createFlightCopy.c_str());

		//if the query didn't work ...
	if (flightTableCopy != 0)
	{
			// ... explain why ...
		cout << mysql_error(&mysql) << endl;
		
		return;  // ... and exit program
	}

		//get the query result(s)
	res = mysql_store_result(conn);
	
		//clean up the query
	mysql_free_result(res);
	
	return;
	
}

//********************************************************************//

void addCity(MYSQL &mysql, MYSQL *conn, MYSQL_RES *res)
{
	string cityCode;
	string cityName;
	int cityTableQuery;
	int cityTableCopy;
	
		//Read in the city code from the user
	cin >> cityCode;
	
		//Read in the city name from the user.
	cin.ignore();
	getline(cin, cityName);
	
		//Insert the city into the table.
	string addCityQuery = "insert into cityTable values(\"";
	addCityQuery += cityCode + "\"," + "\"" + cityName + "\")";
	
		//Send the query.
	cout.flush(); 
	cityTableQuery = mysql_query(conn, addCityQuery.c_str()); 
	

		//if the query didn't work ...
	if (cityTableQuery != 0)
	{
			// ... explain why ...
		cout << mysql_error(&mysql) << endl;
		
		return;  // ... and exit program
	}

		//get the query result(s)
	res = mysql_store_result(conn);
	
		//clean up the query
	mysql_free_result(res);	
	
		//Insert a copy of the data into the copy table.
	string addCityCopy = "insert into cityCopyTable values(\"";
	addCityCopy += cityCode + "\"," + "\"" + cityName + "\")";
	
		//Send the query.
	cout.flush(); 
	cityTableCopy = mysql_query(conn, addCityCopy.c_str()); 
	

		//if the query didn't work ...
	if (cityTableCopy != 0)
	{
			// ... explain why ...
		cout << mysql_error(&mysql) << endl;
		
		return;  // ... and exit program
	}

		//get the query result(s)
	res = mysql_store_result(conn);
	
		//clean up the query
	mysql_free_result(res);	
	
	return;
}

//********************************************************************//

void listCities(MYSQL &mysql, MYSQL *conn, MYSQL_RES *res)
{
	MYSQL_ROW row;
	int listCityQuery;
 
		//Get everything from the city table.
	cout.flush(); 
	listCityQuery = mysql_query(conn, "select * from cityTable");

		//if the query didn't work ...
	if (listCityQuery!=0)
	{
			// ... explain why ...
		cout << mysql_error(&mysql) << endl;
		
		return;  // ... and exit program
	}

		//get the query result(s)
	res = mysql_store_result(conn);

		//go through each line (row) of the answer table
	for(row=mysql_fetch_row(res);
		row!=NULL;
		row=mysql_fetch_row(res))
	{
		// print out the first 2 colums; they are stored in
		//    an "array-like" manner
		cout << row[0] << "  " << row[1] << endl;
	}

		//clean up the query
	mysql_free_result(res);
	
	return;
}

//********************************************************************//

void addAirline(MYSQL &mysql, MYSQL *conn, MYSQL_RES *res)
{
	string airlineCode;
	string airlineName;
	int airlineTableQuery;
	
		//Read in the city code from the user
	cin >> airlineCode;
	
		//Read in the city name from the user.
	cin.ignore();
	getline(cin, airlineName);
	
		//Insert an airline into the airlineTable.
	string addAirlineQuery = "insert into airlineTable values(\"";
	addAirlineQuery += airlineCode + "\"," + "\"" + airlineName + "\")";
	
		//Send the query.
	cout.flush(); 
	airlineTableQuery = mysql_query(conn, addAirlineQuery.c_str());

		//if the query didn't work ...
	if (airlineTableQuery != 0)
	{
			// ... explain why ...
		cout << mysql_error(&mysql) << endl;
		
		return;  // ... and exit program
	}

		//get the query result(s)
	res = mysql_store_result(conn);
	
		//clean up the query
	mysql_free_result(res);
	
	
	return;
}

//********************************************************************//

void listAirlines(MYSQL &mysql, MYSQL *conn, MYSQL_RES *res)
{
	MYSQL_ROW row;
	int listAirlineQuery;
 
		//Get everything from the airlineTable.
	cout.flush(); 
	listAirlineQuery = mysql_query(conn, "select * from airlineTable");

		//if the query didn't work ...
	if (listAirlineQuery!=0)
	{
			// ... explain why ...
		cout << mysql_error(&mysql) << endl;
		
		return;  // ... and exit program
	}

		//get the query result(s)
	res = mysql_store_result(conn);

		//go through each line (row) of the answer table
	for(row=mysql_fetch_row(res);
		row!=NULL;
		row=mysql_fetch_row(res))
	{
		// print out the first 2 colums; they are stored in
		//    an "array-like" manner
		cout << row[0] << "  " << row[1] << endl;
	}

		//clean up the query
	mysql_free_result(res);
	
	return;
}

//********************************************************************//

void addFlight(MYSQL &mysql, MYSQL *conn, MYSQL_RES *res)
{
	string airlineCode;
	string deptCity;
	string destCity;
	string costOfFlight;
	int flightTableQuery;
	int flightTableCopy;
	
		//Read in the abbreviation.
	cin >> airlineCode;

		//Read in the departure city.
	cin >> deptCity;
	
		//Read in the destination city.
	cin >> destCity;
	
		//Read in the cost of the flight.
	cin >> costOfFlight;
		
		//Insert a flight into the flightTable.
	string addFlightQuery = "insert into flightTable values(\"";
	addFlightQuery += airlineCode + "\"," + "\"" + deptCity + "\",";
	addFlightQuery += "\"" + destCity + "\"," + "\"" + costOfFlight;
	addFlightQuery += "\")";
	
		//Send the query.
	cout.flush(); 
	flightTableQuery = mysql_query(conn, addFlightQuery.c_str());

		//if the query didn't work ...
	if (flightTableQuery != 0)
	{
			// ... explain why ...
		cout << mysql_error(&mysql) << endl;
		
		return;  // ... and exit program
	}

		//get the query result(s)
	res = mysql_store_result(conn);
	
		//clean up the query
	mysql_free_result(res);
	
	//Insert a flight into the flightTable.
	string addFlightCopy = "insert into flightTableCopy values(\"";
	addFlightCopy += airlineCode + "\"," + "\"" + deptCity + "\",";
	addFlightCopy += "\"" + destCity + "\"," + "\"" + costOfFlight;
	addFlightCopy += "\")";
	
		//Send the query.
	cout.flush(); 
	flightTableCopy = mysql_query(conn, addFlightCopy.c_str());

		//if the query didn't work ...
	if (flightTableCopy != 0)
	{
			// ... explain why ...
		cout << mysql_error(&mysql) << endl;
		
		return;  // ... and exit program
	}

		//get the query result(s)
	res = mysql_store_result(conn);
	
		//clean up the query
	mysql_free_result(res);
	
	
	return;
}

//********************************************************************//

void listFlights(MYSQL &mysql, MYSQL *conn, MYSQL_RES *res)
{
	MYSQL_ROW row;
	int listFlightsQuery;
	string flightListQuery;
	
		//Get the list of flights.
	flightListQuery = "select airlineTable.AirlineName, \
	cityTable.CityName, cityCopyTable.CityName, flightTable.CostOfFlight \
    from airlineTable, cityTable, flightTable, cityCopyTable where "; 
	flightListQuery += "flightTable.DepartureCity = cityTable.CityCode \
	and flightTable.DestinationCity = cityCopyTable.CityCode and \
	flightTable.AirlineCode = airlineTable.AirlineCode";
	
		//Send the query.
	cout.flush(); 
	listFlightsQuery = mysql_query(conn, flightListQuery.c_str());

		//if the query didn't work ...
	if (listFlightsQuery!=0)
	{
			// ... explain why ...
		cout << mysql_error(&mysql) << endl;
		
		return;  // ... and exit program
	}

		//get the query result(s)
	res = mysql_store_result(conn);

		//go through each line (row) of the answer table
	for(row=mysql_fetch_row(res);
		row!=NULL;
		row=mysql_fetch_row(res))
	{
		cout << row[0] << ": " << row[1];
		cout << " -> " << row[2] << " $" << row[3] << endl;
	}

		//clean up the query
	mysql_free_result(res);
	
	return;
}

//********************************************************************//

void findFlight(MYSQL &mysql, MYSQL *conn, MYSQL_RES *res)
{
	MYSQL_ROW row;
	string departureCode;
	string destinationCode;
	string findFlightQuery;
	string findConnection;
	int numConnections;
	int flightQuery;
	int connectionQuery;
	
		//Read in the departure city.
	cin >> departureCode;
	
		//Read in the destination city.
	cin >> destinationCode;
	
		//Read in the number of connections.
	cin >> numConnections;
	

		//Check for a direct flight.
	if(numConnections == 0)
	{
		findFlightQuery = "select flightTable.DepartureCity, flightTable.DestinationCity, \
		flightTable.AirlineCode, flightTable.CostOfFlight from airlineTable, \
		flightTable where \"";
		findFlightQuery += departureCode + "\"=" + "flightTable.DepartureCity \
		and " + "\"" + destinationCode + "\"" + "= flightTable.DestinationCity";
		
			//Send the query.
		cout.flush(); 
		flightQuery = mysql_query(conn, findFlightQuery.c_str());

			//if the query didn't work ...
		if(flightQuery != 0)
		{
				// ... explain why ...
			cout << mysql_error(&mysql) << endl;
		
			return;  // ... and exit program
		}

			//get the query result(s)
		res = mysql_store_result(conn);

			//go through each line (row) of the answer table
		for(row=mysql_fetch_row(res);
			row!=NULL;
			row=mysql_fetch_row(res))
		{
				//Print out the flight that was found.
			cout << row[0] << " -> " << row[1];
			cout << ": " << row[2] << " $" << row[3] << endl;
		}
		
		return;

	}
	
	else
	{
		int totalCost = 0;
		
		findConnection = "select flightTable.DepartureCity, \
		flightTable.DestinationCity, flightTable.AirlineCode, \
		flightTable.CostOfFlight, flightTableCopy.DepartureCity, \
		flightTableCopy.DestinationCity, flightTableCopy.AirlineCode, \
		flightTableCopy.CostOfFlight from flightTable, flightTableCopy where \"";
		findConnection += departureCode + "\"";
		findConnection += "= flightTable.DepartureCity and ";
		findConnection += "\"" + destinationCode + "\"";
		findConnection += " != flightTable.DestinationCity and ";
		findConnection += "\"" + departureCode + "\"";
		findConnection += " != flightTableCopy.DepartureCity and ";
		findConnection += "\"" + destinationCode + "\"";
		findConnection += "= flightTableCopy.DestinationCity";
		
		//Send the query.
		cout.flush(); 
		connectionQuery = mysql_query(conn, findConnection.c_str());

			//if the query didn't work ...
		if(connectionQuery != 0)
		{
				// ... explain why ...
			cout << mysql_error(&mysql) << endl;
		
			return;  // ... and exit program
		}

			//get the query result(s)
		res = mysql_store_result(conn);

			//go through each line (row) of the answer table
		for(row=mysql_fetch_row(res);
			row!=NULL;
			row=mysql_fetch_row(res))
		{
				//Print out the flight that was found.
			cout << row[0] << " -> " << row[1];
			cout << ": " << row[2] << " $" << row[3] << " ";
			cout << row[4] << " -> " << row[5];
			cout << ": " << row[6] << " $" << row[7] << ", ";
			
			totalCost += atoi(row[3]);
			totalCost += atoi(row[7]);
			
			break;
		}
		
			//Print out the total cost of the flight.
		cout << "for a total cost of $" << totalCost << endl;
		
		return;
	}
}
