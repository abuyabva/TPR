#include "Header.h"
// Сталь т.   Объём: 500    Нормы расхода 1) 10  2) 70
// Цветные металы кг   Объём: 510 Нормы расхода  1) 20  2) 50
// Станки станков/час  Объём: 3100  Нормы расхода  1) 200  2) 100
// Прибыть ден.ед.  5 для 1 и 2 норм
// I - 10, 20, 200: 5
// II - 70, 50, 100: 5
// x1 - кол-во 1-го изделия    x2 - кол-во 2-го
// x1,x2 >= 0
// 10x1 + 70x2 <= 500
// 20x1 + 50x2 <=510
// 200x1 + 100x2 <=3100
// f(x) = 5 * (x1 + x2) -> max
// Переход от неравенств к равенствам
// 10x1 + 70x2 + x3 = 500
// 20x1 + 50x2 + x4 = 510
// 200x1 + 100x2 + x5 = 3100
// A1 = (10; 20; 200), A2 = (70; 50; 100), A3 = (1; 0; 0)
// A4 = (0; 1; 0), A5 = (0; 0; 1), A0 = (500, 510, 3100)

int main()
{
    setlocale(LC_ALL, "rus");
    C.resize(5);
    C = { {"x1", 5}, {"x2", 5}, {"x3", 0}, {"x4", 0}, {"x5", 0} };
    buildSimplexTable();
}

double rectangleRule(double aij, double ais, double arj, double ars) {
    return ((aij * ars) - (ais * arj)) / ars;
}

void buildSimplexTable() {
    SimplexTable t;
    t.high.resize(3);
    t.high = { "x1", "x2", "A0" };
    t.content.push_back({ "x3", 10, 70, 500 });
    t.content.push_back({ "x4", 20, 50, 510 });
    t.content.push_back({ "x5", 200, 100, 3100 });
    getCb(t);
    find_f_string(t);
    outputSimplexTable(t);
    if (!check_f_string(t.content[t.content.size() - 1])) {
        string newBasis = findMinInF(t), erBasis;
        double a;
        if (newBasis == t.high[0]) {
            erBasis = eraseFromBasis(t, 0);
        }
        else {
            erBasis = eraseFromBasis(t, 1);
        }
        cout << newBasis << " " << erBasis << '\n';
        SimplexTable p = t;
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 3; j++) {
                if ((t.high[i] == newBasis) && (t.content[j].x == erBasis)) {
                    p.high[i] = erBasis;
                    p.content[j].x = newBasis;
                    if (i == 0) {
                        a = t.content[j].first;
                        p.content[j].first = pow(a, -1);
                    }
                    else {
                        a = t.content[j].second;
                        p.content[j].second = pow(a, -1);
                    }
                    break;
                }
            }
        }
        buildNewSimplexTable(t, p, a);
    }
}

void buildNewSimplexTable(SimplexTable t, SimplexTable p, double a) {
    getCb(p);
    vector <vector <double>> matrix_t, matrix_p;
    double r, s;
    matrix_t.resize(4);
    matrix_p.resize(4);
    for (int i = 0; i < 4; i++) {
        matrix_t[i].resize(3);
        matrix_t[i] = { t.content[i].first, t.content[i].second, t.content[i].third };
        matrix_p[i].resize(3);
        matrix_p[i] = { p.content[i].first, p.content[i].second, p.content[i].third };
    }
    for (double i = 0; i < 4; i++) {
        for (double j = 0; j < 3; j++) {
            if (a == matrix_t[i][j]) {
                r = i;
                s = j;
            }
        }
    }
    cout << r << " " << s << "   rs\n";
    for (double i = 0; i < 4; i++) {
        for (double j = 0; j < 3; j++) {
            if ((i == r) && (j == s)) {
                continue;
            }
            else if ((i == r) && (j != s)) {
                matrix_p[i][j] = matrix_t[i][j] / a;
            }
            else if ((i != r) && (j == s)) {
                matrix_p[i][j] = -1 * matrix_t[i][j] / a;
            }
            else {
                matrix_p[i][j] = rectangleRule(matrix_t[i][j], matrix_t[i][s], matrix_t[r][j], a);
            }
        }
    }
    for (int i = 0; i < p.content.size(); i++) {
        p.content[i].first = matrix_p[i][0];
        p.content[i].second = matrix_p[i][1];
        p.content[i].third = matrix_p[i][2];
    }
    vector <double> A0;
    for (int i = 0; i < p.content.size() - 1; i++) {
        A0.push_back(p.content[i].third);
    }
    p.content[p.content.size() - 1].third = findRelativeMark(p.Cb, A0, 0);
    outputSimplexTable(p);
    if (!check_f_string(p.content[p.content.size() - 1])) {
        string newBasis = findMinInF(p), erBasis;
        double a;
        if (newBasis == p.high[0]) {
            erBasis = eraseFromBasis(p, 0);
        }
        else {
            erBasis = eraseFromBasis(p, 1);
        }
        cout << newBasis << " " << erBasis << '\n';
        SimplexTable k = p;
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 3; j++) {
                if ((p.high[i] == newBasis) && (p.content[j].x == erBasis)) {
                    k.high[i] = erBasis;
                    k.content[j].x = newBasis;
                    if (i == 0) {
                        a = p.content[j].first;
                        k.content[j].first = pow(a, -1);
                    }
                    else {
                        a = p.content[j].second;
                        k.content[j].second = pow(a, -1);
                    }
                    break;
                }
            }
        }
        buildNewSimplexTable(p, k, a);
    }
    else {
        cout << "Максимальная прибыль: " << p.content[p.content.size() - 1].third << '\n';
        cout << "Достигатся, если производить:\n";
        for (int i = 0; i < p.content.size(); i++) {
            if (p.content[i].x == "x1") {
                cout << p.content[i].third << " Деталей 1-го типа\n";
                break;
            }
        }
        for (int i = 0; i < p.content.size(); i++) {
            if (p.content[i].x == "x2") {
                cout << p.content[i].third << " Деталей 2-го типа\n";
                break;
            }
        }
    }
}

void getCb(SimplexTable& t) {
    vector <double> cb;
    for (int i = 0; i < t.content.size(); i++) {
        if (getByX(t.content[i].x) != -1) {
            cb.push_back(getByX(t.content[i].x));
        }
    }
    t.Cb = cb;
}

string eraseFromBasis(SimplexTable t, int x) {
    vector <double> del;
    double min = 100000;
    string res;
    if (x == 0) {
        for (int i = 0; i < t.content.size() - 1; i++) {
            if (((t.content[i].third / t.content[i].first) > 0) && (min > (t.content[i].third / t.content[i].first))) {
                min = t.content[i].third / t.content[i].first;
                res = t.content[i].x;
            }
        }
    }
    else {
        for (int i = 0; i < t.content.size() - 1; i++) {
            if (((t.content[i].third / t.content[i].second) > 0) && (min > (t.content[i].third / t.content[i].second))) {
                min = t.content[i].third / t.content[i].second;
                res = t.content[i].x;
            }
        }
    }
    return res;
}

string findMinInF(SimplexTable t) {
    if (t.content[t.content.size() - 1].first <= t.content[t.content.size() - 1].second) {
        return t.high[0];
    }
    else
        return t.high[1];
}

bool check_f_string(SimplexTableString f) {
    if ((f.first >= 0) && (f.second >= 0)) {
        return true;
    }
    else
        return false;
}

void find_f_string(SimplexTable& t) {
    SimplexTableString f;
    vector <double> A1, A2, A0;
    f.x = "f";
    for (int i = 0; i < t.content.size(); i++) {
        A1.push_back(t.content[i].first);
        A2.push_back(t.content[i].second);
        A0.push_back(t.content[i].third);
    }
    f.first = findRelativeMark(t.Cb, A1, getByX(t.high[0]));
    f.second = findRelativeMark(t.Cb, A2, getByX(t.high[1]));
    f.third = findRelativeMark(t.Cb, A0, 0);
    t.content.push_back(f);
}

double getByX(string key) {
    for (int i = 0; i < C.size(); i++) {
        if (C[i].x == key) {
            return C[i].coefficient;
        }
    }
    return -1;
}

double findRelativeMark(vector <double> cb, vector <double> A, double c) {
    double del = 0;
    for (int i = 0; i < cb.size(); i++) {
        del += (cb[i] * A[i]);
    }
    del -= c;
    return del;
}

void outputSimplexTable(SimplexTable t) {
    cout << "Симплекс-таблица:\n\n";
    cout << "Cb" << setw(3) << "Cj" << setw(8) << getByX(t.high[0]) << setw(8) << getByX(t.high[1]) << '\n';
    cout << setw(13) << t.high[0] << setw(8) << t.high[1] << setw(8) << t.high[2] << '\n';
    for (int i = 0; i < t.content.size(); i++) {
        if (i < t.Cb.size()) {
            cout << t.Cb[i];
        }
        cout << setw(4) << t.content[i].x << setw(8) << t.content[i].first << setw(8) << t.content[i].second << setw(8) << t.content[i].third << '\n';
    }
}
