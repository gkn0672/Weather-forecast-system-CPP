#include "cloud.h"
#include "algo.h"

void Cloud(int height, int width, vector<string>& raw_cloud, 
string**& cloud_map, string**& cloud_converted, int**& cloud_original) {
    vector <string> v;
    string s, value;
    vector <string> coordinate;
    int x,y;
	
	//Extract data
	for (int i=0; i<raw_cloud.size(); i++) {
		v = ProcessString(raw_cloud[i], "-");
		s = ProcessCoordinate(v[0]);
		coordinate = ProcessString(s, " ");
		x = ConvertStringToInteger(coordinate[0]);
		y = ConvertStringToInteger(coordinate[1]);

		//Assign value
		if((x < width) && (y < height)) {
			cloud_map[height - y - 1][x] = ConvertToIndex(v[1]);
			cloud_original[height - y - 1][x] = ConvertStringToInteger(v[1]);
		}

		if((x < width) && (y < height)) {
			cloud_converted[height - y - 1][x] = ConvertToLMH(ConvertStringToInteger(v[1]));	
		}
	}
}
