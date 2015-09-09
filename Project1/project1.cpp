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
		void addManufacturer(ofstream &);
		void addDealer(ofstream &);
		void listCars(ifstream &);
		void listDealers(ifstream &);
		void findManufacturer(ifstream &, ifstream &, ifstream &);
		void findZipCode(ifstream &, ifstream &, ifstream &);
		void loadFiles(ifstream &, ifstream &, ifstream &);
		void clearVectors();
	private:
			//The following are all information regarding car info
		string VIN;
		string dealer;
		int miles;
		int cost;
			//Vectors keep car info organized
		vector<string> myVINS;
		vector<string> carsDealers;
		vector<int> myMiles;
		vector<int> myCosts;

			//Manufacturer info: abbreviation and name
		string manufacturerAbb;
		string manufacturer;
			//Vectors keep manufacture info organized
		vector<string> manuAbbreviations;
		vector<string> allManufacturers;

			//Dealer information
		string dealerName;
		int zipCode;
		string phoneNumber;
			//Vectors keep dealer info organized
		vector<string> myDealers;
		vector<int> myZipCodes;
		vector<string> myPhoneNumbers;

};

//******************************************************************************

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
			//loads data from files into vectors when the program runs
		myCar.loadFiles(myCars, myManufacturers, myDealers);

		cin >> typeToAdd;

			//take input from user and do the following based on what they want:
			// add car, add manufacturer, add car dealer
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

			//take input from user and do the following:
			//list all cars, list all dealers
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

			//take input from user and do the following:
			//find cars based on manufacturer or based on zip code
		if(tag == 'f')
		{
			switch(typeToAdd)
			{
				case 'm':
					myCar.findManufacturer(myManufacturers, myCars, myDealers);
					break;
				case 'z':
					myCar.findZipCode(myManufacturers, myCars, myDealers);
					break;
			}
		}

		cout << ">>>";
		cin >> tag;

			//clear the vectors out
		myCar.clearVectors();
	}
	return 0;
}

//******************************************************************************

void CarClass::addCar(ofstream &carFile)
{
		//Given - car file
		//Task - add car info from user input
		//Returns - nothing

		//read in car info
	cin >> VIN;
	cin >> miles;
	cin >> dealer;
	cin >> cost;

		//searches manufacturer abbreviation vector, if manufacturer doesn't exist, don't add car
	if(std::find(manuAbbreviations.begin(), manuAbbreviations.end(), VIN.substr(0,3)) != manuAbbreviations.end())
	{
			//make sure car dealer is added before adding car
		if(std::find(myDealers.begin(), myDealers.end(), dealer) != myDealers.end())
		{
				//add car info to file
			carFile << VIN << " " << miles << " " << dealer << " " << cost << endl;
		}
		else
		{
			cout << "Please add dealer before adding car!" <<endl;
		}
	}
	else
	{
		cout << "Please add manufacturer! Does not exist!" << endl;
	}
}

//******************************************************************************

void CarClass::addManufacturer(ofstream &manufacturerFile)
{
		//Given - Manufacturer file
		//Task - add manufacturer info from user
		//Returns - nothing

		//read in manufacturer abbreviation and name
	cin >> manufacturerAbb;
	cin >> manufacturer;

		//if the user already added a specific abbreviation, be sure it can't be used twice
	if(std::find(manuAbbreviations.begin(), manuAbbreviations.end(), manufacturerAbb) != manuAbbreviations.end())
	{
		cout << "Manufacturer already exists!" << endl;
	}

		//if the user already added a manufacturer make sure they can't reuse the name
	else if(std::find(allManufacturers.begin(), allManufacturers.end(), manufacturer) != allManufacturers.end())
	{
		cout << "Manufacturer already exists!" << endl;
	}

	else
	{
		//add manufacturer info to file
		manufacturerFile << manufacturerAbb << " " << manufacturer << endl;
	}
}

//******************************************************************************

void CarClass::addDealer(ofstream &dealerFile)
{
		//Given - car dealer files
		//Task - add dealer info from user
		//Returns - nothing

		//read in dealer name, zip code, and phone number from user
	cin >> dealerName;
	cin >> zipCode;
	cin >> phoneNumber;

	if(std::find(myDealers.begin(), myDealers.end(), dealerName) != myDealers.end())
	{
		cout << "Dealer already exists!" << endl;
	}

	else
	{
			//print dealer info to file
		dealerFile << dealerName << " " << zipCode << " " << phoneNumber << endl;	
	}
}

//******************************************************************************

void CarClass::listCars(ifstream &cars)
{
		//Given - car file
		//Task - list all cars
		//Returns - nothing

		//be sure to always read from top of file
	cars.clear();
	cars.seekg(0, ios::beg);

		//read in car info
	cars >> VIN >> miles >> dealer >> cost;

		//as long as the file contains car info, continue to print all car info out for all cars
	while(cars.good())
	{
		cout << VIN << " " << miles << " " << dealer << " " << cost << endl;
		cars >> VIN >> miles >> dealer >> cost;
	}
}

//******************************************************************************

void CarClass::listDealers(ifstream &dealers)
{
		//Given - car dealer file
		//Task - list all car dealers
		//Returns - nothing

		// Go back and read from top of file
	dealers.clear();
	dealers.seekg(0, ios::beg);

	dealers >> dealerName >> zipCode >> phoneNumber;

		//continue to read in dealers and print them as long as they exist in the file
	while(dealers.good())
	{
		cout << dealerName << " " << zipCode << " " << phoneNumber << " " << endl;
		dealers >> dealerName >> zipCode >> phoneNumber;
	}
}

//******************************************************************************

void CarClass::findManufacturer(ifstream &manufacturers, ifstream &cars, ifstream &dealers)
{
		//Given - manufacturer file, cars file, car dealer file
		//Task - find all cars based on the manufacturer entered
		//Returns - nothing

		//stores manufacturer entered
	string manu;
	string manuAbb;

		//all the following make sure the file is being read from the top
	manufacturers.clear();
	manufacturers.seekg(0, ios::beg);

	cars.clear();
	cars.seekg(0, ios::beg);

	dealers.clear();
	dealers.seekg(0, ios::beg);

	cin >> manu;

		//loop through manufacturer vector to find abbreviation for manufacturer that was entered
	for(unsigned int x = 0; x < allManufacturers.size(); x++)
	{
		if(manu.compare(allManufacturers[x]) == 0)
		{
				//store the abbreviation for the manufacturer that was entered
			manuAbb = manuAbbreviations[x];
		}
	}

		//loop through all car VINS
	for(unsigned int x = 0; x < myVINS.size(); x++)
	{
			//compare the abbreviation to the first 3 of all VINS to find cars of that manufacturer
		if(manuAbb.compare(myVINS[x].substr(0,3)) == 0)
		{
				//loop through all car dealers
			for(unsigned int y = 0; y < myDealers.size(); y++)
			{
					//find car dealer that matches the car in order to find phone number for that dealer
					//once finding the location in the vector of the corresponding dealer, the phone number
					//will be in that same location in the phone number vector
				if(carsDealers[x].compare(myDealers[y]) == 0)
				{
						//print out the cars found for the specific manufacturer
					cout << manu << ": " << myMiles[x] << " miles, $" << myCosts[x] << ": "
					<< carsDealers[x] << "["<< "(" << myPhoneNumbers[y].substr(0,3) << ")" <<
					myPhoneNumbers[y].substr(3,3) << "-" << myPhoneNumbers[y].substr(6,4) << "]"<< endl;
					break;
				}
			}
		}
	}
}

//******************************************************************************

void CarClass::findZipCode(ifstream &manufacturers, ifstream &cars, ifstream &dealers)
{
		//Given - manufacturer file, cars file, dealers file
		//Task - find all cars in the zip code entered by user
		//Returns - nothing

	int currentZip;
	string currentDealer;

		//be sure to read from top of all files
	manufacturers.clear();
	manufacturers.seekg(0, ios::beg);

	cars.clear();
	cars.seekg(0, ios::beg);

	dealers.clear();
	dealers.seekg(0, ios::beg);

		//read in zip to find cars in that zip code
	cin >> currentZip;

		//loop through all the zip codes in the vector
	for(unsigned x = 0; x < myZipCodes.size(); x++)
	{
			//match the zip code entered to the one in the vector
		if(currentZip == myZipCodes[x])
		{
				//store that location in the dealers file into current dealer
			currentDealer = myDealers[x];

				//loop through all car dealers
			for(unsigned y = 0; y < carsDealers.size(); y++)
			{
					//compare the dealer found to all dealers for match
				if(currentDealer.compare(carsDealers[y]) == 0)
				{
						//loop through all manufacturer abbreviations
					for(unsigned z = 0; z < manuAbbreviations.size(); z++)
					{
							//find matching abbreviation to get manufacturer name to print out
						if(manuAbbreviations[z].compare(myVINS[y].substr(0,3)) == 0)
						{
								//print out car info
							cout << allManufacturers[z] << ": " << myMiles[y] << " miles, $" << myCosts[y] <<
							": " << currentDealer << "["<< "(" << myPhoneNumbers[x].substr(0,3) << ")" <<
							myPhoneNumbers[x].substr(3,3) << "-" << myPhoneNumbers[x].substr(6,4) << "]"<< endl;
						}
					}
				}
			}
		}
	}
}

//*****************************************************************************************

void CarClass::loadFiles(ifstream &cars, ifstream &manufacturers, ifstream &dealers)
{
		//Given - cars file, manufacturer file, car dealer file
		//Task - load all data from files into vectors
		//Returns - nothing

	string loadVIN;
	int loadMiles;
	string loadDealers;
	int loadCost;

	string loadManuAbb;
	string loadManufacturer;

	string loadDealer;
	int loadZip;
	string loadPhone;

		//be sure to read from top of all files
	manufacturers.clear();
	manufacturers.seekg(0, ios::beg);

	cars.clear();
	cars.seekg(0, ios::beg);

	dealers.clear();
	dealers.seekg(0, ios::beg);

		//read in car info from cars file and add each to their corresponding vectors
	while(cars >> loadVIN >> loadMiles >> loadDealers >> loadCost)
	{
		myVINS.push_back(loadVIN);
		myMiles.push_back(loadMiles);
		carsDealers.push_back(loadDealers);
		myCosts.push_back(loadCost);
	}

		//read in manufacturer info from file and add each to their corresponding vectors
	while(manufacturers >> loadManuAbb >> loadManufacturer)
	{
		manuAbbreviations.push_back(loadManuAbb);
		allManufacturers.push_back(loadManufacturer);
	}

		//read in car dealer info from file and add each to their corresponding vectors
	while(dealers >> loadDealer >> loadZip >> loadPhone)
	{
		myDealers.push_back(loadDealer);
		myZipCodes.push_back(loadZip);
		myPhoneNumbers.push_back(loadPhone);
	}
}

//*****************************************************************************************

void CarClass::clearVectors()
{
		//Given - nothing
		//Task - empty out all vectors
		//Returns - nothing

		//all the following clear out everything from each vector
	myVINS.clear();
	myMiles.clear();
	carsDealers.clear();
	myCosts.clear();

	manuAbbreviations.clear();
	allManufacturers.clear();

	myDealers.clear();
	myZipCodes.clear();
	myPhoneNumbers.clear();
}

//**************** END OF PROGRAM *********************************************************
