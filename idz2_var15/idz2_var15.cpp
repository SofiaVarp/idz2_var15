#include <iostream>
#include <iomanip>
#include <random>
#include <fstream>
using namespace std;

const int MAX_ROWS = 20;
const int MAX_COLS = 20; 
const int OK = 0;

void Menu() {
    cout << "|----------MENU----------|" << endl;
    cout << "|" << "1. Create a table       " << "|" << endl; //1. Создать таблицу
    cout << "|" << "2. Fill in the table    " << "|" << endl; //2. Заполнить таблицу
    cout << "|" << "3. Show the table       " << "|" << endl; //3. Показать таблицу
    cout << "|" << "4. Process the table    " << "|" << endl; //4. Обработать таблицу
    cout << "|" << "5. Show the result      " << "|" << endl; //5. Показать результат
    cout << "|" << "6. Save the table value " << "|" << endl; //6. Сохранить значение таблицы в файл
    cout << "|" << "7. Finish the job       " << "|" << endl; //7. Закончить работу
    cout << "|------------------------|" << endl;
}

int** createTable(int rows, int cols) {
    int** table = new int* [rows]; //создаем массив из элементов строк, где каждый элемент - указатель на строку
    for (int i = 0; i < rows; i++) {
        table[i] = new int[cols](); // для каждой строки массив из чисел количества столбцов 
    }
    return table;
}

void deleteTable(int** table, int rows) { 
    if (!table) {
        return;
    }
    for (int i = 0; i < rows; i++) {
        delete[] table[i]; 
    }
    delete[] table; //удаляем мссив указателе
}

void Input_manual(int** table, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            while (true) {
                cout << "table[" << i << "][" << j << "] = ";
                if (cin >> table[i][j]) {
                    break;
                }
                else {
                    cout << "Incorrect input. Enter a number.\n";
                    cin.clear();
                    cin.ignore(10000, '\n');
                }
            }
        }
    }
}

void Random_manual(int** table, int rows, int cols) { //случайный ввод
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (j % 2 == 0) {
                table[i][j] = rand() % 1000;
            } 
            else {
                table[i][j] = (rand() % 1000)*(-1);
            }
        }
    }
}

void File_manual(int**& table, int& rows, int& cols) {
    string filename;
    cout << "Enter file name: ";
    cin >> filename;
    ifstream fin(filename);
    if (!fin.is_open()) {
        cout << "Error file opening" << endl;
        return;
    }

    int newrows, newcols;
    fin >> newrows >> newcols;
    if (newrows <= 0 || newrows > MAX_ROWS || newcols <= 0 || newcols > MAX_COLS) {
        cout << "Invalid table size in file" << endl;
        return;
    }

    if (table != nullptr) {
        deleteTable(table, rows);
    }
    rows = newrows;
    cols = newcols;
    table = createTable(rows, cols);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (!(fin >> table[i][j])) {
                cout << "Erorr, not enough numbers in file" << endl;
                fin.close();
                return;
            }
        }
    }
    fin.close();
}

void Save_file(int** table, int rows, int cols) {
    string filename;
    cout << "Enter filename to save: ";
    cin >> filename;
    if (table == nullptr) {
        cout << "Table is not created" << endl;
        return;
    }
    
    ofstream fout(filename);
    if (!fout.is_open()) {
        cout << "Error saving file" << endl;
        return;
    }
    fout << rows << " " << cols << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fout << table[i][j] << " ";
        }
        fout << endl;
    }
    fout.close();
}

int Processing(int** table, int rows, int cols) {  //Изменить таблицу, заменив нулями элементы тех строк и столбцов,которые содержат отрицательные числа.
    bool* zeroRows = new bool[rows];
    bool* zeroCols = new bool[cols];

    if (table == nullptr) {
        cout << "Table is not created" << endl;
        return -1;
    }

    for (int j = 0; j < cols; j++) {
        zeroCols[j] = false;
    }

    for (int i = 0; i < rows; i++) {
        zeroRows[i] = false;
        for (int j = 0; j < cols; j++) {
            if (table[i][j] < 0) {
                zeroCols[j] = true;
                zeroRows[i] = true;
            }
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (zeroCols[j] || zeroRows[i]) {
                table[i][j] = 0;
            }
        }
    }

    delete[] zeroRows;
    delete[] zeroCols;
    return OK;
}

bool Read(int& element) {
    string s;
    cin >> s;
    int start = 0;
    if (s[0] == '-') {
        start = 1;
        if (s.size() == 1) {
            return false;
        }
    }
    for (int i = start; i < s.size(); i++) {
        if (s[i] < '0' || s[i]>'9') {
            return false;
        }
    }
    element = 0;
    for (int i = start; i < s.size(); i++) {
        element = element * 10 + (s[i] - '0');
    }
    if (start == 1) {
        element = -element;
    }
    return true;
}
   
int main() {
    int** table = nullptr; //таблица еще не создана
    int rows = 0;
    int cols = 0;
    bool work = true;
    int choice;
    while (work) {
        Menu();
        while (!Read(choice)) {
            cout << "Incorrect input.Enter a number: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }
        switch (choice) {
            case 1: { //1. Создать таблицу
                cout << "Enter the number of lines: "; 
                while (!Read(rows)||rows<=0||rows>MAX_ROWS) {
                    cout << "Incorrect value of the rows parameters, please re-enter: ";
                    cin.clear();
                    cin.ignore(10000, '\n');
                }
                cout << "Enter the number of cols: ";
                while (!Read(cols)||cols<=0||cols>MAX_COLS) {
                    cout << "Incorrect value of the cols parameters, please re-enter: ";
                    cin.clear();
                    cin.ignore(10000, '\n');
                }
                if (table != nullptr) {
                    deleteTable(table, rows);
                }
                table = createTable(rows, cols);
                cout << "The table has been created in the size of " << rows << " by " << cols << endl;
                break;
            }

            case 2: { //заполнение таблицы
                int yr;
                bool success = false;
                if (table == nullptr) {
                    cout << " First create a table" << endl;
                    break;
                }
                while (!success) {
                    cout << "Do you want to fill out the table yourself or do it randomly? Enter 1 if by yourself, 2 if by accident, 3 if from file: ";
                    if (!Read(yr)) {
                        cout << "Incorrect input. Enter 1 or 2 or 3." << endl;
                        cin.clear();
                        cin.ignore(1000, '\n');
                        continue;
                    }
                    if (yr == 1) {
                        Input_manual(table, rows, cols);
                        success = true;
                    }
                    else if (yr == 2) {
                        Random_manual(table, rows, cols);
                        cout << "The table has been filled" << endl;
                        success = true;
                    }
                    else if (yr == 3) {
                        File_manual(table, rows, cols);
                        success = true;
                    }
                    else if (yr != 1 && yr != 2 && yr != 3) {
                        cout << "Incorrect input,enter 1 or 2 or 3" << endl;
                        cin.clear();
                        cin.ignore(10000, '\n');
                    }
                }
                break;
            }

            case 3: { // Показать таблицу
                if (table == nullptr) {
                    cout << "First create a table" << endl;
                }
                else {
                    for (int i = 0; i < rows; i++) {
                        for (int j = 0; j < cols; j++) {
                            cout << setw(4) << table[i][j] << " ";
                        }
                        cout << endl;
                    }
                }
                break;
            }

            case 4: { //Обработать таблицу
                if (table == nullptr) {
                    cout << " First create a table" << endl;
                    break;
                }
                cout << "Changing the table by replacing the elements of those rows and columns that contain negative numbers with zeros." << endl;
                int res=Processing(table, rows, cols);
                if (res == OK) {
                    cout << "Processing completed" << endl;
                    break;
                }
                else {
                    cout << "Processing error" << endl;
                }
                break;
            }

            case 5: { //Показать результат
                if (table == nullptr) {
                    cout << " First create a table" << endl;
                    break;
                }
                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < cols; j++) {
                        cout << setw(4) << table[i][j] << " ";
                    }
                    cout << endl;
                }
                break;
            }

            case 6: {
                if (table == nullptr) {
                    cout << " First create a table" << endl;
                    break;
                }
                Save_file(table, rows, cols);
                break;
            }

            case 7: { //Закончить работу
                deleteTable(table, rows);
                work = false;
                break;
            }

            default: {
                cout <<"There is no such item"<< endl;
                break;
            }    
        }    
    }
}


//int Printer(int** table, int rows, int cols) { //вывод
    //for (int i = 0; i < rows; i++) {
       // for (int j = 0; j < cols; j++) {
            //cout << setw(4) << table[i][j] << " ";
        //}
        //cout << endl;
    //}
   // return OK;
//}