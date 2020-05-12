#include <iostream>
#include <vector>
#include <cstdio>

using namespace std;

// Класс для "склеивания" строк
class FullString {
    const string &smp;
    const string &dst;
public:
    // Конструктор
    FullString(const string &sample, const string &dest) : smp(sample), dst(dest) {}

    // Доступ к "склеенной" строке через оператор []
    char operator[](size_t i) {
        if (i < smp.size()) {
//            cout << "p[" << i << "]: " << sbstr[i] << endl;
            return smp[i];
        }
//        cout << "t[(" << i << " - " << sbstr.size() << ") % " << dst.size() << "]: " << dst[(i - sbstr.size()) % dst.size()] << endl;
        return dst[(i - smp.size()) % dst.size()];
//        return t[(i - p.size()) % t.size()];
    }
};

class PrefixFunction {
    FullString full_str; // Склеенные sample и dest
    size_t sample_size; // Размер образца
    size_t dst_index; // Текущий индекс строки
    vector<size_t> prefixes; // Значения префикс-функции для образца
public:
    // Функция, рассчитывающая значения префикс-фукнции для образца
    PrefixFunction(const string& sample, const string& dest) : full_str(sample, dest),
                                                               sample_size(sample.size()),
                                                               dst_index(sample.size())
    {
        cout << "Вычисляем значения префикс-функции для образца" << endl;
        prefixes.resize(sample_size); // Выделяем память под размер паттерна
        int i = 0;
        cout << sample.substr(0, i+1) << ": 0" << endl;
        prefixes[i++] = 0; // Для первого элемента значение префикс-функции - 0

        while (i < sample_size) {
            auto k = prefixes[i - 1]; // Берем предыдущее значение

            while (k > 0 && sample[i] != sample[k]) // Пока конец суффикса != концу префикса
                k = prefixes[k - 1]; // Сохраняем предыдущее значение префикс функции

            // Конец префикса == концу суффикса - увеличиваем предыдущее значение на 1, иначе обнуляем
            prefixes[i] = (sample[i] == sample[k]) ? k + 1 : 0;
            cout << sample.substr(0, i+1) << ": " << prefixes[i] << endl;
            i++;
        }
        cout << endl;
    }

    size_t nextValue() {
        static size_t sample_index = 0; // static - сохраняет свое значение после выхода из блока

        if (full_str[dst_index] != full_str[sample_index]) {
            cout << "string_index: " << dst_index << ", sample_index: " << sample_index << "; ";
            cout << full_str[dst_index] << "!=" <<  full_str[sample_index] << endl;
            cout << "Ищем новую позицию для продолжения поиска" << endl;
        }
        while (sample_index > 0 && full_str[dst_index] != full_str[sample_index]) {// Пока конец суффикса != концу префикса
            cout  << "sample_index - было: " << sample_index << ", стало: ";
            int tmp = sample_index - 1;
            sample_index = prefixes[sample_index - 1]; // // Сохраняем предыдущее значение префикс функции
            cout << sample_index << " (prefixes[" << tmp << "])" << endl;
            if (full_str[dst_index] == full_str[sample_index]) {
                cout << "string_index: " << dst_index << ", sample_index: " << sample_index << "; ";
                cout << full_str[dst_index] << "==" <<  full_str[sample_index] << endl;
                cout << "Позиция найдена, продолжаем!" << endl;
                cout << endl;
            }
        }
        if (full_str[dst_index] != full_str[sample_index]) {
            cout << "Вернулись в начало образца, продолжаем поиски" << endl;
            cout << endl;
        }

        // Конец префикса == концу суффикса - увеличиваем предыдущее значение на 1
        if (full_str[dst_index] == full_str[sample_index]) {
            cout << "string_index: " << dst_index << ", sample_index: " << sample_index << "; ";
            cout << full_str[dst_index] << "==" << full_str[sample_index] << endl;
            sample_index++;
        }
        dst_index++;

        // Дошли до конца образца - вхождение найдено!
        if (sample_index == sample_size) {
            cout << "Вхождение найдено!" << endl;
            cout << "sample_prefix = " << prefixes[sample_index - 1] << " (prefixes[" << sample_index - 1 << "])";
            cout << endl;
            sample_index = prefixes[sample_index - 1];
            return sample_size;
        }

        cout << "Символы совпали, проверяем следующие" << endl;
        return sample_index;
    }
};

vector<size_t> getIndices(const string &sample, const string &dest) {
    PrefixFunction prefixFunction(sample, dest);
    vector<size_t> result;

    const string str = sample + dest;

    int i = 0;
    cout << "Начинаем искать индексы вхождений" << endl;
    while (i < dest.size()) {
        size_t sub_position = prefixFunction.nextValue();

        // Если дошли до конца образца - вхождение найдено, пушим в result
        if (sub_position == sample.size())
            result.push_back(i - sample.size() + 1); // i - pi(k) + 1

        i++;
    }

    return result;
}


int getIndexShifted(const string &a, const string &b)
{
    if (a.size() != b.size()) return -1;
    PrefixFunction prefixFunction(a, b);

    const string str = a + b + b;

    int i = 0;
    while (i < 2*b.size()) {
        size_t sub_position = prefixFunction.nextValue();

        // Дошли до конца A - значит, B является циклическим сдвигом A, возвращаем индекс
        if (sub_position == a.size()) return int(i - a.size() + 1); // i - pi(k) + 1
        i++;
    }

    return -1;
}

int main()
{
    system("chcp 65001");

    string p, t;
    cin >> p >> t;

    auto result = getIndices(p, t);
//    auto result_shift = getIndexShifted(t, p);

    if (result.empty()) cout << -1;
    else {
        int i = 0;
        while (i < result.size())
            cout << result[i] << ((++i != result.size()) ? ',' : ' ');

    }
//    cout << result_shift << endl;

    return 0;
}