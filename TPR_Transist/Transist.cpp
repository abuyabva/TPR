#include "Header.h"

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	vector <vector <Node>> costTable;
	costTable.resize(4);
	for (int i = 0; i < 4; i++) {
		costTable[i].resize(5);
	}
	costTable[0] = { {16, false}, {30, false}, {17, false}, {10, false}, {16, false} };
	costTable[1] = { {20, false}, {27, false}, {26, false}, {9, false}, {23, false} };
	costTable[2] = { {13, false}, {4, false}, {22, false}, {3, false}, {1, false} };
	costTable[3] = { {2, false}, {1, false}, {5, false}, {4, false}, {24, false} };
	vector <Vertex> providers, consumers;
	providers.resize(4);
	consumers.resize(5);
	providers = { {4, 0}, {6, 0}, {10, 0}, {10, 0} };
	consumers = { {7, 0}, {7, 0}, {7, 0}, {7, 0}, {2, 0} };
	cout << "Стоимость по опорному плану:\t" << north_west(costTable, providers, consumers) << '\n';
	cout << "Стоимость по методу минимальной стоимости:" << minCostMethod(costTable, providers, consumers) << '\n';
	findPotential(costTable, providers, consumers);
	double cost = 0;
	for (int i = 0; i < costTable.size(); i++) {
		for (int j = 0; j < costTable[i].size(); j++) {
			if (costTable[i][j].basis) {
				cost += (costTable[i][j].waste * costTable[i][j].cost);
			}
		}
	}
	cout << "Финальная стоимость:\t" << cost << '\n';
}

void outputCostTable(vector <vector <Node>> costTable) {
	cout << "Таблица стоимости перевозок:\n";
	for (int i = 0; i < costTable.size(); i++) {
		for (int j = 0; j < costTable[i].size(); j++) {
			if (costTable[i][j].basis) {
				cout << setw(5) << "bas " << setw(2) << costTable[i][j].waste;
			}
			else {
				cout << setw(7) << "xxxxx";
			}
		}
		cout << '\n';
	}
}

double north_west(vector <vector <Node>>& costTable, vector <Vertex> providers, vector <Vertex> consumers) {
	double cost = 0;
	for (int i = 0; i < costTable[0].size(); i++) {
		for (int j = 0; j < costTable.size(); j++) {
			if (consumers[i].content <= 0) {
				break;
			}
			if (providers[j].content <= 0) {
				continue;
			}
			if (providers[j].content >= consumers[i].content) {
				cost += consumers[i].content * costTable[j][i].cost;
				costTable[j][i].waste += consumers[i].content;
				costTable[j][i].basis = true;
				providers[j].content -= consumers[i].content;
				consumers[i].content = 0;
				break;
			}
			else if (providers[j].content < consumers[i].content) {
				cost += providers[j].content * costTable[j][i].cost;
				costTable[j][i].basis = true;
				costTable[j][i].waste += providers[j].content;
				consumers[i].content -= providers[j].content;
				providers[j].content = 0;
			}
		}
	}
	outputCostTable(costTable);
	return cost;
}

double findMin(vector <vector <Node>> costTable) {
	double min = 100;
	for (int i = 0; i < costTable.size(); i++) {
		for (int j = 0; j < costTable[i].size(); j++) {
			if (costTable[i][j].cost < min) {
				min = costTable[i][j].cost;
			}
		}
	}
	return min;
}

double eraseTable(vector <vector <Node>>& costTable, vector <Vertex>& providers, vector <Vertex>& consumers)
{
	double totalCost = 0;
	double min = findMin(costTable);
	for (int i = 0; i < costTable.size(); i++) {
		for (int j = 0; j < costTable[i].size(); j++) {
			if (costTable[i][j].cost == min) {
				if (providers[i].content >= consumers[j].content) {
					providers[i].content -= consumers[j].content;
					totalCost += costTable[i][j].cost * consumers[j].content;
					for (int k = 0; k < costTable.size(); k++) {
						costTable[k].erase(costTable[k].begin() + j);
					}
					consumers.erase(consumers.begin() + j);
					j = 0;
				}
				else {
					consumers[j].content -= providers[i].content;
					totalCost += costTable[i][j].cost * providers[i].content;
					costTable.erase(costTable.begin() + i);
					providers.erase(providers.begin() + i);
					i = 0;
					break;
				}
				if (providers[i].content == 0) {
					costTable.erase(costTable.begin() + i);
					providers.erase(providers.begin() + i);
					i = 0;
					break;
				}
			}
		}
	}
	return totalCost;
}

double minCostMethod(vector <vector <Node>> costTable, vector <Vertex> providers, vector <Vertex> consumers)
{
	double totalCost = 0;
	while (providers.size() > 0) {
		totalCost += eraseTable(costTable, providers, consumers);
	}
	return totalCost;
}

vector <vector <int>> buildBorder(vector <vector <Node>> costTable, vector <int> step) {
	vector <vector <int>> borders;
	vector <int> x;
	vector <int> y;
	for (int i = 0; i < costTable.size(); i++) {
		if (costTable[i][step[1]].basis) {
			x.push_back(i);
		}
	}
	if (x.size() > 1) {
		for (int i = 0; i < x.size(); i++) {
			bool find = false;
			for (int j = 0; j < costTable[i].size(); j++) {
				if (j == step[1]) {
					continue;
				}
				if (costTable[x[i]][j].basis) {
					find = true;
					break;
				}
			}
			if (!find) {
				x.erase(x.begin() + i, x.begin() + 1 + i);
				i--;
			}
		}
	}
	x.push_back(step[0]);
	sort(x.begin(), x.end());
	x.erase(x.begin() + 1, x.end() - 1);

	for (int i = 0; i < costTable[0].size(); i++) {
		if (costTable[step[0]][i].basis) {
			y.push_back(i);
		}
	}
	if (y.size() > 1) {
		for (int i = 0; i < y.size(); i++) {
			bool find = false;
			for (int j = 0; j < costTable.size(); j++) {
				if (j == step[0]) {
					continue;
				}
				if (costTable[j][y[i]].basis) {
					find = true;
					break;
				}
			}
			if (!find) {
				y.erase(y.begin() + i, y.begin() + 1 + i);
				i--;
			}
		}
	}
	y.push_back(step[1]);
	sort(y.begin(), y.end());
	y.erase(y.begin() + 1, y.end() - 1);
	borders.push_back(x);
	borders.push_back(y);
	cout << "\nГраницы:\nx: " << borders[0][0] << " " << borders[0][1] << "\ny: " << borders[1][0] << " " << borders[1][1] << '\n';
	return borders;
}

int basisNum(vector <vector <Node>> costTable, vector <vector <int>> borders) {
	int res = 0;
	for (int i = borders[0][0]; i <= borders[0][1]; i++) {
		for (int j = borders[1][0]; j <= borders[1][1]; j++) {
			if (costTable[i][j].basis) {
				res++;
			}
		}
	}
	return res;
}

vector <vector <int>> buildCycle(vector <vector <int>> allVertexes, vector <int> start, vector <vector <int>> cycle, bool& end) {
	for (int i = 0; i < allVertexes.size(); i++) {
		if ((cycle[0][0] == cycle[cycle.size() - 1][0] || cycle[0][1] == cycle[cycle.size() - 1][1]) && cycle.size() > 2 && cycle.size() % 2 == 0) {
			end = true;
			return cycle;
		}
		if ((allVertexes[i][0] == cycle[cycle.size() - 1][0]) || (allVertexes[i][1] == cycle[cycle.size() - 1][1])) {
			cycle.push_back(allVertexes[i]);
			allVertexes.erase(allVertexes.begin() + i);
			i--;
			if (allVertexes.size() == 0) {
				end = true;
				return cycle;
			}
			cycle = buildCycle(allVertexes, start, cycle, end);
			if (end) return cycle;
		}
	}
	cycle.pop_back();
	return cycle;
}

bool potentialCheck(vector <vector <Node>> costTable, vector <Vertex> providers, vector <Vertex> consumers) {
	for (int i = 0; i < costTable.size(); i++) {
		for (int j = 0; j < costTable[i].size(); j++) {
			if (costTable[i][j].basis) {
				if (providers[i].rate + consumers[j].rate != costTable[i][j].cost) {
					return false;
				}
			}
		}
	}
	return true;
}

void findPotential(vector <vector <Node>>& costTable, vector <Vertex>& providers, vector <Vertex>& consumers)
{
	bool flag = false;
	while (!potentialCheck(costTable, providers, consumers)) {
		for (int i = 0; i < costTable.size(); i++) {
			for (int j = 0; j < costTable[i].size(); j++) {
				if (costTable[i][j].basis) {
					if ((i == 0) || (providers[i].rate != 0)) {
						consumers[j].rate = costTable[i][j].cost - providers[i].rate;
					}
					else if (consumers[j].rate != 0) {
						providers[i].rate = costTable[i][j].cost - consumers[j].rate;
					}
				}
			}
		}
	}
	Min x;
	x.val = 1;
	flag = true;
	cout << "Оценки небазисных:\n";
	for (int i = 0; i < costTable.size(); i++) {
		for (int j = 0; j < costTable[i].size(); j++) {
			if (!costTable[i][j].basis) {
				costTable[i][j].rate = costTable[i][j].cost - (providers[i].rate + consumers[j].rate);
				cout << setw(3) << costTable[i][j].rate << "  в таблице: " << i + 1 << j + 1 << '\n';
				if (costTable[i][j].rate < 0) {
					flag = false;
				}
				if (x.val > costTable[i][j].rate) {
					x.val = costTable[i][j].rate;
					x.x = i;
					x.y = j;
				}
			}
		}
	}
	cout << '\n';
	if (!flag) {
		cout << "\nСамая низкая оценка: " << x.x + 1 << x.y + 1 << '\n';
		vector <vector <int>> cycle;
		vector <int> step;
		step.push_back(x.x);
		step.push_back(x.y);
		cycle.push_back(step);
		vector <vector <int>> allVertexes;
		vector <int> v;
		v.resize(2);
		for (int i = 0; i < costTable.size(); i++) {
			for (int j = 0; j < costTable[i].size(); j++) {
				if (costTable[i][j].basis) {
					v[0] = i;
					v[1] = j;
					allVertexes.push_back(v);
				}
			}
		}
		bool end = false;
		cycle = buildCycle(allVertexes, step, cycle, end);
		cout << "Цикл в таблице:\t";
		for (int i = 0; i < cycle.size(); i++) {
			cout << cycle[i][0] << cycle[i][1] << "  ";
		}
		cout << '\n';
		for (int i = 0; i < cycle.size() - 2; i++) {
			if ((cycle[i][0] == cycle[i + 1][0]) && (cycle[i + 1][0] == cycle[i + 2][0]) ||
				(cycle[i][1] == cycle[i + 1][1]) && (cycle[i + 1][1] == cycle[i + 2][1])) {
				cycle.erase(cycle.begin() + i + 1);
			}
		}
		if ((cycle[cycle.size() - 2][0] == cycle[cycle.size() - 1][0] && cycle[cycle.size() - 1][0] == cycle[0][0]) ||
			(cycle[cycle.size() - 2][1] == cycle[cycle.size() - 1][1] && cycle[cycle.size() - 1][1] == cycle[0][1])) {
			cycle.pop_back();
		}
		double min = 0;
		for (int i = 1; i < cycle.size(); i += 2) {
			if ((min == 0) || (costTable[cycle[i][0]][cycle[i][1]].waste < min)) {
				min = costTable[cycle[i][0]][cycle[i][1]].waste;
			}
		}
		cout << "\n Min: " << min << "\n";
		for (int i = 1; i < cycle.size(); i += 2) {
			if (costTable[cycle[i][0]][cycle[i][1]].waste == min) {
				costTable[cycle[i][0]][cycle[i][1]].basis = false;
				costTable[cycle[0][0]][cycle[0][1]].basis = true;
				costTable[cycle[0][0]][cycle[0][1]].waste = min;
				break;
			}
		}
		for (int i = 1; i < cycle.size(); i++) {
			if (i % 2 == 0) {
				costTable[cycle[i][0]][cycle[i][1]].waste += min;
			}
			else {
				costTable[cycle[i][0]][cycle[i][1]].waste -= min;
			}
		}
		outputCostTable(costTable);
		cout << "Оценки потребителей:\t";
		for (int i = 0; i < consumers.size(); i++) {
			cout << setw(5) << consumers[i].rate;
		}
		cout << "\nОценки поставщиков:";
		for (int i = 0; i < providers.size(); i++) {
			cout << setw(5) << providers[i].rate;
		}
		cout << '\n';
		providers = { {4, 0}, {6, 0}, {10, 0}, {10, 0} };
		consumers = { {7, 0}, {7, 0}, {7, 0}, {7, 0}, {2, 0} };
		cout << "------------------------------------------------------\n";
		findPotential(costTable, providers, consumers);
	}
}
