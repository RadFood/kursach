#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <io.h>
#include <stdio.h>
#include <fstream>
#include <iomanip>
#include <windows.h>
#include <string>
#include <limits>

using namespace std;

struct Station
{
	char arrival_st[50];
	char time_out[10];
	char time_in[10];
	int num;
	double cost;
};

void Create(FILE* file);
void ViewDoc(FILE* file);
void Delete(FILE* file);
void Info(Station* s);

bool checknum(const string& str) {
	for (char c : str) {
		if (c < '0' || c > '9') {
			return false;
		}
	}
	return true;
}

bool checktime(const string& time) {
	if (time.size() != 5) return false;
	if (time[2] != ':') return false;
	for (int i = 0; i < 5; ++i) {
		if (i == 2) continue;
		if (time[i] < '0' || time[i] > '9') return false;
	}
	int hours = (time[0] - '0') * 10 + (time[1] - '0');
	int minutes = (time[3] - '0') * 10 + (time[4] - '0');
	if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59) return false;
	return true;
}

void Create(FILE* file)
{
	file = fopen("station.txt", "ab");
	cout << "Введите номер такси, пункт назначения, время прибытия, время отбытия и стоимость поездки\n";
	Station s;
	bool a = false;
	while (!a) {
		string taxiNumStr;
		cout << "Номер такси: ";
		cin >> taxiNumStr;
		if (!checknum(taxiNumStr)) {
			cout << "Ошибка: Номер такси должен быть числом.\n";
			continue;
		}
		s.num = stoi(taxiNumStr);
		a = true;
	}
		cout << "Пункт назначения: ";
		cin.ignore(); 
		cin.getline(s.arrival_st, 50);
	
	while (a) {
		string arrivalTimeStr;
		cout << "Время прибытия (HH:MM): ";
		cin >> arrivalTimeStr;
		if (!checktime(arrivalTimeStr)) {
			cout << "Ошибка: Неверный формат времени.\n";
			continue;
		}
		strcpy(s.time_in, arrivalTimeStr.c_str());
		a = false;
	}
	while (!a) {
		string departureTimeStr;
		cout << "Время отбытия (HH:MM): ";
		cin >> departureTimeStr;
		if (!checktime(departureTimeStr)) {
			cout << "Ошибка: Неверный формат времени.\n";
			continue;
		}
		strcpy(s.time_out, departureTimeStr.c_str());
		a = true;
	}
	while (a) {
		string costStr;
		cout << "Стоимость поездки: ";
		cin >> costStr;
		if (!checknum(costStr)) {
			cout << "Ошибка: Стоимость поездки должна быть числом.\n";
			continue;
		}
		s.cost = stof(costStr);
		a = false;
	}
	fwrite(&s, sizeof(Station), 1, file);
	fclose(file);
}

void ViewDoc(FILE* file)
{
	file = fopen("station.txt", "rb");
	Station s;
	int kolvo = _filelength(_fileno(file)) / sizeof(Station);
	for (int i = 0; i < kolvo; i++)
	{
		fread(&s, sizeof(Station), 1, file);
		Info(&s);
	}

	fclose(file);
}
void Info(Station* s)
{
	cout << " Номер такси: " << s->num
		<< ", Пункт назначения: " << s->arrival_st
		<< ", Время прибытия: " << s->time_in
		<< ", Время отбытия: " << s->time_out
		<< ", Стоимость: " << s->cost << " руб." << endl << endl;
}

void Delete(FILE* file)
{
	file = fopen("station.txt", "r+b");
	int kolvo = _filelength(_fileno(file)) / sizeof(Station);
	int n;
	ViewDoc(file);
	cout << "\nВведите номер такси: ";
	cin >> n;
		Station s;
		fseek(file, (n - 1) * sizeof(Station), 0);
		fread(&s, sizeof(Station), 1, file);

		for (int i = n; i < kolvo; i++)
		{
			Station s2;
			fread(&s2, sizeof(Station), 1, file);
			fseek(file, (i - 1) * sizeof(Station), 0);
			fwrite(&s2, sizeof(Station), 1, file);
		}

		kolvo--;

		int result = _chsize(_fileno(file), kolvo * sizeof(Station));
		if (result != 0) {
			cout << "Ошибка при изменении размера файла." << endl;
		}
	fclose(file);
}

int LinSearch(Station* arr, int n, char* time_out) {
	for (int i = 0; i < n; i++) {
		if (strcmp(arr[i].time_out, time_out) == 0) {
			return i;
		}
	}
	return -1;
}

void SelectionSort(Station* arr, int n) {
	for (int i = 0; i < n - 1; i++) {
		int min_idx = i;
		for (int j = i + 1; j < n; j++) {
			if (strcmp(arr[j].time_out, arr[min_idx].time_out) < 0) {
				min_idx = j;
			}
		}
		if (min_idx != i) {
			Station temp = arr[i];
			arr[i] = arr[min_idx];
			arr[min_idx] = temp;
		}
	}
}

int Parts(Station* arr, int low, int high) {
	Station pivot = arr[high];
	int i = low - 1;
	for (int j = low; j <= high - 1; j++) {
		if (strcmp(arr[j].time_out, pivot.time_out) < 0) {
			i++;
			Station temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
		}
	}
	Station temp = arr[i + 1];
	arr[i + 1] = arr[high];
	arr[high] = temp;
	return i + 1;
}

void QuickSort(Station* arr, int low, int high) {
	if (low < high) {
		int pi = Parts(arr, low, high);
		QuickSort(arr, low, pi - 1);
		QuickSort(arr, pi + 1, high);
	}
}

int BinarySearch(Station* arr, int low, int high, char* key) {
	int result = -1;
	while (low <= high) {
		int mid = low + (high - low) / 2;
		if (strcmp(arr[mid].time_out, key) >= 0) {
			result = mid;
			high = mid - 1;
		}
		else {
			low = mid + 1;
		}
	}
	return result;
}


void PrintAfterTime(FILE* file, char* time) {
	file = fopen("station.txt", "rb");
	int size = _filelength(_fileno(file)) / sizeof(Station);
	Station* arr = new Station[size];
	for (int i = 0; i < size; i++) {
		fread(&arr[i], sizeof(Station), 1, file);
	}

	QuickSort(arr, 0, size - 1);

	int index = BinarySearch(arr, 0, size - 1, time);
	if (index != -1) {
		for (int i = index; i < size; i++) {
			if (strcmp(arr[i].time_out, time) >= 0) {
				Info(&arr[i]);
			}
		}
	}
	else {
		cout << "Нет доступных рейсов после данного времени." << endl;
	}

	delete[] arr;
	fclose(file);
}

void SortData(FILE* file, bool useQuickSort) {
	file = fopen("station.txt", "rb+");
	int size = _filelength(_fileno(file)) / sizeof(Station);
	Station* arr = new Station[size];
	for (int i = 0; i < size; i++) {
		fread(&arr[i], sizeof(Station), 1, file);
	}
	if (useQuickSort) {
		QuickSort(arr, 0, size - 1);
	}
	else {
		SelectionSort(arr, size);
	}
	rewind(file);
	for (int i = 0; i < size; i++) {
		fwrite(&arr[i], sizeof(Station), 1, file);
	}
	delete[] arr;
	fclose(file);
	cout << "Данные отсортированы." << endl;
}

void SearchByTime(FILE* file) {
	char time[10];
	bool a = false;
	while (!a)
	{
		cout << "Введите время отправления для поиска (в формате HH:MM): ";
		cin >> time;
		if (!checktime(time)) {
			cout << "Ошибка: Неверный формат времени.\n";
			continue;
		}
		a = true;
	}
	file = fopen("station.txt", "rb");
	int size = _filelength(_fileno(file)) / sizeof(Station);
	Station* arr = new Station[size];
	for (int i = 0; i < size; i++) {
		fread(&arr[i], sizeof(Station), 1, file);
	}
	int result = LinSearch(arr, size, time);
	if (result != -1) {
		Info(&arr[result]);
	}
	else {
		cout << "Такси с таким временем отправления не найдено." << endl;
	}
	delete[] arr;
	fclose(file);
}

void AskForTimeAndShow(FILE* file) {
	char time[10];
	bool a = false;
	while (!a)
	{
		cout << "Введите время (HH:MM), после которого нужно найти рейсы: ";
		cin >> time;
		if (!checktime(time)) {
			cout << "Ошибка: Неверный формат времени.\n";
			continue;
		}
		a = true;
	}
	PrintAfterTime(file, time);
}

void Menu()
{
	FILE* file = fopen("station.txt", "wb");
	fclose(file);
	bool menu = true;
	while (menu)
	{
		cout << "1. Добавить такси в файл" << endl;
		cout << "2. Просмотреть такси" << endl;
		cout << "3. Удалить такси" << endl;
		cout << "4. Сортировать данные (QuickSort)" << endl;
		cout << "5. Сортировать данные (Selection Sort)" << endl;
		cout << "6. Поиск по времени отправления" << endl;
		cout << "7. Вывести рейсы после заданного времени" << endl;
		cout << "8. Выйти из программы" << endl;
		int n;
		cin >> n;
		switch (n)
		{
		case 1:
			Create(file);
			break;
		case 2:
			ViewDoc(file);
			break;
		case 3:
			Delete(file);
			break;
		case 4:
			SortData(file, true);
			break;
		case 5:
			SortData(file, false); 
			break;
		case 6:
			SearchByTime(file);
			break;
		case 7:
			AskForTimeAndShow(file);
			break;
		case 8:
			menu = false;
			break;
		default:
			cout << "Неверный ввод" << endl;
		}
	}
}

int main()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	Menu();
}