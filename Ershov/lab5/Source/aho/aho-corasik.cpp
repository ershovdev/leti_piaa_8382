#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <queue>
#include <unordered_map>

using namespace std;

class TreeNode {
public:
    explicit TreeNode(char val) : value(val) {} // Конструктор ноды

    // Отладочная функция для печати бора
    void printTrie() {
        cout << "Бор сейчас:" << endl;

        queue<TreeNode *> queue;
        queue.push(this);

        while (!queue.empty()) {
            auto curr = queue.front();
            if (!curr->value)
                cout << "Корень:" << endl;
            else
                cout << curr->dbgStr << ':' << endl;

            if (curr->suffixLink)
                cout << "\tСуффиксная ссылка: " << (curr->suffixLink == this ? "Root" : curr->suffixLink->dbgStr) << endl;

            if (curr->parent && curr->parent->value)
                cout << "\tРодитель: " << curr->parent->dbgStr << endl;
            else if (curr->parent)
                cout << "\tРодитель: Корень" << endl;

            if (!curr->children.empty()) cout << "\tПотомок: ";
            for (auto child : curr->children) {
                cout << child.second->value << ' ';
                queue.push(child.second);
            }

            queue.pop();
            cout << endl;
        }
        cout << endl;

    }

    // Вставка подстроки в бор
    void insert(const string &str) {
        auto curr = this;
        static size_t countPatterns = 0;

        for (char c : str) { // Идем по строке
            // Если из текущей вершины по текущему символу не было создано перехода
            if (curr->children.find(c) == curr->children.end()) {
                // Создаем переход
                curr->children[c] = new TreeNode(c);
                curr->children[c]->parent = curr;
                curr->children[c]->dbgStr += curr->dbgStr + c;
            }
            // Спускаемся по дереву
            curr = curr->children[c];
        }

        cout << "Вставляем строку: " << str << endl;
        printTrie();

        // Показатель терминальной вершины, значение которого равно порядковому номеру добавления шаблона
        curr->numOfPattern = ++countPatterns;
    }

    // Функция для поиска подстроки в строке при помощи автомата
    vector<size_t> find(const char c) {
        static const TreeNode *curr = this; // Вершина, с которой необходимо начать следующий вызов
        cout << "Ищем '" << c << "' из: " << (curr->dbgStr.empty() ? "Корень" : curr->dbgStr) << endl; // Дебаг

        for (; curr != nullptr; curr = curr->suffixLink) {
            // Обходим потомков, если искомый символ среди потомков не найден, то
            // переходим по суффиксной ссылке для дальнейшего поиска
            for (auto child : curr->children)
                if (child.first == c) { // Если символ потомка равен искомому
                    curr = child.second; // Значение текущей вершины переносим на этого потомка
                    vector<size_t> found; // Вектор номеров найденных терм. вершин

                    // ИНДИВИДУАЛИЗАЦИЯ
//                    if (curr->numOfPattern) { // Для пропуска пересечений, после нахождения терминальной вершины
//                        found.push_back(curr->numOfPattern - 1); // Добавляем к найденным эту вершину
//                        curr = this; // И переходим в корень
//                    }
                    // НЕ ИНДИВИДУАЛИЗАЦИЯ
                    // Обходим суффиксы, т.к. они тоже могут быть терминальными вершинами
                    for (auto temp = curr; temp->suffixLink; temp = temp->suffixLink)
                        if (temp->numOfPattern)
                            found.push_back(temp->numOfPattern - 1);
                    //

                    cout << "Символ '" << c << "' найден!" << endl; // Дебаг
                    return found;
                }

            if (curr->suffixLink) {
                cout << "Переходим по суффиксной ссылке: ";
                cout << (curr->suffixLink->dbgStr.empty() ? "Корень" : curr->suffixLink->dbgStr) << endl;
            }
        }
        cout << "Символ '" << c << "' не найден!" << endl; // Дебаг

        curr = this;
        return {};
    }

    // Функция для построения недетерминированного автомата
    void makeAutomaton() {
        cout << "Строим автомат: " << endl;

        queue<TreeNode *> queue; // Очередь для обхода в ширину

        for (auto child : children) // Заполняем очередь потомками корня
            queue.push(child.second);

        while (!queue.empty()) {
            auto curr = queue.front(); // Обрабатываем верхушку очереди

            // Для дебага
            cout << curr->dbgStr << ':' << endl;
            if (curr->parent && curr->parent->value)
                cout << "\tРодитель: " << curr->parent->dbgStr << endl;
            else if (curr->parent)
                cout << "\tРодитель: Корень" << endl;

            if (!curr->children.empty())
                cout << "\tПотомок: ";
            //

            // Заполняем очередь потомками текущей верхушки
            for (auto child : curr->children) {
                cout << child.second->value << ' '; // Дебаг
                queue.push(child.second);
            }

            // Дебаг
            if (!curr->children.empty())
                cout << endl;

            queue.pop();
            auto p = curr->parent; // Ссылка на родителя обрабатываемой вершины
            char x = curr->value; // Значение обрабатываемой вершины
            if (p) p = p->suffixLink; // Если родитель существует, то переходим по суффиксной ссылке

            // Пока можно переходить по суффиксной ссылке или пока
            // не будет найден переход в символ обрабатываемой вершины
            while (p && p->children.find(x) == p->children.end())
                p = p->suffixLink; // Переходим по суффиксной ссылке

            // Суффиксная ссылка для текущей вершины равна корню, если не смогли найти переход
            // в дереве по символу текущей вершины, иначе равна найденной вершине
            curr->suffixLink = p ? p->children[x] : this;
            // Дебаг
            cout << "\tСуффиксная ссылка: " << (curr->suffixLink == this ? "Корень" : curr->suffixLink->dbgStr) << endl << endl;
        }

        // Дебаг
        cout << endl;
        printTrie();
    }

    ~TreeNode() { // Деструктор ноды
        for (auto child : children) delete child.second;
    }

private:
    string dbgStr = ""; // Для отладки
    char value; // Значение ноды
    size_t numOfPattern = 0; // Номер введенного паттерна
    TreeNode *parent = nullptr; // Родитель ноды
    TreeNode *suffixLink = nullptr; // Суффиксная ссылка
    unordered_map <char, TreeNode*> children; // Потомок ноды
};

class Trie {
public:
    Trie() : root('\0') {} // Конструктор бора

    void insert(const string &str) { root.insert(str); }
    auto find(const char c) { return root.find(c); }
    void makeAutomaton() { root.makeAutomaton(); }

private:
    TreeNode root; // Корень бора
};

auto AhoCorasick(const string &text, const vector <string> &patterns)
{
    Trie bor;
    set <pair<size_t, size_t>> result;

    for (const auto &pattern : patterns) // Заполняем бор введенными паттернами
        bor.insert(pattern);

    bor.makeAutomaton(); // Из полученного бора создаем автомат (путем добавления суффиксных ссылок)

    for (size_t j = 0; j < text.size(); j++) // Проходим циклом по строке, для каждого символа строки запускаем поиск
        for (auto pos : bor.find(text[j])) // Проходим по всем найденным позициям, записываем в результат
            result.emplace(j - patterns[pos].size() + 2, pos + 1);

    return result;
}

int main()
{
    system("chcp 65001");

    string text;
    size_t n;
    cin >> text >> n;

    vector <string> patterns(n);

    for (size_t i = 0; i < n; i++)
        cin >> patterns[i];

    auto res = AhoCorasick(text, patterns);
    for (auto r : res)
        cout << r.first << ' ' << r.second << endl;

    return 0;
}