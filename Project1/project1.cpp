#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

class CarClass
{
	public:
		void addCar(ofstream &);
		void addManufacturer(ofstream &);
		void addDealer(ofstream &);
		void listCars(ifstream &);
		void listDealers(ifstream &);
	private:
		string VIN;		// VIN for car
		string dealer;	// Dealer name
		int miles;		//miles car has on it
		int cost;		//cost of car
		string manufacturerAbb;		//manufacturer abbreviation
		string manufacturer;		//manufacturer name
		string dealerName;
		int zipCode;
		string phoneNumber;
		
		/*
		int listMiles;
		int listCost;
		string listDealer;
		string listVIN;
		
		string nameDealer;
		int listZip;
		string listPhone;*/
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
	
	
	char tag;		//add, list, or find
	char typeToAdd;	// what is being added
	
	cout << ">>>";
	cin >> tag;
	
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
					myCar.addManufacturer(manufacturerFile);
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

void CarClass::addManufacturer(ofstream &manufacturerFile)
{
	cin >> manufacturerAbb;
	cin >> manufacturer;
			
	manufacturerFile << manufacturerAbb << " " << manufacturer << endl;
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


