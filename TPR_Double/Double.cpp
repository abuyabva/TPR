// Сталь т. Объём: 500 Нормы расхода 1) 10 2) 70
// Цветные металы кг Объём: 510 Нормы расхода 1) 20 2) 50
// Станки станков/час Объём: 3100 Нормы расхода 1) 200 2) 100
// Прибыть ден.ед. 5 для 1 и 2 норм
// I - 10, 20, 200: 5
// II - 70, 50, 100: 5
// x1 - кол-во 1-го изделия x2 - кол-во 2-го
// x1,x2 >= 0
// 10x1 + 70x2 <= 500
// 20x1 + 50x2 <=510
// 200x1 + 100x2 <=3100
// f(x) = 5 * (x1 + x2) -> max
// Переход от неравенств к равенствам
// 10x1 + 20x2 >= 500
// 20x1 + 50x2 >= 510              дефицит
// 200x1 + 100x2 >= 3100           дефицит
// x1 > 0
// x2 > 0
//
// (10; 20; 200) (y1) (5)
// AT = (20; 50; 100) X (y2) >= (5)
// (y3)
//
// b = (500)
// (510) 
// (3100)
//
// 10y1 + 20y2 + 200y3 >= 5
// 70y1 + 50y2 + 100y3 >= 5
// y1, y2, y3 >= 0
//
// 500y1 + 510y2 + 3100y3 -> min
// 
// В последней симплекс таблице используются вектора x3 x2 x1
// 
//      (10)               (70)          (1)        (1;   20;  10)
// A1 = (20)          A2 = (50)     A3 = (0)    D = (0;   50;  20)
//      (200)              (100)         (0)        (0   100; 200)
//

#include "Header.h">

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	firstTeorem();
	secondTeorem();
	thirdTheorem();
}

void firstTeorem() 
{
	// Построим обратную матрицу D из последней симплекс таблицы
	// Cb = (0, 5, 5) - также из последней симплекс таблицы
	// g(y) = 500y1 + 510y2 + 3100y3 -> min
	vector <double> Gy;
	Gy.resize(3);
	Gy = { 500, 510, 3100 };
	vector <vector <double>> D;
	D.resize(3);
	for (int i = 0; i < D.size(); i++) D[i].resize(3);
	D[0] = { 1, -1.625, 0.1125 };
	D[1] = { 0, 0.025, -0.0025 };
	D[2] = { 0, -0.0125, 0.00625 };
	vector <double> Cb;
	Cb.resize(3);
	Cb = { 0, 5, 5 };
	vector <double> y;
	for (int i = 0; i < D.size(); i++) {
		double res = 0;
		for (int j = 0; j < D[i].size(); j++) {
			res += Cb[j] * D[j][i];
		}
		y.push_back(res);
	}
	double ans = 0;
	for (int i = 0; i < y.size(); i++) {
		ans += Gy[i] * y[i];
	}
	cout << "Минимальное значение целевой функции:\t" << ans << '\n';
}

void secondTeorem() 
{
	// 20y2 + 200y3 = 5
	// 50y2 + 100y3 = 5
	vector <double> Gy;
	Gy.resize(3);
	Gy = { 500, 510, 3100 };
	vector <double> y;
	y.resize(3);
	y = { 0, 1.0 / 16, 3.0 / 160 };
	double ans = 0;
	for (int i = 0; i < 3; i++) {
		ans += Gy[i] * y[i];
	}
	cout << "Вторая теорема двойственности:\t" << ans << '\n';
}

void thirdTheorem()
{
	double Gmax = 90;
	vector <double> y;
	y.resize(3);
	y = { 0, 1.0 / 16, 3.0 / 160 };
	vector <double> Gy;
	Gy.resize(3);
	Gy = { 500, 510, 3100 };
	vector <vector <double>> D;
	D.resize(3);
	for (int i = 0; i < D.size(); i++) D[i].resize(3);
	/*D[0] = { 1, -1.625, 0.1125 };
	  D[1] = { 0, 0.025, -0.0025 };
	  D[2] = { 0, -0.0125, 0.00625 };*/
	D[0] = { 1, 0, 0 };
	D[1] = { -1.625, 0.025, -0.0125 };
	D[2] = { 0.1125, -0.0025, 0.00625 };
	vector <double> A0, A;
	A0.resize(3);
	A.resize(3);
	A0 = { 20, 5, 13 };
	A = { 500, 510, 3100 };
	for (int i = D.size() - 1; i >= 0; i--) {
		double b_low = 0, b_up = 0;
		for (int j = 0; j < D.size(); j++) {
			if (D[i][j] > 0) {
				if ((A0[j] / D[i][j] > b_low) || (b_low == 0)) {
					b_low = A0[j] / D[i][j];
				}
			}
			else if (D[i][j] < 0) {
				if ((abs(A0[j] / D[i][j]) > b_up) || (b_up == 0)) {
					b_up = abs(A0[j] / D[i][j]);
				}
			}
		}
		if (b_low == 0) b_low = INFINITY;
		if (b_up == 0) b_up = INFINITY;
		cout << "Границы для " << D.size() - i << "-го ресурса:\t(" <<
			A[D.size() - 1 - i] - b_low << ", " << A[D.size() - 1 - i] + b_up << ")\n";
		if (i != 0) {
			Gmax += y[i] * b_up;
		}
	}
	cout << "Оптимальное значение целевой функции при максимальном значении ресурсов:\t" << Gmax << '\n';
}
