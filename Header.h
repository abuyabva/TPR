#pragma once
#include <iostream>
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

void inputTable(vector <Record>& table);
void outputTable(vector <Record> table);
void Pareto(vector <Record> table);
void outputNewParetoTable(vector <Record> table, vector <vector <string>> newParetoTable);
void addInPareto(Record num, vector <Record>& paretoBestChoises);
void constriction1(vector <Record> table);// нижние\верхние + приоритетный
void constriction2(vector <Record> table);// указание верхних\нижних границ
void constriction3(vector <Record> table);// лексикографическая