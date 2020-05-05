#include <iostream>
#include <climits>
#include <set>
#include <algorithm>
#include <vector>

using namespace std;

// Нода для хранения прямого и дополнительно ребра в остаточной сети
struct Node {
    int f1;
    int f2;

    Node() : f1(0), f2(0) {}
    Node(int f1, int f2) : f1(f1), f2(f2){}
};

vector<bool> viewed; // Посещенные вершины
int startPeak, finishPeak; // Старт и финиш
vector<vector<Node>> network(128, vector<Node>(128)); // Остаточная сеть
vector<pair<int, int>> graph; // Граф

// Поиск дополняющего пути
int recFindPath(int v, int delta) {
    char tmp_peak = static_cast<char>(v);
    cout << "Рассматриваем " << tmp_peak << endl;
    if (viewed[v]) { // Уже просмотренна, выходим
        cout << tmp_peak << " уже просмотренна!" << endl;
        return 0;
    }
    viewed[v] = true;

    // Конечная вершина, нашли дополняющий путь, возвращаемся обратно и
    // применяем найденную минимальную пропускную способность
    if (v == finishPeak) {
        cout << "Дошли до конечной, путь найден!" << endl;
        return delta;
    }

    set<pair<int, int>, greater<>> sorted; // сет для сортировки вершин по пропускной способности

    // Ищем все смежные вершины
    cout << "Ищем все пути из " << tmp_peak << ", которые можно использовать" << endl;
    for (size_t u = 0; u < network[v].size(); u++) {
        if (!viewed[u] && network[v][u].f2 > 0) {
            sorted.insert(make_pair(network[v][u].f2, u));
            cout << "Нашли " << static_cast<char>(u) << "(" << network[v][u].f2 << ") (f2)" << endl;
        }
        if (!viewed[u] && network[v][u].f1 > 0) {
            sorted.insert(make_pair(network[v][u].f1, u));
            cout << "Нашли " << static_cast<char>(u) << "(" << network[v][u].f1 << ") (f1)" << endl;
        }
    }

    cout << "Вершины сейчас: ";
    for (auto item : sorted) {
        cout << static_cast<char>(item.second) << "(" << item.first << ") ";
    }
    cout << endl;

    for(auto u : sorted) { // Обходим все смежные вершины в порядке приоретета
        if (network[v][u.second].f2 > 0) { // Если дополнительное ребро у пути больше 0

            // После нахождения пути, здесь будет максимальное возможная величина протекающего потока
            // Рекурсивный поиск пути
            int new_flow_delta = recFindPath(u.second, min(delta, network[v][u.second].f2));

            if (new_flow_delta > 0){
                // Применяем минимальную пропускную способность к пути
                cout << "network[" << static_cast<char>(u.second) << "][" << static_cast<char>(v) <<"].f1 = " << network[u.second][v].f1 << " -> ";
                network[u.second][v].f1 += new_flow_delta;
                cout << "network[" << static_cast<char>(u.second) << "][" << static_cast<char>(v) <<"].f1 = " << network[u.second][v].f1 << ";" << endl;
                cout << "network[" << static_cast<char>(v) << "][" << static_cast<char>(u.second) <<"].f2 = " << network[v][u.second].f2 << " -> ";
                network[v][u.second].f2 -= new_flow_delta;
                cout << "network[" << static_cast<char>(v) << "][" << static_cast<char>(u.second) <<"].f2 = " << network[v][u.second].f2 << "; " << endl;

                return new_flow_delta;
            }
        }

        if (network[v][u.second].f1 > 0) { // Если прямое ребро у пути больше 0

            // После нахождения пути, здесь будет максимальное возможная величина протекающего потока
            // Рекурсивный поиск пути
            int new_flow_delta = recFindPath(u.second, min(delta, network[v][u.second].f1));

            if (new_flow_delta > 0) {
                // Применяем минимальную пропускную способность к пути
                cout << "network[" << static_cast<char>(u.second) << "][" << static_cast<char>(v) <<"].f2 = " << network[u.second][v].f2 << " -> ";
                network[u.second][v].f2 += new_flow_delta;
                cout << "network[" << static_cast<char>(u.second) << "][" << static_cast<char>(v) <<"].f2 = " << network[u.second][v].f2 << ";" <<endl;
                cout << "network[" << static_cast<char>(v) << "][" << static_cast<char>(u.second) <<"].f1 = " << network[v][u.second].f1 << " -> ";
                network[v][u.second].f1 -= new_flow_delta;
                cout << "network[" << static_cast<char>(v) << "][" << static_cast<char>(u.second) <<"].f1 = " << network[v][u.second].f1 << ";"<<endl;

                return new_flow_delta;
            }
        }

    }
    return 0;
}

// Печать результата
void print(){
    sort(graph.begin(), graph.end());
    for(size_t i = 0; i < graph.size(); i++) { // Вывод ребер графа и количества пущенного тока
        cout << static_cast<char>(graph[i].first) << ' ' << static_cast<char>(graph[i].second) << ' ' << network[graph[i].second][graph[i].first].f2 << endl;
    }
}

// Функция для поиска максимального потока
void findMaximumFlow(){
    int flow, ans;
    ans = 0;
    while (true){
        // Очищаем просмотренные
        fill(viewed.begin(), viewed.end(), false);

        // Ищем путь
        flow = recFindPath(startPeak, INT_MAX);

        // Поток равный нулю или INT_MAX, не было найденно дополняющего пути и был найден максимальный поток
        if (flow == 0 || flow == INT_MAX) {
            cout << "Путь не найден, завершаем работу " << endl;
            break;
        } else {
            cout << "Поток увеличился на " << flow << "; ";
            ans +=flow;
            cout << "Максимальный поток сейчас: " << ans << endl;
        }
    }

    cout << ans << endl;
    print();
}

int main() {
    system("chcp 65001");

    int num_of_ribs;
    char u, v;
    int capacity;

    viewed.resize(128);

    cin >> num_of_ribs;

    cin >> u >> v;
    startPeak = static_cast<int>(u);
    finishPeak = static_cast<int>(v);

    int k = 0;
    while (k++ < num_of_ribs) {
        cin >> u >> v >> capacity;
        int i = static_cast<int>(u);
        int j = static_cast<int>(v);

        graph.emplace_back(i, j);
        network[i][j].f1 = capacity;
    }

    findMaximumFlow();
    return 0;
}