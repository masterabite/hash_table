#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <locale>
#include <math.h>
#include <vector>
#include <fstream>

using namespace std;

//хеш-функция
int hashFunction(string key, int mod) {
	int hash = 0;	//наш хеш от ключа key по модулю mod
	int k = 1;		//простой множитель
	//перебираем все символы ключа
	for (size_t i = 0; i < key.size(); ++i) {

		if (key[i] >= '0' && key[i] <= '9') {		//если символ это цифра
			hash += (key[i] - '0') * k;
		}
		else if (key[i] >= 'A' && key[i] <= 'Z') {	//если символ это буква
			hash += (key[i] - 'A' + 10) * k;
		}
		k *= 11;
		hash %= mod; //берем по модулю
	}

	return hash;
}

//структура хеш-таблицы
struct hashTable {
	vector<string>* table;	//таблица представляет собой массив[N] списков(векторов)
	int size;				//размер таблицы (кол-во сегментов)

	hashTable(int _size) {
		cout << "Создание таблицы...\n";
		size = _size;
		table = new vector<string>[size];
	}

	//функция пытается добавить ключ key в таблицу
	int add(string key) {
		int hash = hashFunction(key, size); //получаем хеш от строки (он же индекс списка в массиве table)
		bool has = false;
		for (size_t i = 0; i < table[hash].size(); ++i) { //идем по списку этого хеша
			if (table[hash][i] == key) { //если нашли совпадение
				has = true;
				break;
			}
		}

		if (has) { //если элемент уже есть в таблице
			cout << "Элемент уже добавлен в таблицу.\n";
			return -1;
		}
		else {
			table[hash].push_back(key); //если совпадений не нашли, добавляем элемент в список
			return hash;
		}
	}

	//функция удаления ключа
	bool erase(string key) {
		cout << "Удаление ключа...\n";
		int i = find(key);
		if (i < 0) {
			return false;
		}
		for (size_t j = 0; j < table[i].size(); ++j) {
			if (table[i][j] == key) {
				table[i].erase(table[i].begin() + j);
				cout << "Ключ успешно удален.\n";
				return true;
			}
		}
	}
	

	//функция ищет элемент в хеш-таблице, возвращает хеш если элемент найден и -1 в противном случае
	int find(string key) {
		int hash = hashFunction(key, size); //получаем хеш от строки (он же индекс списка в массиве table)
		for (size_t i = 0; i < table[hash].size(); ++i) { //идем по списку этого хеша
			if (table[hash][i] == key) { //если нашли совпадение
				return hash;
				break;
			}
		}
		return -1; //если элемент не найден возвращаем -1
	}

	//функция ищет элементы, сегмента с номером	 num, возвращает список найденных элементов
	vector<string>& find(int num) {
		return table[(num-1)%size];
	}

	void save_to_file(string fname) {
		printf("Сохранение в файл \"%s\"...\n", fname.c_str());
		FILE* fout = fopen(fname.c_str(), "w");
		if (!fout) {
			cout << "Не удалось открыть файл для записи.\n";
			return;
		}
		fprintf(fout, "%s; %s\n", "segment", "count");
		for (int i = 0; i < size; ++i) {
			fprintf(fout, "%d; %d\n", i, table[i].size());
		}
		fclose(fout);
		printf("Таблица успешно сохранена в файл \"%s\".\n", fname.c_str());
	}

	void print() {
		cout << "Вывод таблицы:\n";
		for (int i = 0; i < size; ++i) {
			if (table[i].size() > 0) {
				cout << i+1 << "'й сегмент: ";
				for (size_t j = 0; j < table[i].size(); ++j) {
					cout << table[i][j] << ' ';
				}cout << '\n';
			}
		}
	}

};

//функция изменяет ключ на следующий в лексикографическом порядке
void next_key(string& key) {
	int i = key.size()-1;
	while (i > 0 && key[i] == '9' || key[i] == 'Z') {
		--i;
	}
	if (i >= 0) {
		++key[i];
	}

	for (i = i+1; i < key.size(); ++i) {
		if (key[i] >= '0' && key[i] <= '9') {
			key[i] = '0';
		}
		else {
			key[i] = 'A';
		}
	}
}


//заполняет хеш-таблцу случайными значениями
void hashTable_fill(hashTable& ht, string key_format) {
	cout << "Заполнение хеш-таблицы...\n";
	int n = 1; //количество всех возможных ключей
	string start_key = "";
	for (size_t i = 0; i < key_format.size(); ++i) {
		if (key_format[i] == 'ц') {
			n *= 10;			//кол-во элементов увеличивается в 10 раз
			start_key += '0';	//добавляем начальную цифру
		}
		else if (key_format[i] == 'Б') {
			n *= 26;			//кол-во возможных букв- 26
			start_key += 'A';	//добавляем начальную букву
		}
	}

	cout << "Добавление ключей...\n";

	//добавляем все возможные ключи в таблицу
	for (int i = 0; i < 8000; ++i) {
		ht.add(start_key);
		next_key(start_key);
	}
	cout << "Таблица успешно заполнена.\n";
}

//функция проверяет ключ на соответствие формату
bool check_format(string str, string format) {
	for (size_t i = 0; i < str.size(); ++i) {
		if (format[i] == 'ц') {
			if (str[i] < '0' || str[i] > '9') {
				return false;
			}
		} 
		else if (format[i] == 'Б') {
			if (str[i] < 'A' || str[i] > 'Z') {
				return false;
			}
		}
	}
	return true;
}

//функция пытается считывать ключ в формате format 
bool scan_key(string& key, string& format) {
	cout << "Введите ключ: "; cin >> key;
	if (!check_format(key, format)) {
		printf("Ключ не соответствует формату \"%s\"\n", format.c_str());
		return false;
	}
	return true;
}

int main()
{
	setlocale(LC_ALL, "Russian");

	//считываем размер очередей
	hashTable ht(1500);
	string format = "ББццББ", key;
	hashTable_fill(ht, format);

	int cmd;

	while (true) {
		cout << "\n\t0- завершение программы\n";
		cout << "\t1- найти ключ\n";
		cout << "\t2- добавить ключ\n";
		cout << "\t3- удалить ключ\n";
		cout << "\t4- просмотреть таблицу\n";
		cout << "\t5- сохранить в файл\n";
		cout << "Введите команду: "; cin >> cmd;
		if (cmd == 0) {
			break;
		}
		else if (cmd == 1) {
			cout << "\n\t0- по ключу\n";
			cout << "\t1- по номеру сегмента\n";
			cout << "Введите команду: "; cin >> cmd;
			if (cmd == 0) {
				if (!scan_key(key, format)) { continue; } //если не получилось считать ключ, пропускаем операцию

				int i = ht.find(key);
				if (i >= 0) {
					cout << "Элемент находится в таблице. Номер сегмента: " << i+1 << '\n';
				}
				else {
					cout << "Элемент не найден.\n";
				}
			}
			else {
				cout << "Введите номер сегмента: "; cin >> cmd;
				vector<string>& v = ht.find(cmd);
				if (v.size() == 0) {
					cout << "Соответствий не найдено.\n";
				}
				else {
					cout << "Найденные элементы: ";
					for (size_t i = 0; i < v.size(); ++i) {
						cout << v[i] << (i < v.size()-1? ", ": "\n");
					}
				}
			}
		}
		else if (cmd == 2) {
			if (!scan_key(key, format)) { continue; } //если не получилось считать ключ, пропускаем операцию
			int ind = ht.add(key); //добавляем ключ
			if (ind >= 0) {
				if (ht.table[ind].size() > 1) {
					printf("При добавление ключа %s возникли коллизии с элементами: ", key.c_str());
					for (size_t i = 0; i < ht.table[ind].size(); ++i) {
						if (key != ht.table[ind][i]) {
							cout << ht.table[ind][i] << ' ';
						}
					}cout << '\n';
				}
				cout << "Ключ успешно добавлен.\n";
			}
		} 
		else if (cmd == 3) {
			if (!scan_key(key, format)) { continue; } //если не получилось считать ключ, пропускаем операцию
			ht.erase(key);
		}
		else if (cmd == 4) {
			ht.print();
		}
		else if (cmd == 5) {
			ht.save_to_file("hashTable.csv");
		}
	}

}
