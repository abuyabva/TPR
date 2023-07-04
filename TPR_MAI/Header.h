#pragma once
#include <iostream>
#include <vector>
#include <Windows.h>
#include <cmath>
#include <iomanip>

using namespace std;

struct Record {
	string name;
	double rating; // +  weight: 7
	double loanRate; // -  weight: 9
	int positiveReviewsCount; // +  weight: 5
	double loanTerm; // -  weight: 3
};

double geom(vector <double> v);
double geomSum(vector <vector <double>> v, vector <double>& w);
void input(vector <vector <double>>& v);
void criterion(vector <vector <double>> v, vector <double>& w);
void inputTable(vector <Record>& table);
void outputTable(vector <Record> table);
void MAI(vector <Record> table, vector <double> w);
void compareAltTable(vector <Record> table, vector <vector <double>>& altTable, vector <vector <double>>& W);
void insertColumn(vector <vector <double>>& S, vector <vector <double>> altTable);
bool isMatrixConformed(vector <double> w, vector <double> s, double n);