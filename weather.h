#ifndef WEATHER_H
#define WEATHER_H
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;
struct City_report {
	string name;
	string id;
	double ave_cloud;
	string cloud_lv;
	double ave_pressure;
	string pres_lv;
	double rain;
	string symbol;	
};

//City_report *report_list;
void Weather(vector <City_report> alist);
void ComputeWeatherData(int height, int width, vector<string> &raw_name, vector<string> &raw_id,string**& city_map, string**& cloud_map, string**& pressure_map, int**& pressure_original, int**& cloud_original, vector<City_report>&alist);
string ToString(City_report r);
#endif