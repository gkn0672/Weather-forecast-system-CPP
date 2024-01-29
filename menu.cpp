#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "menu.h"
#include "algo.h"
#include "city.h"
#include "cloud.h"
#include "pressure.h"
#include "weather.h"
#include "plot.h"

string student_ID = "7559070";
string student_name = "Do Truong Giang";

bool in = false;
vector <string> info;
string file_in;

string** city_map;
vector <string> raw_city;
vector <string> raw_name;
vector <string> raw_id;

string** cloud_map;
string** cloud_converted;
vector <string> raw_cloud;
int** cloud_orginal;

string** pressure_map;
string** pressure_converted;
vector <string> raw_pressure;
int** pressure_orginal;

vector <City_report> alist;
int width;
int height;

//Display main menu
int Displaymenu() {
    string choice;
	while(true) {
		cout << "======================================" << endl;
		cout << "Student ID   : " << student_ID << endl;
		cout << "Student Name : " << student_name << endl;
		cout << "======================================" << endl;
		cout << "Welcome to Weather Information Processing System!\n" << endl;
		cout << "1)\tRead in and process a configuration file" << endl;

		// If configuration file already read
		if(in) {
			cout << "2)\tDisplay city map" << endl;
			cout << "3)\tDisplay cloud coverage map (cloudiness index)" << endl;
			cout << "4)\tDisplay cloud coverage map (LMH symbols)" << endl;
			cout << "5)\tDisplay atmospheric pressure map (pressure index)" << endl;
			cout << "6)\tDisplay atmospheric pressure map (LMH symbols index)" << endl;
			cout << "7)\tShow weather forecast summary report" << endl;
			cout << "8)\tWeather data analyzer" << endl;
		}
		cout << "9)\tQuit\n" << endl;
        cout << "Please enter your choice: ";
		while(choice == "") {
			cin >> choice;
		}

		//Process user input
		choice = TrimSpace(choice);
		bool result = Prompt(choice);

		//If user choose quit
        if(result) break;

		//Reset choice
		choice = "";
    }
    return 0;
}

//Handle user input
bool Prompt(string choice) {
	if(choice == "1") {
		// Read configuration file
		Read();

		//Allocate memory
		Generate();

		//Pre-process data
		City(height, width, raw_city, city_map, raw_name, raw_id);
		Cloud(height, width, raw_cloud, cloud_map, cloud_converted, cloud_orginal);
		Pressure(height, width, raw_pressure, pressure_map, pressure_converted, pressure_orginal);
		alist.clear();

		//Process weather report
		ComputeWeatherData(height, width, raw_name, raw_id, city_map, cloud_map, pressure_map, pressure_orginal, cloud_orginal, alist);
		return false;

	} else if((choice == "2") && (in)) {
		ClearScreen();
		cout << endl;
		cout << "[Display city map]\n" << endl;

		//Display city map
		Render(city_map, width, height);
		Freeze();
		ClearScreen();
    	return false; 
	} else if((choice == "3") && (in)) {
		ClearScreen();
		cout << endl;
		cout << "[Display cloud coverage map (cloudiness index)]\n" << endl;

		//Display cloud map (cloud index)
		Render(cloud_map, width, height);
		Freeze();
		ClearScreen();
    	return false;
	} else if((choice == "4") && (in)) {
		ClearScreen();
		cout << endl;
		cout << "[Display cloud coverage map (LMH symbols)]\n" << endl;

		//Display cloud map (LMH format)
		Render(cloud_converted, width, height);
		Freeze();
		ClearScreen();
    	return false;
	} else if((choice == "5") && (in)) {
		ClearScreen();
		cout << endl;
		cout << "[Display atmospheric pressure map (pressure index)]\n" << endl;

		//Display pressure map (pressure index)
		Render(pressure_map, width, height);
		Freeze();
		ClearScreen();
    	return false;
	} else if((choice == "6") && (in)) {
		ClearScreen();
		cout << endl;
		cout << "[Display atmospheric pressure map (LMH symbols index)]\n" << endl;

		//Display pressure map (LMH format)
		Render(pressure_converted, width, height);
		Freeze();
		ClearScreen();
    	return false;
	} else if((choice == "7") && (in)) {
		ClearScreen();
		cout << endl;
		cout << "Weather Forecast Summary Report" << endl;
		cout << "-------------------------------" << endl;

		//Display weather forecase report
		Weather(alist);
		Freeze();
		ClearScreen();
		return false;
	} else if((choice == "8") && (in)) {
		cout << endl;
		cout << "[Virtual Solution quick weather data analyzer]" << endl;

		//Analyze data
		Plot(alist);
		Freeze();
		ClearScreen();
		return false;
	} else if(choice == "9") {
		cout << endl;
		cout << "[Quit]";

		//Reclaim memory
		Reclaim();
		MySleep(1);
		ClearScreen();

		//Signal program to terminate
		return true;
	} else {
		cout << endl;
		ClearScreen();
		return false;	
	}
}

//Handle configuration file
void Read() {
	fstream inputFile;
	string aLine;
	vector <string> v;
	cout << endl;

	cout << "[Read in and process a configuration file]" << endl;
	cout << "Please enter config file name: ";
	cin >> file_in;
	inputFile.open(file_in.c_str());
    cout << endl;
    info.clear();

	//Extracting data from configuration file
    if(!inputFile) {
    	cout << "Error when reading input file" << endl;
    	cout << endl;
	} else {
    	while (inputFile) {
    		getline (inputFile, aLine);
    		if(aLine[0] == '/') continue;
    		if(aLine == "") continue;
    		info.push_back(aLine);
		}
		inputFile.close();
		cout << "Reading in " << info[0] << " " << "...";
		cout << " done!" << endl;
		cout << "Reading in " << info[1] << " " << "...";
		cout << " done!" << endl;
		cout << endl;
		cout << "Storing data from input file :" << endl;

		v = ProcessString(info[0], "-");
		width = ConvertStringToInteger(v[1]);
		v = ProcessString(info[1], "-");
		height = ConvertStringToInteger(v[1]);
		width ++;
		height ++;

		//Read other text file
		raw_city = ReadFile(info[2]);
		raw_cloud = ReadFile(info[3]);
		raw_pressure = ReadFile(info[4]);

		cout << endl;
		cout << "All records successfully stored. Going back to main menu ... " << endl;
		if(!in) in = true;
		cout << endl;
	}
	MySleep(2);
	ClearScreen();
}

//Allocate memory using new
void Generate() {
	city_map = new string*[height];
    GenerateStringMap(city_map, width, height);

	cloud_map = new string*[height];
	cloud_converted = new string*[height];
	cloud_orginal = new int*[height];
	
    GenerateStringMap(cloud_map, width, height);
    GenerateStringMap(cloud_converted, width, height);
    GenerateIntegerMap(cloud_orginal, width, height);

	pressure_map = new string*[height];
	pressure_converted = new string*[height];
	pressure_orginal = new int*[height];
	GenerateIntegerMap(pressure_orginal, width, height);
    GenerateStringMap(pressure_map, width, height);
    GenerateStringMap(pressure_converted, width, height);
}

//Reclaim memory using delete
void Reclaim() {
	DeleteStringMap(city_map, height);
	DeleteStringMap(cloud_map, height);
	DeleteStringMap(cloud_converted, height);
	DeleteIntMap(cloud_orginal, height);
	DeleteStringMap(pressure_map, height);
	DeleteStringMap(pressure_converted, height);
	DeleteIntMap(pressure_orginal, height);
}

