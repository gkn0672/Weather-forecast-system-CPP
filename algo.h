#ifndef ALGO_H
#define ALGO_H
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <random>
using namespace std;

vector <string> ReadFile(string name);
vector<string> ProcessString(string input, string splitby);

string ProcessCoordinate(string str);
string TrimSpace(string str);
string SurroundCheck(string **map, int x, int y, int tableWidth, int tableHeight);
string ConvertToLMH(int val);

int ConvertStringToInteger(string s);
int GetRandomNumber(int min, int max);

void GenerateStringMap(string** arr, int width, int height);
void GenerateIntegerMap(int** arr, int width, int height);
void Render(string** arr, int width, int height);
void Freeze();
void DeleteStringMap(string** arr, int height);
void DeleteIntMap(int** arr, int height);
void MySleep(int amount);
void ClearScreen();
#endif