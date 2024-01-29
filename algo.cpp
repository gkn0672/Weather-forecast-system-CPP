#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "algo.h"

/* Remove extra blank space from a string */
string TrimSpace(string str) {
	str.erase(remove(str.begin(),str.end(),' '),str.end());
	return str;
}

/* Read weather data from a file */
vector<string> ReadFile(string name) {
    fstream inputFile;
    string aLine;
	vector<string> result;
	cout << name << " ... ";
	inputFile.open(name.c_str());
    while (inputFile) {
    	getline (inputFile, aLine);
    	if (aLine == "") {
    		continue;
		}
    	result.push_back(aLine);
	}
	inputFile.close();	
	cout << "done!" << endl;
	return result;
}

/* Get data from a string with seperator (a/b/c -> a b c)*/
vector<string> ProcessString(string input, string splitby) {
	size_t pos = 0;
	string element;
	vector<string> result;
	
	while ((pos = input.find(splitby)) != string::npos) {
		element = input.substr(0, pos);	
    	result.push_back (element);
		input.erase(0, pos + splitby.length());
	}
	
	//Get result
	result.push_back (input);
	return (result);
}

/* Convert a string to integer using sstream */
int ConvertStringToInteger(string s) {
	int x;
	stringstream ss;
	ss.clear();
	ss.str(s);
	ss >> x;
	return x;
}

/* Generate 2d string array */
void GenerateStringMap(string** arr, int width, int height) {
	for (int i = 0; i < height; i++) {
        arr[i] = new string[width];	
    }
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            arr[i][j] = " ";
        }
    }
}

/* Generate 2d int array */
void GenerateIntegerMap(int** arr, int width, int height) {
	for (int i = 0; i < height; i++) {
        arr[i] = new int[width];
    }
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            arr[i][j] = 0;
        }
    }
}

/* Get coordinate data inside bracket ([3, 4] -> 3 4) */
string ProcessCoordinate(string str) {
	char chars[] = "[],";
   	for (unsigned int i = 0; i < sizeof(chars); ++i) {
      str.erase (std::remove(str.begin(), str.end(), chars[i]), str.end());
   	} 	
   	return str;
}

/* Print 2d map to screen */
void Render(string** arr, int width, int height) {
	string border = " ";
	for(int i=0; i< width + 2; i++) {
		border += "#  ";
	}
	
	cout <<  " " << "  " << border << endl;
	
	for (int i = 0; i < height; i++) {
		cout<< setw(2) << height - i - 1 << "  " << "#" << "  ";
        for (int j = 0; j < width; j++) {
            cout << arr[i][j] << "  ";
        }
        cout << "#";
        cout << endl;
    }
    cout << " " << "  " << border << endl;
    cout << setw(6) << " ";
    for(int i=0; i< width; i++) {
    	cout << setw(2) << i << " ";
	}
	cout << "\n" << endl;
}

/* Freeze the screen until press enter */
void Freeze() {
	string chh = "1";
	cout << "Press <enter> to go back to main menu ..." << endl;
	cin.ignore();
	do {
		getline(cin, chh);
	} while(chh.length() != 0);
}

/* Reclaim memory */
void DeleteStringMap(string** arr, int height) {
    for(int i=0;i<height;i++) {
		delete [] arr[i];    	
	}
	delete [] arr;
}

void DeleteIntMap(int** arr, int height) {
    for(int i=0;i<height;i++) {
		delete [] arr[i];   
	}
	delete [] arr;
}

/* Check neighbor to see which city it belongs to */
string SurroundCheck(string **map, int x, int y, int tableWidth, int tableHeight) {
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) {
                continue;  // Skip the current position
            }

            int newX = x + dx;
            int newY = y + dy;

            if (newX >= 0 && newX < tableWidth && newY >= 0 && newY < tableHeight) {
                const std::string& value = map[newX][newY];
                if (value != " ") {
                    return value;
                }
            }
        }
    }

    return "";
}

/* Convert data to LMH */
string ConvertToLMH(int val) {
	string value;
	switch(val) {
		case 0 ... 34:
			value = "L";
			break;
		case 35 ... 64:
			value = "M";
			break;
		default:
			value = "H";
			break;
	}	
	return value;
}

string ConvertToIndex(string val) {
	string value;
	if (TrimSpace(val).size() == 1) {
		value = "0";
	} else {
		value = val.substr(0, 1);
	}
	return value;
}

/* Sleep function that can run on both window and unix */
void MySleep(int amount) {
	#ifdef WIN32
	Sleep(amount * 1000);
	#define window
	#else
	sleep(amount);
	#define unix
	#endif
}

/* Clear screen function that can run on both window and unix */
void ClearScreen() {
	#ifdef WIN32
	system("cls");
	#define window
	#else
	system("clear");
	#define unix
	#endif
}
