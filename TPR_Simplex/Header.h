#pragma once
#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cmath>
using namespace std;

struct Coefficient {
	string x;
	int coefficient;
};

vector <Coefficient> C;

struct SimplexTableString {
	string x;
	double first;
	double second;
	double third;
};

struct SimplexTable {
	vector <string> high;
	vector <SimplexTableString> content;
	vector <double> Cb;
};

double rectangleRule(double aij, double ais, double arj, double ars);
void buildSimplexTable();
double findRelativeMark(vector <double> cb, vector <double> A, double c);
void find_f_string(SimplexTable& t);
double getByX(string key);
void outputSimplexTable(SimplexTable t);
bool check_f_string(SimplexTableString f);
string findMinInF(SimplexTable t);
string eraseFromBasis(SimplexTable t, int x);
void buildNewSimplexTable(SimplexTable t, SimplexTable p, double a);
void getCb(SimplexTable& t);