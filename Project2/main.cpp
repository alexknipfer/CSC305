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
					//addCar();
					cout << "add car" << endl;
					break;
					
					//add manufacturer if type entered was 'm'
				case 'm':
					//addManufacturer();
					cout << "add manufacturer" << endl;
					break;
					
					//add dealer if type entered was 'd'
				case 'd':
					//addDealer();
					cout << "add dealer" << endl;
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
	
	int carQuery;
	int manuQuery;
	int dealerQuery;

	carTable  = "create table if not exists carTable (vin char(100), miles integer, dealer char(50), year integer, primary key(vin))";
	cout.flush();
	carQuery = mysql_query(conn, carTable.c_str());
	
	// if the query didn't work ...
	if (carQuery!=0)
	{
		// ... explain why ...
		cout << "ERROR" << endl;
		return 1;  // ... and exit program
	}
		
	manuTable  = "create table if not exists manuTable (manuAbb char(3), manu char(50), primary key(manuAbb))";
	cout.flush();
	manuQuery = mysql_query(conn, manuTable.c_str());
	
		// if the query didn't work ...
	if (manuQuery!=0)
	{
		// ... explain why ...
		cout << "ERROR" << endl;
		return 1;  // ... and exit program
	}
	
	dealerTable  = "create table if not exists dealerTable (dealerName char(50), zipCode integer, phoneNumber char(10), primary key(dealerName))";
	cout.flush();
	dealerQuery = mysql_query(conn, dealerTable.c_str());
	
		// if the query didn't work ...
	if (dealerQuery!=0)
	{
		// ... explain why ...
		cout << "ERROR" << endl;
		return 1;  // ... and exit program
	}	
	
	
	return 0;
}

