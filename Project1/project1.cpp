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
};

//**********************************************************************

int main()
{
	CarClass myCar;
	
	ofstream carFile("cars.txt", fstream::app);
	ofstream manufacturerFile("manufacturer.txt");
	ofstream dealerFile("dealer.txt");
	
	
	char tag;		//add, list, or find
	char typeToAdd;	// what is being added
	
	cout << ">>>";
	cin >> tag;
	
	while(tag != 'q')
	{
		cin >> typeToAdd;
		
		if(typeToAdd == 'c')
		{
			myCar.addCar(carFile);	
		}
		
		if(typeToAdd == 'm')
		{
			myCar.addManufacturer(manufacturerFile);
		}
		
		if(typeToAdd == 'd')
		{
			myCar.addDealer(dealerFile);
		}
		
		cout << ">>>";
		cin >> tag;
		
	}
	
	return 0;
}

void CarClass::addCar(ofstream &carFile)
{
	cin >> VIN;
	cin >> miles;
	cin >> dealer;
	cin >> cost;
			
	carFile << VIN << " " << miles << " " << dealer << " " << cost << endl;
}

void CarClass::addManufacturer(ofstream &manufacturerFile)
{
	cin >> manufacturerAbb;
	cin >> manufacturer;
			
	manufacturerFile << manufacturerAbb << " " << manufacturer << endl;
}

void CarClass::addDealer(ofstream &dealerFile)
{
	cin >> dealerName;
	cin >> zipCode;
	cin >> phoneNumber;
	
	dealerFile << dealerName << " " << zipCode << " " << phoneNumber << endl;
}


