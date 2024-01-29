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
				city_size[id]++;
				cloud_sum[id] += cloud_original[i][j];
				pressure_sum[id] += pressure_original[i][j];
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
struct LevelAssignments {
    std::string low;
    std::string medium;
    std::string high;
};

void Assign(City_report &r) {
    auto getLevel = [](double value, const LevelAssignments &levels) -> std::string {
        if (value <= 34) {
            return levels.low;
        } else if (value <= 64) {
            return levels.medium;
        } else {
            return levels.high;
        }
    };

    LevelAssignments cloudLevels{"L", "M", "H"};
    LevelAssignments pressureLevels{"L", "M", "H"};

    r.cloud_lv = getLevel(r.ave_cloud, cloudLevels);
    r.pres_lv = getLevel(r.ave_pressure, pressureLevels);

    // Lookup table for rain values and symbols
    std::unordered_map<std::string, double> rainValues{
        {"LL", 70.0}, {"LM", 80.0}, {"LH", 90.0},
        {"ML", 40.0}, {"MM", 50.0}, {"MH", 60.0},
        {"HL", 10.0}, {"HM", 20.0}, {"HH", 30.0}
    };

    std::unordered_map<std::string, std::string> symbolValues{
        {"LL", "~~~~\n~~~~~\n  \\\\\\"}, {"LM", "~~~~\n~~~~~\n \\\\\\\\"}, {"LH", "~~~~\n~~~~~\n\\\\\\\\\\"},
        {"ML", "~~~~\n~~~~~\n"}, {"MM", "~~~~\n~~~~~\n    \\"}, {"MH", "~~~~\n~~~~~\n   \\\\"},
        {"HL", "~\n~~\n"}, {"HM", "~~\n~~~\n"}, {"HH", "~~~\n~~~~\n"}
    };

    // Combine levels to get the key for lookup
    std::string levelKey = r.pres_lv + r.cloud_lv;

    // Assign rain and symbol based on lookup
    r.rain = rainValues[levelKey];
    r.symbol = symbolValues[levelKey];
}