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
	private:
			// The following are all information regarding car info
		string VIN;
		string dealer;
		int miles;
		int cost;
		vector<string> myQueue;

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

void CarClass::findManufacturer(ifstream &myManufacturers, ifstream &myCars, ifstream &dealers)
{
	std::string line;
	std::string carLine;
	string manu;

	std::string dealersLine;
	string foundDealer;
	size_t dealPos = 0;
	int y = 0;
	string dealerToken;

	string delimiter = " ";	//tokenize string after hitting space
	string token[4];	//Holds each value of the tokenized string
	size_t pos = 0;	//holds position during string tokenize
	int x = 0;	//initialize array value

	cin >> manu;

	while(getline(myManufacturers, line))
	{
		if(line.find(manu) != string::npos)
		{
			foundManu = line.substr(0,3);
		}
	}

	while(getline(myCars, carLine))
	{
		if(carLine.find(foundManu) != string::npos)
		{
			while((pos = carLine.find(delimiter)) != string::npos)
			{
				token[x++] = carLine.substr(0,pos);
				carLine.erase(0, pos + delimiter.length());
			}
		}
		cout << token[2] << endl;
		x = 0;
	}




	/*while(std::getline(myManufacturers, line))
	{
		if(line.find(manu) != std::string::npos)
		{
			foundManu = line.substr(0,3);

			while(std::getline(myCars, carLine))
			{
				if(carLine.find(foundManu) != std::string::npos)
				{
					while ((pos = carLine.find(delimiter)) != std::string::npos)
					{
    					token[x++] = carLine.substr(0, pos);
    					carLine.erase(0, pos + delimiter.length());


								//finds dealers in dealer file
							while(std::getline(dealers, dealersLine))
							{
								if(dealersLine.find(token[2]) != std::string::npos)
								{
									foundDealer = dealersLine;
									cout << dealersLine << endl;

									while((dealPos = foundDealer.find(delimiter)) != std::string::npos)
									{
										dealerToken = foundDealer.substr(foundDealer.length()-10, foundDealer.length());
										foundDealer.erase(0, dealPos + delimiter.length());
										//cout << dealerToken << endl;
										myQueue.push_back(dealerToken);
										break;
									}
								}

							}
					}
						//cout  << manu << ": " << token[1] << " miles, "<< " $" << carLine << ": " << myQueue[dealPos] << endl;
						myQueue.pop_back();
						x = 0;
				}
			}
		}
	}*/

		// Go back and read from top of file
	myManufacturers.clear();
	myManufacturers.seekg(0, ios::beg);
	myCars.clear();
	myCars.seekg(0,ios::beg);
}

//**********************************************************************
