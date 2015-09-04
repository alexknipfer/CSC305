#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

class CarClass
{
	public:
		void addCar(ofstream &);
		void addManufacturer(ofstream &, ifstream &);
		void addDealer(ofstream &);
		void listCars(ifstream &);
		void listDealers(ifstream &);
		void findManufacturer(ifstream &, ifstream &);
	private:
			// The following are all information regarding car info
		string VIN;		
		string dealer;	
		int miles;		
		int cost;		
		
			// Manufacturer info: abbreviation and name
		string manufacturerAbb;
		string manufacturer;
		
			// Dealer information
		string dealerName;
		int zipCode;
		string phoneNumber;
		
		string foundManu;
};

//**********************************************************************

int main()
{
	CarClass myCar;
	
	ofstream carFile("cars.txt", fstream::app);
	ofstream manufacturerFile("manufacturer.txt", fstream::app);
	ofstream dealerFile("dealer.txt", fstream::app);
	
	ifstream myCars("cars.txt");
	ifstream myDealers("dealer.txt");
	ifstream myManufacturers("manufacturer.txt");
	
	
	char tag;		//add, list, or find
	char typeToAdd;	// what is being added
	
	cout << ">>>";
	cin >> tag;
	
		// continue to ask for input unless the user types "q" then terminate
	while(tag != 'q')
	{
		cin >> typeToAdd;

		if(tag == 'a')
		{
			switch(typeToAdd)
			{
				case 'c':
					myCar.addCar(carFile);
					break;
				case 'm':
					myCar.addManufacturer(manufacturerFile, myManufacturers);
					break;
				case 'd':
					myCar.addDealer(dealerFile);
					break;
			}
		}
		
		if(tag == 'l')
		{
			switch(typeToAdd)
			{
				case 'c':
					myCar.listCars(myCars);
					break;
				case 'd':
					myCar.listDealers(myDealers);
					break;
			}
		}
		
		if(tag == 'f')
		{
			switch(typeToAdd)
			{
				case 'm':
					myCar.findManufacturer(myManufacturers, myCars);
					break;
			}
		}
		
		cout << ">>>";
		cin >> tag;
		
	}
	
	return 0;
}

//**********************************************************************

void CarClass::addCar(ofstream &carFile)
{
	cin >> VIN;
	cin >> miles;
	cin >> dealer;
	cin >> cost;
			
	carFile << VIN << " " << miles << " " << dealer << " " << cost << endl;
}

//**********************************************************************

void CarClass::addManufacturer(ofstream &manufacturerFile, ifstream &myManufacturers)
{
	string searchLine;
	string duplicate;
	
	cin >> manufacturerAbb;
	cin >> manufacturer;
	
	manufacturerFile << manufacturerAbb << manufacturer << endl;	
}

//**********************************************************************

void CarClass::addDealer(ofstream &dealerFile)
{
	cin >> dealerName;
	cin >> zipCode;
	cin >> phoneNumber;
	
	dealerFile << dealerName << " " << zipCode << " " << phoneNumber << endl;
}

//**********************************************************************

void CarClass::listCars(ifstream &myCars)
{
	myCars >> VIN >> miles >> dealer >> cost;
	
	while(!myCars.eof()) //myCars.good() 
	{
		cout << VIN << " " << miles << " " << dealer << " " << cost << endl;
		myCars >> VIN >> miles >> dealer >> cost;
	}
	
		// Go back and read from top of file
	myCars.clear();
	myCars.seekg(0, ios::beg);
}

//**********************************************************************

void CarClass::listDealers(ifstream &myDealers)
{
	myDealers >> dealerName >> zipCode >> phoneNumber;
	
	while(myDealers.good())
	{
		cout << dealerName << " " << zipCode << " " << phoneNumber << " " << endl;
		myDealers >> dealerName >> zipCode >> phoneNumber;
	}	
	
		// Go back and read from top of file
	myDealers.clear();
	myDealers.seekg(0, ios::beg);	
}

//**********************************************************************

void CarClass::findManufacturer(ifstream &myManufacturers, ifstream &myCars)
{
	std::string line;
	std::string carLine;
	string manu;
	
	cin >> manu;
	
	while(std::getline(myManufacturers, line))
	{
		if(line.find(manu) != std::string::npos)
		{
			foundManu = line.substr(0,3);
			//cout << foundManu << endl;
			
			while(std::getline(myCars, carLine))
			{
				if(carLine.find(foundManu) != std::string::npos)
				{
					std::cout << carLine << std::endl;
				}
			}
		}
	}
	
		// Go back and read from top of file
	myManufacturers.clear();
	myManufacturers.seekg(0, ios::beg);
	myCars.clear();
	myCars.seekg(0,ios::beg);
}

//**********************************************************************
