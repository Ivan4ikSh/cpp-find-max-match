#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <stack>
#include <vector>

class BipartiteMatching {
public:
    // Конструктор, который принимает имя файла и загружает граф
    BipartiteMatching(const std::string& filename) {
        ReadGraph(filename);
        InitiMatch();
    }

    // Функция для вывода результата
    void PrintResult(std::ostream& out) {
        out << "Maximum matching size: " << FindMaxMatching() << "\n";

        for (const auto& [v, u] : match_) {
            if (u != -1) {
                out << u << " " << v << "\n";
            }
        }
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
private:
    std::vector<std::pair<int, int>> graph_;
    std::map<int, int> match_;
    std::vector<bool> visited_;
    int n_;
    int m_;

    // Функция для чтения графа из файла
    void ReadGraph(const std::string& filename) {
        std::ifstream input(filename);
        input >> n_ >> m_;
        int u, v;
        while (input >> u >> v) {
            graph_.push_back({ u, v });
        }
        input.close();
    }

    // Функция для инициализации массива паросочетаний (изначально все вершины свободны)
    void InitiMatch() {
        for (const auto& [u, v] : graph_) {
            match_[v] = -1;
        }
    }

    // Функция для поиска увеличивающего пути
    bool FindAugmentingPath(int start) {
        std::stack<int> stk;
        stk.push(start);

        while (!stk.empty()) {
            int cur = stk.top();
            stk.pop();

            if (visited_[cur]) continue;
            visited_[cur] = true;

            for (const auto& [u, v] : graph_) {
                if (u == cur) {
                    if (match_[v] == -1) {
                        match_[v] = cur;
                        return true;
                    }
                    else if (!visited_[match_[v]]) {
                        stk.push(match_[v]);
                    }
                }
            }
        }

        return false;
    }

};

void LogExecutionTime(std::ostream& log, const std::string& filename) {
    int n = 10;
    int sum = 0;
    log << "File: " << filename << "\n";
    BipartiteMatching matching(filename);
    for (size_t i = 0; i < n; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        int cnt = matching.FindMaxMatching();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        log << duration.count() / 1000.0 << "\n";
        sum += duration.count() / 1000.0;
    }
    log << "Average time: " << sum / static_cast<double>(n) << "ms\n\n";
}

void TEST() {
    std::ofstream log("log.txt");
    LogExecutionTime(log, "time-test-files/test-1.txt");
    LogExecutionTime(log, "time-test-files/test-2.txt");
    LogExecutionTime(log, "time-test-files/test-3.txt");
    log.close();
}

int main() {
    std::cout << "Chose the output mode:\n";
    std::cout << "1. Output in console\n";
    std::cout << "2. Output in file\n";
    std::cout << "3. Log speed\n";
    int mode;
    std::cin >> mode;
    system("cls");
    
    std::string file_name;
    if (mode != 3) {
        std::cout << "Enter the input file name:\n";
        std::cin >> file_name;
        system("cls");
    }
    
    BipartiteMatching matching(file_name);

    switch (mode) {
    case 1:
        matching.PrintResult(std::cout);
        break;
    case 2:
        {
            std::cout << "Enter the output file name:\n";
            std::string out_file_name;
            std::cin >> out_file_name;

            std::ofstream output(out_file_name);
            matching.PrintResult(output);
            output.close();
            system("cls");
            std::cout << "Output in file " + out_file_name + " successful!\n";
        }
        break;
    case 3:
        TEST();
        std::cout << "Algoritm time duration logged in log.txt\n";
        break;
    default:
        std::cerr << "Invalid command\n";
        break;
    }

    return 0;
}
