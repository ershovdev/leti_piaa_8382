#include <iostream>
#include <vector>
#include <ctime>

#define UNDERLINE "\033[4m"
#define CLOSEUNDERLINE "\033[0m"

using std::cin;
using std::cout;
using std::endl;
using std::vector;

// Структура для хранения расположения и размера обрезка
struct Square {
    int x;
    int y;
    int size;
};

vector<Square> minArrOfSquares;
int n;
int minNumOfSquares;

// Инициализация "поля", в котором размещаются обрезки
void init(vector<vector<int>> &square) {
    for(int i = 0; i < n; i++) {
        square.resize(n);
        for(int j = 0; j < n; j++) square[i].push_back(0);
    }
}

// Вспомогательная функция для печати столешницы в данный момент времени
void printSquare(vector<vector<int>> &square, int k = 1) {
	int r = 3;
	int flag = 1;
    for(int i = 0; i < n*k; i++) {
        for(int j = 0; j < n*k; j++) {
            cout.width(r);
            if (square[i][j + 1] != square[i][j]) {
            	cout << square[i][j] << "|";
				if (flag) {
					r--;
					flag = 0;
				}
			} else {
				cout << square[i][j];
				r = 3;
				flag = 1;	
			}
            cout.width(r);
        }
        cout << endl;
        r = 3;
    }
    cout << endl;
}

// Удаление обрезка с поля
void deletePiece(vector<vector<int>> &square, vector<Square> &arrOfSquares) {
    Square tmp = *(arrOfSquares.rbegin());
    arrOfSquares.pop_back();
    for(int i = tmp.x; i < tmp.x + tmp.size; i++) {
        for(int j = tmp.y; j < tmp.y + tmp.size; j++) {
            square[i][j] = 0;
        }
    }

}

// Размещение обрезка на поле
int putPiece(vector<vector<int>> &square, int x, int y, int size) {
	if(n-x < size || n-y < size) return 0;
	
    for(int i = x; i < x+size; i++) {
        for(int j = y; j < y+size; j++) {
            if(square[i][j] != 0) return 0;
        }
    }
    
    for(int i = x; i < x+size; i++) {
        for(int j = y; j < y+size; j++) {
            square[i][j] = size;
        }
    }
    
    return 1;
}

// Функция для оптимизации работы программы (распологает 3 обрезка на поле, которые всегда являются оптимальными)
void putFirstThree(vector<vector<int>> &square, vector<Square> &arrOfSquares, int& activeArea) {
    arrOfSquares.push_back({0, 0, (n+1)/2});
    putPiece(square, 0, 0, (n+1)/2);
    activeArea -= ((n+1)/2)*((n+1)/2);

    arrOfSquares.push_back({0, (n+1)/2, n/2});
    putPiece(square, 0, (n+1)/2, n/2);
    activeArea -= (n/2)*(n/2);

    arrOfSquares.push_back({(n+1)/2, 0, n/2});
    putPiece(square, (n+1)/2, 0, n/2);
    activeArea -= (n/2)*(n/2);
}

// Рекурсивное нахождение оптимального расположения
void reccFinding(vector<vector<int>> &square, int curSquare, int activeArea, int countOfSquares, vector<Square> &arrOfSquares) {
    // Если в текущей ситуации становится понятно, что оптимального расположения не достичь - возвращаемся на предыдущий уровень рекурсии
    if(countOfSquares + 1 == minNumOfSquares && activeArea > curSquare * curSquare) return;

    int flag = 0;

    // В первое пустое место распологаем обрезок
    for(int i = 0; i < n && !flag; i++) {
        for(int j = 0; j < n && !flag; j++) {
            if(square[i][j] == 0) {
                if(putPiece(square, i, j, curSquare)) {
                    flag = 1;
                    //cout << "Располагаем кусок с шириной " << curSquare << " в x = " << i << " и y = " << j << endl;
                    //printSquare(square);
                    arrOfSquares.push_back({i, j, curSquare}); // Записываем обрезок в массив расположенных обрезков
                } else return;
            } else j += square[i][j] - 1; // Перескакиваем к конец ряда столешницы
        }
    }

    // Удаляем, если в текущей ситуации уже не получится оптимального расположения
    if(countOfSquares + 1 == minNumOfSquares) {
    	//cout << countOfSquares + 1 << " равно " << minNumOfSquares << endl;
        deletePiece(square, arrOfSquares);
        return;
    }

    // Оптимальное расположение
    if(activeArea == curSquare*curSquare && countOfSquares + 1 < minNumOfSquares) {
        // Обновляем минимальное количество обрезков
        minNumOfSquares = countOfSquares + 1;
        // Обновляем массив с оптимальными обрезками
        minArrOfSquares.assign(arrOfSquares.begin(), arrOfSquares.end());
        //cout << "Оптимальное расположение!" << endl;
        //printSquare(square);
        //Square tmp = *(arrOfSquares.rbegin());
        //cout << "Очищаем " << tmp.x << " " << tmp.y << " " << tmp.size << endl;
        // Удаляем последний обрезок для того, чтобы попробовать другие способы размещения в следующих вызовах функции
        deletePiece(square, arrOfSquares);
        //printSquare(square);
        return;
    }

    // Пробуем все размерности обрезков в следующих рекурсивных вызовах
    for(int i = n/2; i >= 1; i--) {
        if(i*i <= activeArea) { // Пробуем, только если обрезок меньше оставшейся области для размещения
        	//cout << "Пробуем " << i << " в цикле" << endl;
            reccFinding(square, i, activeArea - curSquare*curSquare, countOfSquares + 1, arrOfSquares);
        }
    }

    // В конце функции, в любом случае удаляем обрезок для того, чтобы пробовать другие расположения
    deletePiece(square, arrOfSquares);
}

int main() {
	while(true) {

	    cin >> n;
	    int k = 1;
	    
	    if((n < 2) || (n > 50)) {
	        cout << "Error!" << endl;
	        return 0;
	    }
	    
	    vector<vector<int>> square; // Столешница
	    vector<Square> arrOfSquares; // Массив обрезков
	
	    init(square);
	    
	    vector<vector<int>> result; // Результат
	    init(result);

	    // Оптимизация перед выполнением - сводим к более простым случаям
	    if (n % 2 == 0) {
	        k = n/2;
	        n = 2;
		}
		if(n % 3 == 0) {
	        k = n/3;
	        n = 3;
		}
		if(n % 5 == 0) {
	        k = n/5;
	        n = 5;
	    }

		// Оставшаяся область для вставки обрезков
	    int activeArea = n*n;
	    minNumOfSquares = 2*n+1; // Начальное минимальное количество обрезков
	    
	    putFirstThree(square, arrOfSquares, activeArea);
	    
	    time_t start = clock();
	    // Запускаем функцию c размером начального обрезка от n/2 до 1
	    for(int i = n/2; i >= 1; i--) {
	    	//cout << "Пробуем " << i << endl;
	    	//printSquare(square);
	        reccFinding(square, i, activeArea, arrOfSquares.size(), arrOfSquares);
	    }
	    time_t end = (clock() - start) / CLOCKS_PER_SEC;
	    cout << "Time: " << end << " seconds" << endl;
	
	    cout << minNumOfSquares << endl;
	    for(auto& i : minArrOfSquares) { // Вывод, домножаем на k, чтобы из упрощенных случаев вернуть обратно к начальным
	        cout << k*i.x + 1 << " " << k*i.y + 1 << " " << k*i.size << endl;
	        putPiece(result, k*i.x, k*i.y, k*i.size);
	    }
	
	    printSquare(result, k);
	}
    return 0;
}

