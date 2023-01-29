#include "weather.h"
#include "algo.h"
string ToString(City_report r);
int *city_size;
int *cloud_sum;
int *pressure_sum;
int rp_size, id;
City_report *report_list;
void Assign(City_report &r);

//Print weather forecast report
void Weather(vector <City_report> alist) {
	for(int i=0; i<rp_size; i++) {
		cout << ToString(alist[i]) << endl;
	}
	
}

//Handle weather report data
void ComputeWeatherData(int height, int width, vector<string> &raw_name, vector<string> &raw_id, 
string**& city_map, string**& cloud_map, string**& pressure_map, int**& pressure_original, int**& cloud_original, vector <City_report> &alist) {
    string city_belong;

	rp_size = raw_name.size();
	report_list = new City_report[rp_size];
	cloud_sum = new int[rp_size];
	pressure_sum = new int[rp_size];
	city_size = new int[rp_size];
	
	for(int i = 0; i< rp_size; i++) {
		report_list[i].name = raw_name[i];
		report_list[i].id = raw_id[i];
		cloud_sum[i] = 0;
		pressure_sum[i] = 0;
		city_size[i] = 0;
	}
	
	for(int i=0; i<height; i++) {
		for(int j=0; j<width; j++) {
			city_belong = SurroundCheck(city_map, i, j, height, width);
			if(city_belong != "") {
				id = ConvertStringToInteger(city_belong);
				id --;
				city_size[rp_size - id - 1]++;
				cloud_sum[rp_size - id - 1] += cloud_original[i][j];
				pressure_sum[rp_size - id - 1] += pressure_original[i][j];
			}
		}
	}
	
	for(int i = 0; i< rp_size; i++) {
		report_list[i].ave_cloud = cloud_sum[i] / static_cast<double>(city_size[i]);
		report_list[i].ave_pressure = pressure_sum[i] / static_cast<double>(city_size[i]);	
	}

	for(int i = 0; i< rp_size; i++) {
		Assign(report_list[i]);
		alist.push_back(report_list[i]);
	}

	delete[] report_list;
	delete[] cloud_sum;
	delete[] pressure_sum;
	delete[] city_size;
}

/* Generate report */
string ToString(City_report r) {

	ostringstream oss;
	string value;
	oss << "City Name : " << r.name << endl;
	oss << "City ID   : " << r.id << endl;
	oss << "Ave. Cloud Cover (ACC)	: " << setprecision(2) << fixed << r.ave_cloud << " (" << r.cloud_lv <<")" << endl;
	oss << "Ave. Pressure (AP)	: " << setprecision(2) << fixed << r.ave_pressure << " (" << r.pres_lv <<")" << endl;
	oss << "Probability of rain (%)	: " << setprecision(2) << fixed << r.rain << endl;
	oss << r.symbol << endl;
	return (oss.str());
}


//Convert average value to string format
void Assign(City_report &r){

	switch(static_cast<int>(r.ave_cloud)) {
		case 0 ... 34:
			r.cloud_lv = "L";
			break;
		case 35 ... 64:
			r.cloud_lv = "M";
			break;
		default:
			r.cloud_lv = "H";
			break;
	}

	switch(static_cast<int>(r.ave_pressure)) {
		case 0 ... 34:
			r.pres_lv = "L";
			break;
		case 35 ... 64:
			r.pres_lv = "M";
			break;
		default:
			r.pres_lv = "H";
			break;
	}

	if(r.pres_lv == "L") {
		if(r.cloud_lv == "L") {
			r.rain = 70.0;
			r.symbol = "~~~~\n~~~~~\n  \\\\\\";
		} else if(r.cloud_lv == "M") {
			r.rain = 80.0;
			r.symbol = "~~~~\n~~~~~\n \\\\\\\\";
		} else {
			r.rain = 90.0;
			r.symbol = "~~~~\n~~~~~\n\\\\\\\\\\";
		}
	} else if(r.pres_lv == "M") {
		if(r.cloud_lv == "L") {
			r.rain = 40.0;
			r.symbol = "~~~~\n~~~~~\n";
		} else if(r.cloud_lv == "M") {
			r.rain = 50.0;
			r.symbol = "~~~~\n~~~~~\n    \\";
		} else {
			r.rain = 60.0;
			r.symbol = "~~~~\n~~~~~\n   \\\\";
		}
	} else{
		if(r.cloud_lv == "L") {
			r.rain = 10.0;
			r.symbol = "~\n~~\n";
		} else if(r.cloud_lv == "M") {
			r.rain = 20.0;
			r.symbol = "~~\n~~~\n";
		} else {
			r.rain = 30.0;
			r.symbol = "~~~\n~~~~\n";
		}
	}
}