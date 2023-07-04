#include "Header.h"

int main() {
	vector <Record> table;
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	inputTable(table);
	outputTable(table);
	Pareto(table);
}

void Pareto(vector <Record> table) {
	vector <vector <string>> newParetoTable;
	vector <Record> paretoBestChoises;
	newParetoTable.resize(10);
	for (int i = 0; i < 10; i++) {
		newParetoTable[i].resize(10);
	}
	for (int i = 0; i < table.size(); i++) {
		for (int j = 0; j < table.size(); j++) {
			if (i != j) {
				bool first = false, second = false;
				if (table[i].rating > table[j].rating) {
					first = true;
				}
				else {
					second = true;
				}
				if (table[i].loanRate < table[j].loanRate) {
					first = true;
				}
				else {
					second = true;
				}
				if (table[i].positiveReviewsCount > table[j].positiveReviewsCount) {
					first = true;
				}
				else {
					second = true;
				}
				if (table[i].loanTerm < table[j].loanTerm) {
					first = true;
				}
				else {
					second = true;
				}
				if ((first && second) || (!first && !second)) {
					newParetoTable[i][j] = "-";
				}
				else if (first) {
					newParetoTable[i][j] = i + '0';
					addInPareto(table[i], paretoBestChoises);
				}
				else {
					newParetoTable[i][j] = j + '0';
					addInPareto(table[j], paretoBestChoises);
				}
			}
			else {
				newParetoTable[i][j] = "x";
				continue;
			}
		}
	}
	outputNewParetoTable(table, newParetoTable);
	if (paretoBestChoises.size() > 1) {
		cout << "\nПрименение первого сужения:\n";
		constriction1(table);
		cout << "\nПрименение второго сужения:\n";
		constriction2(table);
		cout << "\nПрименение третьего сужения:\n";
		constriction3(table);
	}
}

void constriction1(vector <Record> table) {
	cout << "Главный критерий - процентная ставка\n";
	cout << "Ограничения выставлены по критериям:\n";
	cout << "Рейтинг банка min - 7\n";
	cout << "Срок кредитования max - 15\n";
	for (int i = 0; i < table.size(); i++) {
		if ((table[i].rating < 7) || (table[i].loanTerm > 15)) {
			table.erase(table.begin() + i);
			i--;
		}
	}
	cout << "После сужения получилось следующее множество:\n\n";
	outputTable(table);
	double minLoanRate = 100;
	for (int i = 0; i < table.size(); i++) {
		if (table[i].loanRate > minLoanRate) {
			table.erase(table.begin() + i);
			i--;
		}
		else {
			minLoanRate = table[i].loanRate;
		}
	}
	cout << "\nЛучший вариант по методу первого сужения:\n\n";
	outputTable(table);
}

void constriction2(vector <Record> table) {
	cout << "Ограничения выставлены по критериям:\n";
	cout << "Рейтинг банка min - 7\n";
	cout << "Количество положительный отзывов min - 800\n";
	for (int i = 0; i < table.size(); i++) {
		if ((table[i].rating < 7) || (table[i].positiveReviewsCount < 800)) {
			table.erase(table.begin() + i);
			i--;
		}
	}
	cout << "После сужения получилось следующее множество:\n\n";
	outputTable(table);
	Pareto(table);
}

void constriction3(vector <Record> table) {
	cout << "Важнейший критерий - процентная ставка по кредиту\n";
	double minLoanRate = 100;
	bool flag = false;
	for (int i = 0; i < table.size(); i++) {
		if (table[i].loanRate <= minLoanRate) {
			minLoanRate = table[i].loanRate;
		}
		else {
			table.erase(table.begin() + i);
			i--;
		}
		if ((i == table.size() - 1) && (!flag)) {
			i = -1;
			flag = true;
		}
	}
	flag = false;
	if (table.size() > 1) {
		cout << "После сужения получилось множество:\n\n";
		outputTable(table);
		cout << "Следующий по важности критерий - рейтинг банка\n";
		double maxRating = 0;
		for (int i = 0; i < table.size(); i++) {
			if (table[i].rating >= maxRating) {
				maxRating = table[i].rating;
			}
			else {
				table.erase(table.begin() + i);
				i--;
			}
			if ((i == table.size() - 1) && (!flag)) {
				i = -1;
				flag = true;
			}
		}
		flag = false;
		if (table.size() > 1) {
			cout << "После сужения получилось множество:\n\n";
			outputTable(table);
			cout << "Следующий по важности критерий - количество положительных отзывов\n";
			double maxCount = 0;
			for (int i = 0; i < table.size(); i++) {
				if (table[i].positiveReviewsCount >= maxCount) {
					maxCount = table[i].positiveReviewsCount;
				}
				else {
					table.erase(table.begin() + i);
					i--;
				}
				if ((i == table.size() - 1) && (!flag)) {
					i = -1;
					flag = true;
				}
			}
			flag = false;
			if (table.size() > 1) {
				cout << "После сужения получилось множество:\n\n";
				outputTable(table);
				cout << "Следующий по важности критерий - срок кредитования\n";
				double minLoanTerm = 100;
				for (int i = 0; i < table.size(); i++) {
					if (table[i].loanTerm >= minLoanTerm) {
						minLoanTerm = table[i].loanTerm;
					}
					else {
						table.erase(table.begin() + i);
						i--;
					}
					if ((i == table.size() - 1) && (!flag)) {
						i = -1;
						flag = true;
					}
				}
			}
		}
	}
	cout << "Лучший результат после сужения:\n";
	outputTable(table);
}

void addInPareto(Record num, vector <Record>& paretoBestChoises) {
	for (int i = 0; i < paretoBestChoises.size(); i++) {
		if (num.name == paretoBestChoises[i].name) {
			return;
		}
	}
	paretoBestChoises.push_back(num);
}

void outputNewParetoTable(vector <Record> table, vector <vector <string>> newParetoTable) {
	cout << "\nПрименение метода Парето:\n\n";
	cout << setw(17) << "Название банка|\n";
	cout << "-----------------------------------------------------------|\n";
	for (int i = 0; i < table.size(); i++) {
		cout << setw(16) << table[i].name << "(" << i << ")|";
		for (int j = 0; j < newParetoTable.size(); j++) {
			cout << " " << newParetoTable[i][j] << " |";
		}
		cout << "\n-----------------------------------------------------------|\n";
	}
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

void inputTable(vector <Record>& table) {
	table.resize(10);
	table[0] = { "Сбербанк", 5.4, 6.3, 980, 13 };
	table[1] = { "ВТБ", 2.6, 14, 662, 15 };
	table[2] = { "Альфабанк", 6.4, 12.7, 903, 21 };
	table[3] = { "Открытие", 2.4, 11.1, 267, 20 };
	table[4] = { "Бинбанк", 9.4, 4.1, 838, 14 };
	table[5] = { "Росбанк", 7.8, 7.7, 266, 10 };
	table[6] = { "Абсолют банк", 4.1, 4.6, 315, 14 };
	table[7] = { "АК барс", 3, 6, 252, 5 };
	table[8] = { "Промсвязь банк", 10, 12.5, 852, 9 };
	table[9] = { "Россельхоз банк", 5.8, 5.2, 522, 21 };
}
