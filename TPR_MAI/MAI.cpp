#include "Header.h"

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	vector <Record> table;
	vector <vector <double>> v;
	vector <double> w; // вектор приоритетов критериев (2-го уровня)
	inputTable(table);
	input(v);
	criterion(v, w);
	MAI(table, w);
}

double geom(vector <double> v) {
	double res = 1;
	for (int i = 0; i < v.size(); i++) {
		res *= v[i];
	}
	return pow(res, 1.0 / v.size());
}

double geomSum(vector <vector <double>> v, vector <double>& w) {
	double res = 0;
	for (int i = 0; i < v.size(); i++) {
		res += geom(v[i]);
	}
	for (int i = 0; i < v.size(); i++) {
		cout << geom(v[i]) << " ";
		w.push_back(geom(v[i]) / res);
		w[w.size() - 1] = round(w[w.size() - 1] * 100) / 100;
	}
	cout << "\n";
	return res;
}

void input(vector <vector <double>>& v) {
	v.resize(4);
	for (int i = 0; i < v.size(); i++) {
		v[i].resize(4);
	}
	v[0] = { 1, 1.0 / 3, 3, 5 };
	v[1] = { 3, 1, 5, 7 };
	v[2] = { 1.0 / 3, 1.0 / 5, 1, 3 };
	v[3] = { 1.0 / 5, 1.0 / 7, 1.0 / 3, 1 };
}

void criterion(vector <vector <double>> v, vector <double>& w) {
	//geomSum(v, w);
	cout << "Сумма геометрического среднего для критериев:\n";
	cout << geomSum(v, w) << '\n';
	cout << "Вектор приоритетов:\n";
	cout << "(";
	for (int i = 0; i < w.size(); i++) {
		cout << w[i];
		if (i != w.size() - 1) {
			cout << "; ";
		}
	}
	cout << ")\n";
	vector <double> s;
	s.resize(4);
	s = { 4.53, 1.67, 9.33, 16 };
	if (isMatrixConformed(w, s, 4)) {
		cout << "Матрица критериев согласованна\n";
	}
	else {
		cout << "Матрица критериев несогласованна\n";
	}
}

void MAI(vector <Record> table, vector <double> w) {
	vector <vector <double>> altTable;
	vector <vector <double>> W;
	W.resize(4);
	altTable.resize(10);
	for (int i = 0; i < 10; i++) {
		altTable[i].resize(10);
	}
	compareAltTable(table, altTable, W);
	vector <double> total;
	double t = 0;
	for (int i = 0; i < W[0].size(); i++) {
		for (int j = 0; j < w.size(); j++) {
			t += (w[j] * W[j][i]);
		}
		total.push_back(t);
		t = 0;
	}
	cout << "Приоритеты альтернатив:\n";
	for (int i = 0; i < total.size(); i++) {
		cout << "Приоритет " << table[i].name << ": " << total[i] << '\n';
	}
}

bool isMatrixConformed(vector <double> w, vector <double> s, double n) {
	double lam = 0, IS = 0, SI = 0, OS = 0;
	if (n == 4) {
		SI = 0.9;
	}
	else if (n == 10) {
		SI = 1.49;
	}
	cout << "P:   ";
	for (int i = 0; i < s.size(); i++) {
		lam += (s[i] * w[i]);
		cout << s[i] * w[i] << " ";
	}
	cout << '\n';
	cout << "lam:   " << lam << '\n';
	IS = (lam - n) / (n - 1);
	cout << "IS:   " << IS << '\n';
	OS = IS / SI;
	cout << "OS:   " << OS << '\n';
	if (OS <= 0.1) {
		return true;
	}
	else {
		return false;
	}
}

void insertColumn(vector <vector <double>>& S, vector <vector <double>> altTable) {
	vector <double> column;
	double s = 0;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			s += altTable[j][i];
		}
		column.push_back(s);
		s = 0;
	}
	S.push_back(column);
	column.clear();
}

void compareAltTable(vector <Record> table, vector <vector <double>>& altTable, vector <vector <double>>& W) {
	vector <vector <double>> S;
	vector <double> column;
	double s = 0;
	for (int i = 0; i < altTable.size(); i++) {    // сравнение по первому критерию
		for (int j = 0; j < altTable[i].size(); j++) {
			for (double k = 1; k < 9; k++) {
				if (table[i].rating > table[j].rating) {
					if (table[j].rating + k > table[i].rating) {
						altTable[i][j] = k + 1;
						break;
					}
				}
				else if (table[j].rating > table[i].rating) {
					if (table[i].rating + k > table[j].rating) {
						altTable[i][j] = round((1.0 / (k + 1.0)) * 100) / 100;
						break;
					}
				}
				else {
					altTable[i][j] = 1;
					break;
				}
			}
		}
	}
	cout << "altTable\n";
	for (int i = 0; i < altTable.size(); i++) {
		for (int j = 0; j < altTable[i].size(); j++) {
			cout << setw(5) << altTable[i][j] << " ";
		}
		cout << '\n';
	}
	insertColumn(S, altTable);
	geomSum(altTable, W[0]);
	cout << "S:\n";
	for (int i = 0; i < S[0].size(); i++) {
		cout << S[0][i] << '\n';
	}
	//cout << geomSum(altTable, W[0]) << '\n';
	cout << "Вектор приоритетов:\n";
	cout << "(";
	for (int i = 0; i < W[0].size(); i++) {
		cout << W[0][i];
		if (i != W[0].size() - 1) {
			cout << "; ";
		}
	}
	cout << ")\n";
	for (int i = 0; i < 10; i++) {   // сравнение по 2-му критерию
		for (int j = 0; j < 10; j++) {
			for (double k = 1; k < 10; k++) {
				if (table[i].loanRate < table[j].loanRate) {
					if (table[i].loanRate + (k * 1.2) > table[j].loanRate) {
						altTable[i][j] = k + 1;
						break;
					}
				}
				else if (table[j].loanRate < table[i].loanRate) {
					if (table[j].loanRate + (k * 1.2) > table[i].loanRate) {
						altTable[i][j] = round((1.0 / (k + 1.0)) * 100) / 100;
						break;
					}
				}
				else {
					altTable[i][j] = 1;
					break;
				}
			}
		}
	}
	cout << "altTable\n";
	for (int i = 0; i < altTable.size(); i++) {
		for (int j = 0; j < altTable[i].size(); j++) {
			cout << setw(5) << altTable[i][j] << " ";
		}
		cout << '\n';
	}
	insertColumn(S, altTable);
	//geomSum(altTable, W[1]);
	cout << geomSum(altTable, W[1]) << '\n';
	cout << "S:\n";
	for (int i = 0; i < S[1].size(); i++) {
		cout << S[1][i] << '\n';
	}
	cout << "Вектор приоритетов:\n";
	cout << "(";
	for (int i = 0; i < W[1].size(); i++) {
		cout << W[1][i];
		if (i != W[1].size() - 1) {
			cout << "; ";
		}
	}
	cout << ")\n";
	for (int i = 0; i < 10; i++) {   // сравнение по 3-му критерию
		for (int j = 0; j < 10; j++) {
			for (double k = 1; k < 10; k++) {
				if (table[i].positiveReviewsCount > table[j].positiveReviewsCount) {
					if (table[j].positiveReviewsCount + (k * 90) > table[i].positiveReviewsCount) {
						altTable[i][j] = k + 1;
						break;
					}
				}
				else if (table[j].positiveReviewsCount > table[i].positiveReviewsCount) {
					if (table[i].positiveReviewsCount + (k * 90) > table[j].positiveReviewsCount) {
						altTable[i][j] = round((1.0 / (k + 1.0)) * 100) / 100;
						break;
					}
				}
				else {
					altTable[i][j] = 1;
					break;
				}
			}
		}
	}
	cout << "altTable\n";
	for (int i = 0; i < altTable.size(); i++) {
		for (int j = 0; j < altTable[i].size(); j++) {
			cout << setw(5) << altTable[i][j] << " ";
		}
		cout << '\n';
	}
	insertColumn(S, altTable);
	//geomSum(altTable, W[2]);
	cout << geomSum(altTable, W[2]) << '\n';
	cout << "S:\n";
	for (int i = 0; i < S[2].size(); i++) {
		cout << S[2][i] << '\n';
	}
	cout << "Вектор приоритетов:\n";
	cout << "(";
	for (int i = 0; i < W[2].size(); i++) {
		cout << W[2][i];
		if (i != W[2].size() - 1) {
			cout << "; ";
		}
	}
	cout << ")\n";
	for (int i = 0; i < 10; i++) {   // сравнение по 4-му критерию
		for (int j = 0; j < 10; j++) {
			for (double k = 1; k < 10; k++) {
				if (table[i].loanTerm < table[j].loanTerm) {
					if (table[i].loanTerm + (k * 2) >= table[j].loanTerm) {
						altTable[i][j] = k + 1;
						break;
					}
				}
				else if (table[j].loanTerm < table[i].loanTerm) {
					if (table[j].loanTerm + (k * 2) >= table[i].loanTerm) {
						altTable[i][j] = round((1.0 / (k + 1.0)) * 100) / 100;
						break;
					}
				}
				else {
					altTable[i][j] = 1;
					break;
				}
			}
		}
	}
	cout << "altTable\n";
	for (int i = 0; i < altTable.size(); i++) {
		for (int j = 0; j < altTable[i].size(); j++) {
			cout << setw(5) << altTable[i][j] << " ";
		}
		cout << '\n';
	}
	insertColumn(S, altTable);
	//geomSum(altTable, W[3]);
	cout << geomSum(altTable, W[3]) << '\n';
	cout << "S:\n";
	for (int i = 0; i < S[3].size(); i++) {
		cout << S[3][i] << '\n';
	}
	cout << "Вектор приоритетов:\n";
	cout << "(";
	for (int i = 0; i < W[3].size(); i++) {
		cout << W[3][i];
		if (i != W[3].size() - 1) {
			cout << "; ";
		}
	}
	cout << ")\n";
	for (int i = 0; i < W.size(); i++) {
		if (isMatrixConformed(W[i], S[i], 10)) {
			cout << "Матрица К" << i + 1 << " согласованна\n";
		}
		else {
			cout << "Матрица К" << i + 1 << " не согласованна\n";
		}
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
}

void outputTable(vector <Record> table) {
	cout << setw(16) << "Название банка|" << setw(14) << "Рэйтинг(+)|" << setw(14)
		<< "Процент(-)|" << setw(14) << "Пол отзывы(+)|" << setw(14) << "Срок(-)|" << '\n';
	cout << "-----------------------------------------------------------------------|\n";
	for (int i = 0; i < table.size(); i++) {
		cout << setw(15) << table[i].name << "|" << setw(13) << table[i].rating << "|"
			<< setw(13) << table[i].loanRate << "|" << setw(13)
			<< table[i].positiveReviewsCount << "|" << setw(13) << table[i].loanTerm << "|" << '\n';
		cout << "-----------------------------------------------------------------------|\n";
	}
}
