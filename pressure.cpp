#include "pressure.h"
#include "algo.h"

void Pressure(int height, int width, vector<string>& raw_pressure, 
string**& pressure_map, string**& pressure_converted, int**& pressure_orginal) {
    vector <string> v;
    string s, value;
    vector <string> coordinate;
    int x,y;

	//Extracting data
    for (int i=0; i<raw_pressure.size(); i++) {
		v = ProcessString(raw_pressure[i], "-");
		s = ProcessCoordinate(v[0]);
		value = v[1].substr(0,1);
		coordinate = ProcessString(s, " ");
		x = ConvertStringToInteger(coordinate[0]);
		y = ConvertStringToInteger(coordinate[1]);

		//Assign value
		if((x < width) && (y < height)) {
			pressure_map[height - y - 1][x] = value;	
			pressure_orginal[height - y - 1][x] = ConvertStringToInteger(v[1]);
		}		
		
		if((x < width) && (y < height)) {
			pressure_converted[height - y - 1][x] = ConvertToLMH(ConvertStringToInteger(v[1]));	
		}		
	}
}

