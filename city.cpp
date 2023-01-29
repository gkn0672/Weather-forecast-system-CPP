#include "city.h"
#include "algo.h"

void City(int height, int width, vector<string>& raw_city, 
string**& city_map, vector<string> &raw_name, vector<string> &raw_id) {   

    string value;
    string name;
    string s;
    vector <string> coordinate;
    vector <string> v;
    int x,y;
    
    //Clear vector before compute (handle re-read configuration file)
    raw_name.clear();
    raw_id.clear();
    
	//Extract data
	for (int i=0; i<raw_city.size(); i++) {
		v = ProcessString(raw_city[i], "-");
		s = ProcessCoordinate(v[0]);
		value = v[1];
		name = v[2];
		
		//Assign value
		if(!count(raw_name.begin(), raw_name.end(), name)) {
			raw_name.push_back(name);
			raw_id.push_back(value);
		}
		
		coordinate = ProcessString(s, " ");
		x = ConvertStringToInteger(coordinate[0]);
		y = ConvertStringToInteger(coordinate[1]);
		
		//Rotate map 
		if((x < width) && (y < height)) {
			city_map[height - y - 1][x] = value;	
		}		
	}
}
