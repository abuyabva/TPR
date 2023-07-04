#pragma once
#include <iostream>
#include <Windows.h>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <Windows.h>

using namespace std;

struct Node {
	double cost;
	bool basis;
	double rate = 0;
	double waste = 0;
};

struct Min {
	double val;
	int x;
	int y;
};

struct Vertex {
	double content;
	double rate;
};

void outputCostTable(vector <vector <Node>> costTable);
double north_west(vector <vector <Node>>& costTable,
	vector <Vertex> providers, vector <Vertex> consumers);
double minCostMethod(vector <vector <Node>> costTable,
	vector <Vertex> providers, vector <Vertex> consumers);
double findMin(vector <vector <Node>> costTable);
double eraseTable(vector <vector <Node>>& costTable,
	vector <Vertex>& providers, vector <Vertex>& consumers);
void findPotential(vector <vector <Node>>& costTable,
	vector <Vertex>& providers, vector <Vertex>& consumers);
vector <vector <int>> buildBorder(vector <vector <Node>> costTable,
	vector <int> step);
int basisNum(vector <vector <Node>> costTable,
	vector <vector <int>> borders);
vector <vector <int>> buildCycle(vector <vector <int>> allVertexes,
	vector <int> start, vector <vector <int>> cycle, bool& end);
bool potentialCheck(vector <vector <Node>> costTable,
	vector <Vertex> providers, vector <Vertex> consumers);