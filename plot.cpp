#include "plot.h"
#include "algo.h"

string chart_input = "";

vector <double> stat;
vector <int> count_entry = {0, 0, 0, 0, 0};
vector <string> entry_range = {"0 - 20", "20 - 40", "40 - 60", "60 - 80", "80 - 100"};

bool median = false;

string center(const City_report cr, const int w);
string CenterNumber(const double stat, const int w);
string CenterInt(const int stat, const int w);

void HandleSorting(string input, vector<City_report> &alist);
void ToGraph(vector<City_report> &alist, string input);
void ToHistogram();
void DataGrouping(double entry);
void Summary(vector<City_report> &alist, string input);

double Scale(double data);

bool Compare_cloud(const City_report &a, const City_report &b);
bool Compare_pressure(const City_report &a, const City_report &b);
bool Compare_rain(const City_report &a, const City_report &b);

//Menu
void Plot(vector<City_report> alist) {
  cout << "Welcome to Virtual Solution's weather data analyzer!" << endl;
  cout << "1. Ave. Cloud Cover (ACC)" << endl;
  cout << "2. Ave. Pressure (AP)" << endl;
  cout << "3. Probability of Rain" << endl;
  cout << "Your option: ";
  cin >> chart_input;

  //Read user input and process data
  if (chart_input == "1") {
    HandleSorting(chart_input, alist);
    for(int i=0; i<alist.size(); i++){
      stat.push_back(Scale(alist[i].ave_cloud));
      DataGrouping(alist[i].ave_cloud);
    }
  } else if (chart_input == "2") {
    HandleSorting(chart_input, alist);
    for(int i=0; i<alist.size(); i++){
      stat.push_back(Scale(alist[i].ave_pressure));
      DataGrouping(alist[i].ave_pressure);
    }
  } else if (chart_input == "3") {
    HandleSorting(chart_input, alist);
    for(int i=0; i<alist.size(); i++){
      stat.push_back(Scale(alist[i].rain));
      DataGrouping(alist[i].rain);
    }
  } else {
    cout << "Invalid option!" << endl;
    MySleep(1);
    return;
  }

  ToGraph(alist, chart_input);
  Summary(alist, chart_input);
  ToHistogram();
  stat.clear();
  for(int i=0; i<count_entry.size(); i++){
    count_entry[i] = 0;
  }
}

/* Sorting based on user input */
void HandleSorting(string input, vector<City_report> &alist) {
  bool valid_sort = false;
  string sorting_input = "";

  do{
    ClearScreen();
    cout << "Specify sorting option" << endl;
    cout << "1. By city id asc" << endl;
    cout << "2. By city id dsc" << endl;
    cout << "3. By value asc" << endl;
    cout << "4. By value dsc" << endl;
    cout << "Your option: ";
    cin >> sorting_input;
    sorting_input = TrimSpace(sorting_input);

    if (sorting_input == "1") {
      reverse(alist.begin(), alist.end());
      valid_sort = true;
    }

    if (sorting_input == "2") {
      valid_sort = true;
    }

    if (sorting_input == "3") {
      if (input == "1") {
        sort(alist.begin(), alist.end(), Compare_cloud);
      } else if (input == "2") {
        sort(alist.begin(), alist.end(), Compare_pressure);
      } else {
        sort(alist.begin(), alist.end(), Compare_rain);
      }
      valid_sort = true;
      median = true;
    }

    if (sorting_input == "4") {
      if (input == "1") {
        sort(alist.begin(), alist.end(), Compare_cloud);
        reverse(alist.begin(), alist.end());
      } else if (input == "2") {
        sort(alist.begin(), alist.end(), Compare_pressure);
        reverse(alist.begin(), alist.end());
      } else {
        sort(alist.begin(), alist.end(), Compare_rain);
        reverse(alist.begin(), alist.end());
      }
      valid_sort = true;
      median = true;
    }
  } while (!valid_sort);

}

/* Draw bar graph */
void ToGraph(vector<City_report> &alist, string input) {
  ClearScreen();
  if(input == "1"){
    cout << "[Analyzing average Cloud Cover]" << endl;
  } else if (input == "2"){
    cout << "[Analyzing average Pressure]" << endl;
  } else {
    cout << "[Analyzing probability of rain]" << endl;
  }

  cout << endl;
  cout << "      ^ " << endl;

  const int chart_height = 25;
  for (int current_height = chart_height; current_height > 0; --current_height) {
    if (!(current_height%5)){
      cout << setw(5) << current_height * 4 << "\u001b[31;1m | \033[0m"; // Bright red color
    } else {
      cout << "\u001b[31;1m      | \033[0m"; // Bright red color
    }

		for (int i=0; i< stat.size(); i++) {
			int bar_height = floor(stat[i]) + 1;
      if (bar_height + 1 == current_height){
        if(input == "1"){
          cout << CenterNumber(alist[i].ave_cloud, 12);
        } else if(input == "2"){
          cout << CenterNumber(alist[i].ave_pressure, 12);
        }else{
          cout << CenterNumber(alist[i].rain, 12);
        }
      } else if (bar_height < current_height){
        cout << "           ";
      } else if (bar_height == current_height){
        double stat_convert = stat[i] * 10;
        int decimal;
        if(stat_convert - floor(stat_convert) > 0.5) {
          decimal = ceil(stat_convert);
        } else {
          decimal = floor(stat_convert);
        }
        //int decimal = floor(stat_convert);
        decimal = decimal % 10;
        if(decimal == 0){
            cout << "\033[1;31m  _______  \033[0m"; // Red color
        } else if (decimal == 1) {
            cout << "\033[1;32m  .......  \033[0m"; // Green color
        } else if (decimal == 2) {
            cout << "\033[1;33m  -------  \033[0m"; // Yellow color
        } else if (decimal == 3) {
            cout << "\033[1;35m  ~~~~~~~  \033[0m"; // Purple color
        } else if (decimal == 4) {
            cout << "\033[1;36m  =======  \033[0m"; // Cyan color
        } else if (decimal == 5) {
            cout << "\033[1;36m  :::::::  \033[0m"; // Cyan color
        } else if (decimal == 6) {
            cout << "\033[1;35m  >>>>>>>  \033[0m"; // Purple color
        } else if (decimal == 7) {
            cout << "\033[1;33m  ^^^^^^^  \033[0m"; // Yellow color
        } else if (decimal == 8) {
            cout << "\033[1;32m  '''''''  \033[0m"; // Green color
        } else{
          cout << "  |||||||  ";
        }
      }else {
          cout << "\033[1;34m |       | \033[0m";
      }
		} 
			cout << endl;
	}
  cout << "      +-";
	for (int i = 0; i < stat.size(); ++i) {
		cout << "\033[1;31m-----v-----\033[0m";
	}
	cout << "-->" << endl;

  cout << "         ";

	for (int i = 0; i < alist.size(); ++i) {
		cout << center(alist[i], 9) << "  ";
	}
  
  cout << endl;
}

/* Draw histogram */
void ToHistogram() {
    cout << endl;
    cout << endl;
    cout << "[Data distribution]" << endl;
    cout << endl;
    cout << "      ^ " << endl;

    int max_count = *std::max_element(begin(count_entry), end(count_entry));
    int chart_height = 5 * max_count;
    for (int current_height = chart_height; current_height > 0; --current_height) {
        if (!(current_height % 5)) {
            cout << setw(5) << (current_height * max_count) / chart_height << " | ";
        } else {
            cout << "      | ";
        }

        for (int i = 0; i < count_entry.size(); i++) {
            int bar_height = floor(count_entry[i]) + 1;
            if (bar_height + 1 == current_height) {
                cout << "\033[1;31m" << CenterInt(count_entry[i], 12) << "\033[0m"; // Red color
            } else if (bar_height < current_height) {
                cout << "           ";
            } else if (bar_height == current_height) {
                cout << "\033[1;32m  _______  \033[0m"; // Green color
            } else {
                cout << "\033[1;34m |       | \033[0m"; // Blue color
            }
        }
        cout << endl;
    }
    cout << "      +-";
    for (int i = 0; i < count_entry.size(); ++i) {
        cout << "-----v-----";
    }
    cout << "-->" << endl;
    cout << "           ";
    cout << "\033[1;36m0 - 20    20 - 40    40 - 60    60 - 80    80 - 100\033[0m" << endl; // Cyan color
    cout << endl;
    auto it = std::minmax_element(count_entry.begin(), count_entry.end());
    int max_idx = std::distance(count_entry.begin(), it.second);

    cout << "Most frequent data range: " << "(" << entry_range[max_idx] << ")" << endl;
    cout << "Frequency: " << count_entry[max_idx] << endl;
    cout << endl;
}

/* Generate data summary */
void Summary(vector<City_report> &alist, string input) {
  double min_val = 0;
  string min_name = "";
  double max_val = 0;
  string max_name = "";
  double sum_val = 0;
  int mid_pos = (alist.size() + 1) / 2;
  double median_val = 0.0;
  min_name = alist[0].name;
  max_name = alist[alist.size() - 1].name;
  
  if (input == "1") {
    sort(alist.begin(), alist.end(), Compare_cloud);
    min_val = alist[0].ave_cloud;
    max_val = alist[alist.size() - 1].ave_cloud;
    median_val = alist[mid_pos].ave_cloud;
    for(int i=0; i<alist.size(); i++){
      sum_val += alist[i].ave_cloud;
    }
  } else if (input == "2") {
    sort(alist.begin(), alist.end(), Compare_pressure);
    min_val = alist[0].ave_pressure;
    max_val = alist[alist.size() - 1].ave_pressure;
    median_val = alist[mid_pos].ave_pressure;
    for(int i=0; i<alist.size(); i++){
      sum_val += alist[i].ave_pressure;
    }
  } else {
    sort(alist.begin(), alist.end(), Compare_rain);
    min_val = alist[0].rain;
    max_val = alist[alist.size() - 1].rain;
    median_val = alist[mid_pos].rain;
    for(int i=0; i<alist.size(); i++){
      sum_val += alist[i].rain;
    }
  }
  cout << endl;
  cout << endl;
  cout << "==========================================" << endl;
  cout << "  [Weather data statistics]" << endl;
  cout << "  Total records: " << alist.size() << endl;
  cout << "  Biggest entry: " << setprecision(2) << fixed << max_val << " (" << max_name << ")" << endl;
  cout << "  Smallest entry: " << setprecision(2) << fixed << min_val << " (" << min_name << ")" << endl;
  if(median) cout << "  Median entry: " << setprecision(2) << fixed << median_val << endl;
  cout << "  Average: " << setprecision(2) << fixed << sum_val / alist.size() << endl;
  cout << "==========================================" << endl;
  cout << endl;
}

//Convert data to range
void DataGrouping(double entry) {
  if(entry >= 0.0 && entry < 20.0) {
    count_entry[0]++;
  } else if (entry >= 20.0 && entry < 40.0) {
    count_entry[1]++;
  } else if (entry >= 40.0 && entry < 60.0) {
    count_entry[2]++;
  } else if (entry >= 60.0 && entry < 80.0) {
    count_entry[3]++;
  } else {
    count_entry[4]++;
  }
}

//Value scale (for bar chart)
double Scale(double data) {
  return data / 4.0;
}

//Compare by average cloud cover
bool Compare_cloud(const City_report &a, const City_report &b) {
  return a.ave_cloud < b.ave_cloud;
}

//Compare by average pressure
bool Compare_pressure(const City_report &a, const City_report &b) {
  return a.ave_pressure < b.ave_pressure;
}

//Compare by probability of rain
bool Compare_rain(const City_report &a, const City_report &b) {
  return a.rain < b.rain;
}

//Center to axis (city name, double value and int value)
string center(const City_report cr, const int w) {
    std::stringstream ss, spaces;
    string s;
    if(cr.name.size() > w){
      s = "ID" + cr.id;
    } else {
      s = cr.name;
    }
    int pad = w - s.size();                
    for(int i=0; i<pad/2; ++i)
        spaces << " ";
    if(pad>0 && pad%2!=0)                  
        ss << " ";
    ss << spaces.str() << s << spaces.str(); 
    return ss.str();
}

string CenterNumber(const double stat, const int w) {
    std::stringstream ss, spaces;
    string s;
    std::stringstream stream;
    
    stream << std::fixed << std::setprecision(2) << stat;
    s = stream.str();

    int pad = w - s.size();      
    for(int i=0; i<pad/2; ++i)
        spaces << " ";
    ss << spaces.str() << s << spaces.str();
    return ss.str();
}

string CenterInt(const int stat, const int w) {
    std::stringstream ss, spaces;
    string s;
    std::stringstream stream;
    
    stream << stat;
    s = stream.str();

    int pad = w - s.size();      
    for(int i=0; i<pad/2; ++i)
        spaces << " ";
    ss << spaces.str() << s << spaces.str();
    return ss.str();
}
