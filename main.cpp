#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <fstream>
#include <algorithm>

using namespace std;

class BipartiteMatching {
public:
    // Конструктор, который принимает имя файла и загружает граф
    BipartiteMatching(const string& filename) {
        ReadGraph(filename);
        InitiMatch();
    }

    // Функция для выполнения поиска максимального паросочетания и вывода результата
    void PrintResult() {
        int max_matching = FindMaxMatching();
        cout << "Maximum matching size: " << max_matching << endl;

        for (const auto& [v, u] : match_) {
            if (u != -1) {
                cout << u << " " << v << endl;
            }
        }
    }
private:
    vector<pair<int, int>> graph_;
    map<int, int> match_;
    vector<bool> visited_;
    int n_;
    int m_;

    // Функция для чтения графа из файла
    void ReadGraph(const string& filename) {
        ifstream input(filename);
        input >> n_ >> m_;
        int u, v;
        while (input >> u >> v) {
            graph_.push_back({ u, v });
        }
        input.close();
    }

    // Функция для инициализации массива паросочетаний (изначально все вершины свободны)
    void InitiMatch() {
        for (const auto& [v, u] : graph_) {
            match_[u] = -1;
        }
    }

    // Функция для поиска увеличивающего пути
    bool FindAugmentingPath(int start) {
        stack<int> stk;
        stk.push(start);

        vector<int> parent(n_, -1);

        vector<pair<int, int>> current_edges;
        copy_if(graph_.begin(), graph_.end(), back_inserter(current_edges), [start](const pair<int, int>& edge) {
            return edge.first == start;
            });

        while (!stk.empty()) {
            int cur = stk.top();
            stk.pop();

            if (visited_[cur]) continue;
            visited_[cur] = true;

            for (const auto& [_, to] : current_edges) {
                if (match_[to] == -1) {
                    int u = cur;
                    int v = to;

                    while (u != -1) {
                        int next = match_[v];
                        match_[v] = u;
                        v = next;
                        u = parent[u];
                    }

                    return true;
                }

                if (!visited_[match_[to]]) {
                    parent[match_[to]] = cur;
                    stk.push(match_[to]);
                }
            }
        }

        return false;
    }

    // Функция для поиска максимального паросочетания
    int FindMaxMatching() {
        visited_.assign(n_, false);

        int max_matching = 0;
        for (const auto& [v, u] : graph_) {
            if (FindAugmentingPath(v)) ++max_matching;
        }

        return max_matching;
    }
};

int main() {
    BipartiteMatching matching("input.txt");
    matching.PrintResult();

    return 0;
}
