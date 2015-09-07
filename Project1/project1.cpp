#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

class CarClass
{
	public:
		void addCar(ofstream &);
		void addManufacturer(ofstream &, ifstream &);
		void addDealer(ofstream &);
		void listCars(ifstream &);
		void listDealers(ifstream &);
		void findManufacturer(ifstream &, ifstream &, ifstream &);
		void loadFiles(ifstream &, ifstream &, ifstream &);
	private:
			// The following are all information regarding car info
		string VIN;
		string dealer;
		int miles;
		int cost;

		vector<string> myVINS;
		vector<string> carsDealers;
		vector<int> myMiles;
		vector<int> myCosts;

			// Manufacturer info: abbreviation and name
		string manufacturerAbb;
		string manufacturer;

		vector<string> manuAbbreviations;
		vector<string> allManufacturers;

			// Dealer information
		string dealerName;
		int zipCode;
		string phoneNumber;
		vector<string> myDealers;
		vector<int> myZipCodes;
		vector<string> myPhoneNumbers;

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

	myCar.loadFiles(myCars, myManufacturers, myDealers);


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
					myCar.findManufacturer(myManufacturers, myCars, myDealers);
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

	myVINS.push_back(VIN);
	myMiles.push_back(miles);
	carsDealers.push_back(dealer);
	myCosts.push_back(cost);

	carFile << VIN << " " << miles << " " << dealer << " " << cost << endl;
}

//**********************************************************************

void CarClass::addManufacturer(ofstream &manufacturerFile, ifstream &myManufacturers)
{
	string searchLine;
	string duplicate;

	cin >> manufacturerAbb;
	cin >> manufacturer;

	manuAbbreviations.push_back(manufacturerAbb);
	allManufacturers.push_back(manufacturer);
	manufacturerFile << manufacturerAbb << " " << manufacturer << endl;
}

//**********************************************************************

void CarClass::addDealer(ofstream &dealerFile)
{
	cin >> dealerName;
	cin >> zipCode;
	cin >> phoneNumber;

	myDealers.push_back(dealerName);
	myZipCodes.push_back(zipCode);
	myPhoneNumbers.push_back(phoneNumber);
	dealerFile << dealerName << " " << zipCode << " " << phoneNumber << endl;
}

//**********************************************************************

void CarClass::listCars(ifstream &myCars)
{
	//myCars >> VIN >> miles >> dealer >> cost;

	while(myCars.good())
	{
		cout << VIN << " " << miles << " " << dealer << " " << cost << endl;
		//myCars >> VIN >> miles >> dealer >> cost;
	}

	for(unsigned x = 0; x < myVINS.size(); x++)
	{
		cout << myVINS[x] << " " << myMiles[x] << " " << carsDealers[x] << " " << myCosts[x] << endl;
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

void CarClass::findManufacturer(ifstream &myManufacturers, ifstream &myCars, ifstream &dealers)
{
	string manu;
	string manuAbb;
	string currentVIN;

	cin >> manu;

	for(unsigned int x = 0; x < allManufacturers.size(); x++)
	{
		if(manu.compare(allManufacturers[x]) == 0)
		{
			manuAbb = manuAbbreviations[x];
		}
	}

	for(unsigned x = 0; x < myVINS.size(); x++)
	{
		if(manuAbb.compare(myVINS[x].substr(0,3)) == 0)
		{
			currentVIN = myVINS[x];
			//cout << currentVIN << endl;

			for(unsigned y = 0; y < myDealers.size(); y++)
			{
				if(carsDealers[x].compare(myDealers[y]))
				{
					cout << manu << ": " << myMiles[x] << " miles, $" << myCosts[x] << ": "
					<< carsDealers[x] << "["<< "(" << myPhoneNumbers[x].substr(0,3) << ")" <<
					myPhoneNumbers[x].substr(3,3) << "-" << myPhoneNumbers[x].substr(6,4) << "]"<< endl;
					break;
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

void CarClass::loadFiles(ifstream &cars, ifstream &manufacturers, ifstream &dealers)
{
	string loadVIN;
	int loadMiles;
	string loadDealers;
	int loadCost;

	string loadManuAbb;
	string loadManufacturer;

	string loadDealer;
	int loadZip;
	string loadPhone;

	while(cars >> loadVIN >> loadMiles >> loadDealers >> loadCost)
	{
		myVINS.push_back(loadVIN);
		myMiles.push_back(loadMiles);
		carsDealers.push_back(loadDealers);
		myCosts.push_back(loadCost);
	}

	while(manufacturers >> loadManuAbb >> loadManufacturer)
	{
		manuAbbreviations.push_back(loadManuAbb);
		allManufacturers.push_back(loadManufacturer);
	}

	while(dealers >> loadDealer >> loadZip >> loadPhone)
	{
		myDealers.push_back(loadDealer);
		myZipCodes.push_back(loadZip);
		myPhoneNumbers.push_back(loadPhone);
	}

	/*std::vector<int>::iterator itr;
	for ( itr = myCosts.begin(); itr != myCosts.end(); ++itr )
	{
		cout << *itr;
	}*/
}
