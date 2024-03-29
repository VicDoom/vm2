#include<iostream>
#include<fstream>

using namespace std;

bool checkInput(ifstream& fin) {

	if (fin.is_open()) {
		return true;
	}
	else {
		return false;
	}
}

bool readInput(ifstream& fin, ofstream& fout, int& n, double *& xArray, double *& yArray, double &A, double &B, double &XX) {

	int count = 0;

	fin >> n;

	xArray = new double[n + 1];
	yArray = new double[n + 1];
	bool isRightInput = true;

	double num;

	for (int i = 0; i < n + 1 && !fin.eof(); i++) {

		fin >> num;
		xArray[i] = num;
		count++;

	}

	if (fin.eof()) {

		isRightInput = false;

	}

	for (int i = 0; i < n + 1 && !fin.eof(); i++) {

		fin >> num;
		yArray[i] = num;
		count++;

	}

	if (!fin.eof()) {

		fin >> A >> B >> XX;

	}
	else {
		isRightInput = false;
	}

	(isRightInput)
		? fout << "������ ������� �����" << '\n'
		: fout << "����������� �������� ������";

	return (isRightInput);
}

bool IER(ofstream& fout, int n, double* xArray, double* yArray, 
	double A, double B, double XX) {
	if (n < 3) {

		fout << "IER = 1";
		return false;

	}
	else {

		int i = 1;
		while (i != n + 1 && xArray[i - 1] < xArray[i]) {
			
			i++;

		}

		if (i != n + 1) {

			fout << "IER = 2";
			return false;

		}
		else {

			if (xArray[n] < XX || XX < xArray[0]) {

				fout << "IER = 3";
				return false;
			} 
			
			else {

				fout << "IER = 0";
				return true;
			}

		} 
	}

}

void fillH(double*xArray, double*& h, int n) {

	for (int i = 1; i <= n; i++) {
		h[i] = xArray[i] - xArray[i - 1];
	}

}

void fillMass(int n, double*& up, double*& down , double*& center, double*h) {

	up[0] = 0.5;
	down[n] = 0;
	center[0] = 1;
	center[n] = 1;

	for (int i = 1; i < n; i++) {
		up[i] = h[i + 1];
		down[i] = h[i];
		center[i] = 2 * (h[i] + h[i + 1]);
	}

	//cout << '\n' << center[0] << ' ' << up[0];
	//for (int i = 1; i < n; i++) {
	//	cout << '\n' << down[i] << ' ' << center[i] << ' ' << up[i];
	//}

}

void fillF(double*& F, int n, double A, double B, double* h, double* f) {

	F[0] = (3 / h[1]) * ((f[1] - f[0]) / h[1] - A) ;
	F[n] = B;

	for (int i = 1; i < n; i++) {

		F[i] = 6 * ((f[i + 1] - f[i]) / h[i + 1] - (f[i] - f[i - 1]) / h[i]);

	}

}

void findC(int n, double* a, double* c, double* b, double* F, double*& indexC) {

	// ������ ��������
	double* m = new double[n+1];
	double* l = new double[n+1];

	m[1] = (-1) * b[0] / c[0];
	l[1] = F[0] / c[0];

	for (int i = 1; i < n; i++) {

		double zn = c[i] + a[i] * m[i];
		m[i + 1] = (-1) * b[i] / zn;
		l[i + 1] = (F[i] - a[i] * l[i]) / zn;

	}

	// �������� ��������
	//cout << '\n' << F[n] << ' ' << a[n] << ' ' << l[n]
	//	<< ' ' << c[n] << ' ' << m[n];
	indexC[n] = (F[n] - a[n] * l[n]) / (c[n] + a[n] * m[n]);

	//cout << '\n' << "indexC[n] = " << indexC[n];

	for (int i = n - 1; i > -1; i--) {

		//cout << '\n' << "���������� indexC[i]" << m[i + 1] << ' ' << indexC[i + 1] << ' ' << l[i + 1];

		indexC[i] = m[i + 1] * indexC[i + 1] + l[i + 1];

	}

}

void solveMatrix(int n, double* a, double* c, double* b, double* f, double *& x)
{
	a[0] = 0;
	b[n] = 0;
	double m;
	for (int i = 1; i < n + 1; i++)
	{
		m = a[i] / c[i - 1];
		c[i] = c[i] - m * b[i - 1];
		f[i] = f[i] - m * f[i - 1];
	}

	x[n] = f[n - 1] / c[n - 1];

	for (int i = n - 1; i >= 0; i--)
	{
		x[i] = (f[i] - b[i] * x[i + 1]) / c[i];
	}
}

void findA(int n, double* yArray, double*& indexA) {

	for (int i = 1; i <= n; i++) {

		indexA[i] = yArray[i];

	}

}

void findDAndB(int n, double* yArray, double* indexC,
	double*& indexD, double*& indexB, double* h) {

	for (int i = 1; i <= n; i++) {

		//cout << "\n���������� D ��� ������ " << indexC[i] << ' ' 
		//	<< indexC[i - 1] << ' ' << h[i] << '\n';

		indexD[i] = (indexC[i] - indexC[i - 1]) / h[i];

		//cout << '\n' << "indexD[i]" << indexD[i];
		indexB[i] = h[i] * indexC[i] / 2 - h[i] * h[i] * indexD[i] / 6 
			+ (yArray[i] - yArray[i - 1]) / h[i];

	}

}

bool findArea(int n, double* xArray, double XX, int &area) {

	for (int i = 1; i <= n; i++) {

		if (xArray[i - 1] < XX && XX <= xArray[i]) {

			area = i;
			return true;

		}

	}

	return false;

}

// ���������� ������� 
void func(double x, ofstream& fout) {

	double solve = x * x * x + x * x + 1;
	cout << "\n�������� ������� � ����� XX = " << solve;
	fout << "\n�������� ������� � ����� XX = " << solve;

}

// ������������ �������
void func1(double x, ofstream& fout) {

	double solve = x * x - x;
	cout << "\n�������� ������� � ����� = " << solve;
	fout << "\n�������� ������� � ����� = " << solve;

}

// �������� ������� 
void func2(double x, ofstream& fout) {

	double solve = 10 * x + 3;
	cout << "\n�������� ������� � ����� = " << solve;
	fout << "\n�������� ������� � ����� = " << solve;

}

//����������� �������
void func3(double x, ofstream &fout) {

	cout << "\n�������� ������� � ����� = " << 61;
	fout << "\n�������� ������� � ����� = " << 61;

}

int main() {

	setlocale(LC_ALL, "rus");

	int n; // ����� ��������
	double *yArray, *xArray; //������ �������� ���������� � ������� 
	double A, B; //��������� ���������
	double XX; //�����, � ������� ����� ��������� f(x)

	ifstream fin;
	ofstream fout;

	fin.open("input.txt");
	fout.open("output.txt");

	if (checkInput(fin)) {

		if (readInput(fin, fout, n, xArray, yArray, A, B, XX) && IER(fout, n, xArray, yArray, A, B, XX)) {
			cout << "Success";
		}

	}
	else {

		fout << "���� �� ������";
		return 0;

	}

	double* up, * down, * center, *h, *F;
	up = new double[n]; // ������ ��� ������� ����������
	down = new double[n + 1]; // ������ ��� ������� ����������
	center = new double[n + 1]; // ������� ���������
	h = new double[n + 1]; // ����� ��������
	F = new double[n + 1]; // ������ ����� (�������)

	double* indexC, * indexB, * indexD, *indexA;
	indexB = new double[n + 1];
	indexD = new double[n + 1];
	indexC = new double[n + 1];
	indexA = new double[n + 1];

	fillH(xArray, h, n);
	fillMass(n, up, down, center, h);
	fillF(F, n, A, B, h, yArray);

	findC(n, down, center, up, F, indexC); // ��� � ��������� ������� 
	//solveMatrix(n, down, center, up, F, indexC);
	findA(n, yArray, indexA);			   // ���� ������������ �������
	findDAndB(n, yArray, indexC, indexD, indexB, h);

	cout << '\n' << "������������ ��������: \n";

	for (int i = 1; i <= n; i++) {

		cout << '\n' << "A[" << i << "] = " << indexA[i]
			<< ",  " << "B[" << i << "] = " << indexB[i]
			<< ",  " << "C[" << i << "] = " << indexC[i]
			<< ",  " << "D[" << i << "] = " << indexD[i] << '\n';
	}

	int area;
	if (findArea(n, xArray, XX, area)) {

		double YY;
		double a = indexA[area];
		double b = indexB[area];
		double c = indexC[area];
		double d = indexD[area];
		double x = xArray[area];

		YY = a + b * (XX - x) + c * (XX - x) * (XX - x) / 2 
			+ d * (XX - x) * (XX - x) * (XX - x) / 6;

		cout << "\n����� XX ������������� �� ������� " << area << '\n';

		cout << "\n�������� ������� � ����� XX = " << YY;
		fout << "\n�������� ������� � ����� XX = " << YY;

	}

	func(XX, fout);
	//func1(XX, fout);
	//func2(XX, fout);
	//func3(XX, fout);

	return 0;
}