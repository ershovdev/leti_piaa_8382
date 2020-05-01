#include <iostream>
#include<sstream>
#include <map>
#include <vector>
#include <set>
#include <queue>

using namespace std;

// Компаратор для множества ребер
struct SetCompare
{
    bool operator()(pair<char, double> v1, pair<char, double> v2)
    {
        if (v1.second == v2.second)
            return v1.first < v2.first;
        return  v1.second < v2.second;
    }
};

// Компаратор для очереди с приоритетом
struct Compare
{
    bool operator()(pair<char, double> v1, pair<char, double> v2)
    {
        if (v1.second == v2.second)
            return v1.first < v2.first;
        return v1.second > v2.second;
    }
};

class Graph {
private:
    // map - отсортированный ассоциативный контейнер, порядок ключей - SetCompare
    // каждой вершине соответствует множество выходящих из нее ребер
    map <char, set<pair<char, double>, SetCompare>> ways; // список смежности

public:
    // set - ассоциативный контейнер пар (pair) с компаратором SetCompare
    // Возвращает контейнер с вершинами, к которым ведет way
    set<pair<char, double>, SetCompare>& operator[](char peak)
    {
        return ways[peak];
    }

    string greedySearch(char start, char finish)
    {
        map <char, bool> checked; // контейнер с просмотренными вершинами
        string res; // итоговый путь

        cout << "Жадный алгоритм, промежуточный путь:" << endl;

        res.push_back(start); // сразу добавляем стартовую вершину
        checked[start] = true; // просмотрели стартовую
        char curr = start; // текущая - стартовая

        cout << res << endl;

        do {
            bool path_from_current = false; // флаг, указывающий на то, есть ли путь из текущей вершины в другую
            pair <char, double> temp; // смежная к curr, с минимальным весом

            // Рассматриваем первую смежную вершину
            if (!ways[curr].empty()) {
                for (auto it = ways[curr].begin(); !path_from_current && it != ways[curr].end(); it++) {
                    temp = *it;
                    if (!checked[it->first]) path_from_current = true;
                }
            }

            // Если не нашли смежных вершин - откатываемся
            if (!path_from_current) {
                cout << res << endl;
                res.pop_back();
                curr = *res.rbegin();
            } else { // Нашли - меняем текущую на найденную
                curr = temp.first;
                res.push_back(curr);
                cout << res << endl;
                checked[curr] = true;
            }
        } while (curr != finish && !res.empty()); // Выполняем, пока текущая не станет конечной

        return res;
    }

    void print_queue_and_peaks(
            priority_queue <pair<char, double>, vector<pair<char, double>>, Compare> const queue,
            vector <pair<char, double>> const peaks) {
        cout << "-------" << endl;
        priority_queue <pair<char, double>, vector<pair<char, double>>, Compare> tmp = queue;
        cout << "Очередь: ";
        while (!tmp.empty()) {
            cout << tmp.top().first << " ";
            tmp.pop();
        }
        cout << endl;
        cout << "Мультипоточный контейнер: ";
        for (auto peak : peaks) {
            cout << peak.first << " ";
        }
        cout << endl << "-------" << endl;
    }

    string aStarSearch(char start, char finish, int n = 1)
    {
        priority_queue <pair<char, double>, vector<pair<char, double>>, Compare> queue; // Очередь с приоритетом
        map <char, string> path_from_start; // Контейнер с путями от стартовой до char
        map <char, double> gX;  // Контейнер со стоимостями достижения вершин (g(h))

        path_from_start[start] = start;
        cout << "A*, промежуточный путь:" << endl;
        cout << "Путь из " << start << ": " << path_from_start[start] << endl;
        gX[start] = 0;
        cout << "Стоимость достижения " << start << ": " << 0 << endl;
        queue.push(*gX.begin());
        cout << "Вставляем " << (*gX.begin()).first << " в очередь" << endl;

        while (!queue.empty())
        {
            vector <pair<char, double>> peaks; // n верхних элементов очереди, для "мультипоточности"

            auto curr = queue.top(); // Вытаскиваем верх очереди и проверяем на равенство конечной вершине
            if (curr.first == finish) {
                cout << "Дошли до конечной вершины!" << endl;
                return path_from_start[finish];
            }

            for (int i = 0; i < n && !queue.empty(); i++) {
                curr = queue.top();
                if (curr.first == finish) continue;
                queue.pop();
                cout << "(" << curr.first << ") очередь -> мультипоточный контейнер" << endl;
                peaks.push_back(curr);

                print_queue_and_peaks(queue, peaks);
            }

            cout << endl;

            for (auto &peak : peaks) { // Обрабатываем все снятые с очереди вершины
                cout << "Рассмотрим " << peak.first << " из мультипоточного контейнера" << endl;
                for (auto &w : ways[peak.first]) { // Обрабатываем смежные к peak вершины
                    cout << "Рассмотрим смежную к " << peak.first << " вершину " << w.first << endl;
                    double new_gX = gX[peak.first] + w.second; // Новая стоимость достижения для текущей смежной вершины
                    cout << "Стоимость достижения " << w.first << ": " << new_gX << endl;

                    // Если вес уменьшился или он еще не вычислен
                    if (new_gX < gX[w.first] || !gX[w.first]) {
                        cout << "Новый приоритет для " << w.first << ": " << new_gX + abs(finish - w.first) << endl;
                        cout << "Стоимость достижения, было: " << gX[w.first] << ", ";
                        cout << "стало: " << new_gX << endl;
                        gX[w.first] = new_gX; // Обновляем стоимость достижения
                        cout << "Путь, было: " << path_from_start[w.first] << ", ";
                        cout << "стало: " << path_from_start[peak.first] + w.first << endl;
                        path_from_start[w.first] = path_from_start[peak.first] + w.first; // Обновляем путь

                        // Пушим в очередь с приоритетом вершину
                        queue.push({w.first, new_gX + abs(finish - w.first)});

                        print_queue_and_peaks(queue, peaks);
                    }
                }
            }
        }

        return {}; // Если очередь пустая - возвращаем пустой путь
    }
};

int main() {
    Graph graph;
    int n;
    char startPeak, finishPeak;
    stringstream input_stream;
    string input;

    cout << "Сколько вершин извлекать из очереди с приоритетами?" << endl;
    getline(cin, input);
    input_stream.str(input);

    input_stream >> n;

    getline(cin, input);
    startPeak = input[0];
    finishPeak = input[2];

    while (getline(cin, input)) {
        if (input.empty()) break;
        graph[input[0]].emplace(input[2], stod(input.substr(4)));
    }

    string greedy = graph.greedySearch(startPeak, finishPeak);
    cout << greedy << endl;

    string a_star = "A*: " + graph.aStarSearch(startPeak, finishPeak, n);
    cout << a_star << endl;

    return 0;
}