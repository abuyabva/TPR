#include "Header.h"

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	vector <Record> table;
	ElectraGraph graph = ElectraGraph();
	inputTable(table);
	cout << "\nЗаклдированная таблица:\n\n";
	outputTable(table);
	Electra(table, graph);
}

void Electra(vector <Record> table, ElectraGraph& graph) {
	vector <vector <double>> ElectraTable;
	ElectraTable.resize(10);
	for (int i = 0; i < 10; i++) {
		ElectraTable[i].resize(10);
	}
	for (int i = 0; i < table.size(); i++) {
		for (int j = 0; j < table.size(); j++) {
			double Pij = 0, Nij = 0, Pji = 0, Nji = 0, Dij, Dji;
			if (i != j) {
				if (table[i].rating > table[j].rating) {
					Pij += 4;
					Nji += 4;
				}
				else if (table[i].rating < table[j].rating) {
					Pji += 4;
					Nij += 4;
				}
				if (table[i].loanRate < table[j].loanRate) {
					Pij += 5;
					Nji += 5;
				}
				else if (table[i].loanRate > table[j].loanRate) {
					Pji += 5;
					Nij += 5;
				}
				if (table[i].positiveReviewsCount > table[j].positiveReviewsCount) {
					Pij += 3;
					Nji += 3;
				}
				else if (table[i].positiveReviewsCount < table[j].positiveReviewsCount) {
					Pji += 3;
					Nij += 3;
				}
				if (table[i].loanTerm < table[j].loanTerm) {
					Pij += 2;
					Nji += 2;
				}
				else if (table[i].loanTerm > table[j].loanTerm) {
					Pji += 2;
					Nij += 2;
				}
				Dij = round(Pij / Nij * 10) / 10;
				Dji = round(Pji / Nji * 10) / 10;
				if (Dij > 1) {
					
					ElectraTable[i][j] = Dij;
				}
				if (Dji > 1) {
					
					ElectraTable[j][i] = Dji;
				}
			}

		}
	}
	outputElectraTable(table, ElectraTable);
	vector <ElectraGraph> g = graph.inputGraph(ElectraTable);
	vector <int> path;
	cout << "\nИзначальный граф:\n";
	graph.outputGraph(g);
	while (graph.findLoops(g, path)) {
		graph.deleteLoops(g, ElectraTable);
	}
	cout << "\nПолучившийся граф после удаления петель:\n";
	graph.outputGraph(g);
	cout << "\nЛучший банк по методу Электры:\n";
	graph.findBest(table, g);
}

void ElectraGraph::findBest(vector <Record> table, vector <ElectraGraph> g) {
	vector <int> allVertexes;
	for (int i = 0; i < g.size(); i++) {
		allVertexes.push_back(g[i].bankNumber);
	}
	for (int i = 0; i < g.size(); i++) {
		for (int j = 0; j < g[i].dominated.size(); j++) {
			findInAll(allVertexes, g[i].dominated[j]);
		}
	}
	for (int i = 0; i < allVertexes.size(); i++) {
		cout << table[allVertexes[i]].name << " (" << allVertexes[i] << ")" << '\n';
	}
}

bool ElectraGraph::findInAll(vector <int>& allVertexes, int el) {
	for (int i = 0; i < allVertexes.size(); i++) {
		if (el == allVertexes[i]) {
			allVertexes.erase(allVertexes.begin() + i);
			return true;
		}
	}
	return false;
}

bool ElectraGraph::findLoops(vector <ElectraGraph> g, vector <int> path) {
	if (path.size() == 0) {
		for (int i = 0; i < g.size(); i++) {
			path.push_back(i);
			for (int j = 0; j < g[i].dominated.size(); j++) {
				if (findLoops(g, path)) {
					return true;
				}
			}
			path.clear();
		}
	}
	else {
		for (int j = 0; j < g[path[path.size() - 1]].dominated.size(); j++) {
			if (findInPath(path, g[path[path.size() - 1]].dominated[j])) {
				return true;
			}
			else {
				path.push_back(g[path[path.size() - 1]].dominated[j]);
				if (findLoops(g, path)) {
					return true;
				}
				path.erase(path.begin() + path.size() - 1);
			}
		}
	}
	return false;
}

bool ElectraGraph::findInPath(vector <int> path, int el) {
	for (int i = 0; i < path.size(); i++) {
		if (path[i] == el) {
			return true;
		}
	}
	return false;
}

void ElectraGraph::deleteLoops(vector <ElectraGraph>& g, vector <vector <double>>& ElectraTable) {
	double minDom = this->findMinDom(ElectraTable);
	for (int i = 0; i < ElectraTable.size(); i++) {
		for (int j = 0; j < ElectraTable[i].size(); j++) {
			if (ElectraTable[i][j] == minDom) {
				ElectraTable[i][j] = NULL;
			}
		}
	}
	g = this->inputGraph(ElectraTable);
}

double ElectraGraph::findMinDom(vector <vector <double>> ElectraTable) {
	double min = 0;
	for (int i = 0; i < ElectraTable.size(); i++) {
		for (int j = 0; j < ElectraTable[i].size(); j++) {
			if (ElectraTable[i][j] != NULL) {
				if ((ElectraTable[i][j] < min) || (min == 0)) {
					min = ElectraTable[i][j];
				}
			}
		}
	}
	return min;
}

vector <ElectraGraph> ElectraGraph::inputGraph(vector <vector <double>> ElectraTable) {
	vector <ElectraGraph> g;
	ElectraGraph x;
	for (int i = 0; i < ElectraTable.size(); i++) {
		x.bankNumber = i;
		for (int j = 0; j < ElectraTable[i].size(); j++) {
			if (ElectraTable[i][j] > 0) {
				x.dominated.push_back(j);
			}
		}
		g.push_back(x);
		x.dominated.clear();
	}
	return g;
}

void ElectraGraph::outputGraph(vector <ElectraGraph> g) {
	for (int i = 0; i < g.size(); i++) {
		cout << "Банк номер " << i << " доминирует над: ";
		for (int j = 0; j < g[i].dominated.size(); j++) {
			cout << g[i].dominated[j] << " ";
		}
		cout << '\n';
	}
}

void outputElectraTable(vector <Record> table, vector <vector<double>> ElectraTable) {
	cout << "\nПрименение метода Электры:\n\n";
	cout << setw(21) << "Название банка|\n";
	cout << "-------------------------------------------------------------------------------|\n";
	for (int i = 0; i < table.size(); i++) {
		cout << setw(16) << table[i].name << "(" << i << ")|";
		for (int j = 0; j < ElectraTable[i].size(); j++) {
			if (i == j) {
				cout << setw(5) << "x" << "|";
				continue;
			}
			else if (ElectraTable[i][j] == 0) {
				cout << setw(5) << "-" << "|";
				continue;
			}
			cout << setw(5) << ElectraTable[i][j] << "|";
		}
		cout << "\n-------------------------------------------------------------------------------|\n";
	}
}

void outputTable(vector <Record> table) {
	cout << setw(16) << "Название банка|" << setw(21) << "Рэйтинг(+) вес: 4|" << setw(21)
		<< "Процент(-) вес: 5|" << setw(21) << "Пол отзывы(+) вес: 3|" << setw(21) << "Срок(-) вес: 2|" << '\n';
	cout << "---------------------------------------------------------------------------------------------------|\n";
	for (int i = 0; i < table.size(); i++) {
		cout << setw(15) << table[i].name << "|" << setw(20) << table[i].rating << "|"
			<< setw(20) << table[i].loanRate << "|" << setw(20)
			<< table[i].positiveReviewsCount << "|" << setw(20) << table[i].loanTerm << "|" << '\n';
		cout << "---------------------------------------------------------------------------------------------------|\n";
	}
}

void inputTable(vector <Record>& table) {
	table.push_back({ "Сбербанк", 5.4, 6.3, 980, 13 });
	table.push_back({ "ВТБ", 2.6, 14, 662, 15 });
	table.push_back({ "Альфабанк", 6.4, 12.7, 903, 21 });
	table.push_back({ "Открытие", 2.4, 11.1, 267, 20 });
	table.push_back({ "Бинбанк", 9.4, 4.1, 838, 14 });
	table.push_back({ "Росбанк", 7.8, 7.7, 266, 10 });
	table.push_back({ "Абсолют банк", 4.1, 4.6, 315, 14 });
	table.push_back({ "АК барс", 3, 6, 252, 5 });
	table.push_back({ "Промсвязь банк", 10, 12.5, 852, 9 });
	table.push_back({ "Россельхоз банк", 5.8, 5.2, 522, 21 });
	cout << "Изначальная таблица:\n\n";
	outputTable(table);
	// рейтинг: [2; 4] - 4, [4.1; 6] - 8, [6.1 - 8] - 12, [8.1; 10] - 15
	// процентная ставка: [4.1; 6] - 5, [6.1; 8] - 10, [8.1; 10] - 15, [10; 15] - 20
	// кол-во положительных отзывов: [200; 400] - 3, [401; 600] - 6, [601; 800] - 9, [801; 1000] - 12
	// срок кредитования: [5; 10] - 2, [10.1; 15] - 4, [15.1; 21] - 6
	// кодировка:
	for (int i = 0; i < table.size(); i++) {
		if ((table[i].rating >= 2) && (table[i].rating <= 4)) {
			table[i].rating = 4;
		}
		else if ((table[i].rating > 4) && (table[i].rating <= 6)) {
			table[i].rating = 8;
		}
		else if ((table[i].rating > 6) && (table[i].rating <= 8)) {
			table[i].rating = 12;
		}
		else table[i].rating = 16;
		if ((table[i].loanRate > 4) && (table[i].loanRate <= 6)) {
			table[i].loanRate = 5;
		}
		else if ((table[i].loanRate > 6) && (table[i].loanRate <= 8)) {
			table[i].loanRate = 10;
		}
		else if ((table[i].loanRate > 8) && (table[i].loanRate <= 10)) {
			table[i].loanRate = 15;
		}
		else table[i].loanRate = 20;
		if ((table[i].positiveReviewsCount >= 200) && (table[i].positiveReviewsCount <= 400)) {
			table[i].positiveReviewsCount = 3;
		}
		else if ((table[i].positiveReviewsCount > 400) && (table[i].positiveReviewsCount <= 600)) {
			table[i].positiveReviewsCount = 6;
		}
		else if ((table[i].positiveReviewsCount > 600) && (table[i].positiveReviewsCount <= 800)) {
			table[i].positiveReviewsCount = 9;
		}
		else table[i].positiveReviewsCount = 12;
		if ((table[i].loanTerm >= 5) && (table[i].loanTerm <= 10)) {
			table[i].loanTerm = 2;
		}
		else if ((table[i].loanTerm > 10) && (table[i].loanTerm <= 15)) {
			table[i].loanTerm = 4;
		}
		else table[i].loanTerm = 6;
	}
}
