#include <iostream>
#include <omp.h>
#include <vector>
#include <cmath>
#include <chrono>
using namespace std;

// Exercício 1 — “Hello World” Paralelo
void exercicio1() {
    cout << "===== EXERCÍCIO 1 =====" << endl;

    omp_set_num_threads(4); // Configura 4 threads

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int total = omp_get_num_threads();
        cout << "Olá do thread " << tid << " de " << total << endl;
    }

    cout << endl;
}

// Exercício 2 — Paralelizando um for simples
void exercicio2() {
    cout << "===== EXERCÍCIO 2 =====" << endl;

    const int N = 100;
    vector<int> v(N, 1);

    // Versão sequencial
    int soma_seq = 0;
    for (int i = 0; i < N; i++) soma_seq += v[i];

    // Versão paralela com reduction
    int soma_par = 0;
    #pragma omp parallel for reduction(+:soma_par)
    for (int i = 0; i < N; i++) soma_par += v[i];

    cout << "Soma sequencial: " << soma_seq << endl;
    cout << "Soma paralela:   " << soma_par << endl;
    cout << "Reduction é necessária pois evita condição de corrida, garantindo que cada thread contribua de forma segura à variável 'soma'." << endl;
    cout << endl;
}

// Exercício 3 — Expressão Vetorial
void exercicio3() {
    cout << "===== EXERCÍCIO 3 =====" << endl;

    const int N = 1000000;
    vector<double> x(N, 1.0), y(N, 2.0), z(N, 3.0), a(N, 0.0);

    // Versão sequencial
    auto t1 = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++)
        a[i] = x[i]*x[i] + y[i]*y[i] + z[i]*z[i];
    auto t2 = chrono::high_resolution_clock::now();
    double tempo_seq = chrono::duration<double>(t2 - t1).count();

    // Versão paralela
    t1 = chrono::high_resolution_clock::now();
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++)
        a[i] = x[i]*x[i] + y[i]*y[i] + z[i]*z[i];
    t2 = chrono::high_resolution_clock::now();
    double tempo_par = chrono::duration<double>(t2 - t1).count();

    cout << "Tempo sequencial: " << tempo_seq << " s" << endl;
    cout << "Tempo paralelo:   " << tempo_par << " s" << endl;
    cout << endl;
}

// Exercício 4 — Medindo tempo por thread
void exercicio4() {
    cout << "===== EXERCÍCIO 4 =====" << endl;

    const int N = 1000000;
    vector<double> x(N, 1.0), y(N, 2.0), z(N, 3.0), a(N, 0.0);

    int num_threads = 0;
    double tempo_total_inicio = omp_get_wtime();

    #pragma omp parallel
    {
        double inicio = omp_get_wtime();
        #pragma omp for schedule(static)
        for (int i = 0; i < N; i++)
            a[i] = x[i]*x[i] + y[i]*y[i] + z[i]*z[i];
        double fim = omp_get_wtime();

        int tid = omp_get_thread_num();
        #pragma omp single
        num_threads = omp_get_num_threads();

        cout << "Thread " << tid << " executou em " << fim - inicio << " s" << endl;
    }

    double tempo_total_fim = omp_get_wtime();
    cout << "Tempo total: " << tempo_total_fim - tempo_total_inicio << " s" << endl;
    cout << "Total de threads: " << num_threads << endl;
    cout << endl;
}

// Exercício 5 — Escalonamento
void exercicio5() {
    cout << "===== EXERCÍCIO 5 =====" << endl;

    const int N = 1000000;
    vector<double> x(N, 1.0), y(N, 2.0), z(N, 3.0), a(N, 0.0);

    int configs[] = {2, 4, 8};

    for (int t : configs) {
        omp_set_num_threads(t);

        auto inicio = chrono::high_resolution_clock::now();
        #pragma omp parallel for schedule(static)
        for (int i = 0; i < N; i++)
            a[i] = x[i]*x[i] + y[i]*y[i] + z[i]*z[i];
        auto fim = chrono::high_resolution_clock::now();
        double tempo_static = chrono::duration<double>(fim - inicio).count();

        inicio = chrono::high_resolution_clock::now();
        #pragma omp parallel for schedule(dynamic, 1000)
        for (int i = 0; i < N; i++)
            a[i] = x[i]*x[i] + y[i]*y[i] + z[i]*z[i];
        fim = chrono::high_resolution_clock::now();
        double tempo_dynamic = chrono::duration<double>(fim - inicio).count();

        cout << "Threads: " << t << " | static: " << tempo_static << " s | dynamic(1000): " << tempo_dynamic << " s" << endl;
    }

    cout << "\nO escalonamento static é melhor quando todas as iterações têm custo semelhante (trabalho balanceado)." << endl;
    cout << "Já o dynamic é vantajoso quando o custo das iterações varia, pois redistribui o trabalho entre as threads." << endl;
    cout << endl;
}

// Função principal
int main() {
    exercicio1();
    exercicio2();
    exercicio3();
    exercicio4();
    exercicio5();
    return 0;
}
