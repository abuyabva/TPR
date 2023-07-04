#pragma once
#include <iostream>
#include <algorithm>
#include <Windows.h>
#include <vector>
#include <iomanip>

using namespace std;

struct Record {
	string name;
	double rating; // +  weight: 4
	double loanRate; // -  weight: 5
	int positiveReviewsCount; // +  weight: 3
	double loanTerm; // -  weight: 2
};

class ElectraGraph {
public:
	int bankNumber;
	vector <int> dominated;
	ElectraGraph() { bankNumber = NULL; }
	vector <ElectraGraph> inputGraph(vector <vector <double>> ElectraTable);
	void outputGraph(vector <ElectraGraph> g);
	bool findLoops(vector <ElectraGraph> g, vector <int> path);
	void findBest(vector <Record> table, vector <ElectraGraph> g);
	bool findInAll(vector <int>& allVertexes, int el);
	bool findInPath(vector <int> path, int el);
	void deleteLoops(vector <ElectraGraph>& g, vector <vector <double>>& ElectraTable);
	double findMinDom(vector <vector <double>> ElectraTable);
};

void inputTable(vector <Record>& table);
void outputTable(vector <Record> table);
void Electra(vector <Record> table, ElectraGraph& graph);
void outputElectraTable(vector <Record> table, vector <vector<double>> ElectraTable);