#include <iostream>
#include <vector>
#include <random>
using namespace std;

//Генератор случайных чисел
random_device rd;
mt19937 gen(rd());
uniform_real_distribution<> dis(0.0, 1.0);


//Муравьиный алгоритм
void antColonyOptimization(vector<vector<int>> matrixOfWays, int ants, int iters) {
    int alpha = 1, beta = 2, iterations = iters, countOfAnts = ants, countOfPoints = (int)matrixOfWays.size(), bestDistance = INT_MAX;
    double intensity = 0.5;
    vector<int> bestPath(countOfPoints);

    //Инициализация феромонов
    vector<vector<double>> pheromoneMatrix(countOfPoints, vector<double>(countOfPoints));
    for (int i = 0; i < countOfPoints; i++) {
        for (int j = 0; j < countOfPoints; j++) {
            pheromoneMatrix[i][j] = 0.1; //помечаем каждый путь феромоном с весом 0.1
        }
    }
    vector<vector<int>> antPaths(countOfAnts, vector<int>(countOfPoints)); //создаем двумерный массив (муравьи Х пути)
    for (int iter = 0; iter < iterations; iter++) { // Цикл итераций
        for (auto& el1 : antPaths) { //обнуление путей
            for (auto& el2 : el1) {
                el2 = 0;
            }
        }
        //Перемещение муравьев
        for (int k = 0; k < countOfAnts; k++) { //делаем с каждым муравьем следующее:
            int startPoint = rand() % countOfPoints; //выдаем ему рандомную точку (высаживаем муравья в нее)
            vector<bool> visited(countOfPoints); //создаем массив посещенных вершин
            visited[startPoint] = 1; //помечаем вершину, куда высадили муравья, посещенной для этого муравья
            antPaths[k][0] = startPoint; //в двумерном массиве путей у муравья помечаем его стартовую точку, куда его высадили (в индекс 0)
            for (int i = 0; i < countOfPoints - 1; i++) { //цикл на количество оставшихся точек для муравья
                double total = 0.0;
                for (int j = 0; j < countOfPoints; j++) {
                    if (!visited[j]) { //если вершина еще не была посещена муравьем
                        total += pow(pheromoneMatrix[startPoint][j], alpha) * pow(1.0 / matrixOfWays[startPoint][j], beta); //получаем знаменатель формулы, используя феромены непосещенных вершин
                    }
                }
                double randValue = dis(gen); //получаем рандомное число от 0.0 до 1.0
                double probability = 0.0; //объявляем вероятность перехода из вершины i в вершину j
                int nextPoint = -1;
                for (int j = 0; j < countOfPoints; j++) { //цикл прохода по всем непосещенным вершинам с вычислением следующей (ее probability выщитывается по формуле ниже)
                    if (visited[j] == 0) {
                        probability += pow(pheromoneMatrix[startPoint][j], alpha) * pow(1.0 / matrixOfWays[startPoint][j], beta) / total; //вычисляем для каждой непосещенной вершины
                        if (randValue <= probability) { //если вероятность выше рандомного числа от 0.0 до 1.0,
                            nextPoint = j; //то эта точка становится следующей в пути для муравья
                            break;
                        }
                    }
                }
                visited[nextPoint] = 1; //следующая точка помечается как посещенная
                antPaths[k][i + 1] = nextPoint; //следующая точка помещается в путь муравья в таблице путей муравьев 
                startPoint = nextPoint; //смещаемся на другую точку
            }
        }

        //Вычисление длины пути каждого муравья
        vector<double> antDistances(countOfAnts); //создаем массив длины путей всех муравьев
        for (int k = 0; k < countOfAnts; k++) { //проходимся по каждому муравью
            double distance = 0.0;
            for (int i = 0; i < countOfPoints - 1; i++) {
                distance += matrixOfWays[antPaths[k][i]][antPaths[k][i + 1]]; //складываем длину всех путей в его маршруте
            }
            antDistances[k] = distance; //закидываем в наш массив длины путей путь конкретного муравья
        }

        //Обновление феромонов
        for (int k = 0; k < countOfAnts; k++) {
            for (int i = 0; i < countOfPoints - 1; i++) {
                pheromoneMatrix[antPaths[k][i]][antPaths[k][i + 1]] *= (1.0 - intensity);
                pheromoneMatrix[antPaths[k][i]][antPaths[k][i + 1]] += intensity / antDistances[k];
            }
        }
        
        //находим лучший путь в данной итерации и сверяем, лучший ли он за все итерации (если да, то присваиваем)
        for (int k = 0; k < countOfAnts; k++) {
            int distance = 0;
            for (int i = 0; i < countOfPoints - 1; i++) {
                distance += matrixOfWays[antPaths[k][i]][antPaths[k][i + 1]];
            }
            if (distance < bestDistance) {
                bestDistance = distance;
                bestPath = antPaths[k];
            }
        }
    }

    //Вывод лучшего пути
    cout << "Best path: ";
    for (int i = 0; i < countOfPoints; i++) {
        cout << bestPath[i] + 1 << " ";
    }
    cout << endl;
    cout << "Best distance: " << bestDistance << endl;
}

vector<vector<int>> makeMatrix() {
    int countOfPoints;
    cout << "Enter the number of points: ";
    cin >> countOfPoints;
    vector<vector<int>> matrixOfWays(countOfPoints, vector<int>(countOfPoints));
    for (int i = 0; i < countOfPoints; ++i) {
        for (int j = i + 1; j < countOfPoints; ++j) {
            int distance;
            cout << "Enter the distance between points " << i + 1 << " and " << j + 1 << ": ";
            cin >> distance;
            matrixOfWays[i][j] = distance;
            matrixOfWays[j][i] = distance;
        }
    }
    cout << "Your matrix is: " << endl;
    for (const auto& el1 : matrixOfWays) {
        for (const auto& el2 : el1) {
            cout << el2 << " ";
        }
        cout << endl;
    }
    return matrixOfWays;
}

int main() {
    int countOfAnts, iterations;
    cout << "Enter count of ants and count of iterations: ";
    cin >> countOfAnts >> iterations;
    antColonyOptimization(makeMatrix(), countOfAnts, iterations);
    return 0;
}