#include <iostream>
#include <vector>
#include <ctime>

#define UNDERLINE "\033[4m"
#define CLOSEUNDERLINE "\033[0m"

using std::cin;
using std::cout;
using std::endl;
using std::vector;

struct Square {
    int x;
    int y;
    int size;
};

vector<Square> minArrOfSquares;
int n;
int minNumOfSquares;

void init(vector<vector<int>> &square) {
    for(int i = 0; i < n; i++) {
        square.resize(n);
        for(int j = 0; j < n; j++) square[i].push_back(0);
    }
}

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

void deletePiece(vector<vector<int>> &square, vector<Square> &arrOfSquares) {
    Square tmp = *(arrOfSquares.rbegin());
    arrOfSquares.pop_back();
    for(int i = tmp.x; i < tmp.x + tmp.size; i++) {
        for(int j = tmp.y; j < tmp.y + tmp.size; j++) {
            square[i][j] = 0;
        }
    }

}

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

void reccFinding(vector<vector<int>> &square, int curSquare, int activeArea, int countOfSquares, vector<Square> &arrOfSquares) {
    if(countOfSquares + 1 == minNumOfSquares && activeArea > curSquare * curSquare) return;

    int flag = 0;
    for(int i = 0; i < n && !flag; i++) {
        for(int j = 0; j < n && !flag; j++) {
            if(square[i][j] == 0) {
                if(putPiece(square, i, j, curSquare)) {
                    flag = 1;
                    //cout << "Располагаем кусок с шириной " << curSquare << " в x = " << i << " и y = " << j << endl;
                    //printSquare(square);
                    arrOfSquares.push_back({i, j, curSquare});
                } else return;
            } else j += square[i][j] - 1;
        }
    }
    
    if(countOfSquares + 1 == minNumOfSquares) {
    	//cout << countOfSquares + 1 << " равно " << minNumOfSquares << endl;
        deletePiece(square, arrOfSquares);
        return;
    }
    
    if(activeArea == curSquare*curSquare && countOfSquares + 1 < minNumOfSquares) {
        minNumOfSquares = countOfSquares + 1;
        minArrOfSquares.assign(arrOfSquares.begin(), arrOfSquares.end());
        //cout << "Оптимальное расположение!" << endl;
        //printSquare(square);
        //Square tmp = *(arrOfSquares.rbegin());
        //cout << "Очищаем " << tmp.x << " " << tmp.y << " " << tmp.size << endl;
        deletePiece(square, arrOfSquares);
        //printSquare(square);
        return;
    }
    
    for(int i = n/2; i >= 1; i--) {
        if(i*i <= activeArea) {
        	//cout << "Пробуем " << i << " в цикле" << endl;
            reccFinding(square, i, activeArea - curSquare*curSquare, countOfSquares + 1, arrOfSquares);
        }
    }
    
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
	    
	    vector<vector<int>> square;
	    vector<Square> arrOfSquares;
	
	    init(square);
	    
	    vector<vector<int>> result;
	    init(result);
	    
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
	    
	    int activeArea = n*n;
	    minNumOfSquares = 2*n+1;
	    
	    putFirstThree(square, arrOfSquares, activeArea);
	    
	    time_t start = clock();
	    for(int i = n/2; i >= 1; i--) {
	    	//cout << "Пробуем " << i << endl;
	    	//printSquare(square);
	        reccFinding(square, i, activeArea, arrOfSquares.size(), arrOfSquares);
	    }
	    time_t end = (clock() - start) / CLOCKS_PER_SEC;
	    cout << "Time: " << end << " seconds" << endl;
	
	    cout << minNumOfSquares << endl;
	    for(auto& i : minArrOfSquares) {
	        cout << k*i.x + 1 << " " << k*i.y + 1 << " " << k*i.size << endl;
	        putPiece(result, k*i.x, k*i.y, k*i.size);
	    }
	
	    printSquare(result, k);
	}
    return 0;
}

