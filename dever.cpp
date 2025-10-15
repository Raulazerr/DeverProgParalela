#include <iostream>
#include <omp.h>
#include <vector>
#include <cmath>
#include <chrono>
using namespace std;

// Exercício 1 — “Hello World” Paralelo
void exercicio1() {
    cout << "===== EXERCICIO 1 =====" << endl;

    // Define manualmente o número de threads que serão usadas
    omp_set_num_threads(4);

    // Cria uma região paralela: tudo dentro das chaves será executado por várias threads
    #pragma omp parallel
    {
        // Cada thread executa este bloco e obtém seu identificador único
        int tid = omp_get_thread_num();      // Retorna o ID da thread atual
        int total = omp_get_num_threads();   // Retorna o total de threads ativas

        // Cada thread imprime sua própria mensagem
        cout << "Ola do thread " << tid << " de " << total << endl;
    }

    cout << endl;
}

// Exercício 2 — Paralelizando um for simples
void exercicio2() {
    cout << "===== EXERCICIO 2 =====" << endl;

    const int N = 100;
    vector<int> v(N, 1); // Cria um vetor com 100 elementos, todos valendo 1

    // Versão sequencial (sem paralelismo)
    int soma_seq = 0;
    for (int i = 0; i < N; i++)
        soma_seq += v[i];

    // Versão paralela com redução (soma compartilhada entre threads)
    int soma_par = 0;
    #pragma omp parallel for reduction(+:soma_par)
    for (int i = 0; i < N; i++)
        soma_par += v[i];

    // Mostra resultados
    cout << "Soma sequencial: " << soma_seq << endl;
    cout << "Soma paralela:   " << soma_par << endl;
    cout << endl;

    //A diretiva reduction evita condições de corrida, pois cria cópias locais da variável 'soma' e depois soma todas no final.
}

// Exercício 3 — Expressão Vetorial
void exercicio3() {
    cout << "===== EXERCICIO 3 =====" << endl;

    const int N = 1000000; // Um milhão de elementos
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

    // Mostra os tempos
    cout << "Tempo sequencial: " << tempo_seq << " s" << endl;
    cout << "Tempo paralelo:   " << tempo_par << " s" << endl;
    cout << endl;
}

// Exercício 4 — Medindo tempo por thread
void exercicio4() {
    cout << "===== EXERCICIO 4 =====" << endl;

    const int N = 1000000;
    vector<double> x(N, 1.0), y(N, 2.0), z(N, 3.0), a(N, 0.0);

    int num_threads = 0;
    double tempo_total_inicio = omp_get_wtime(); // Marca o início total do programa

    // Região paralela
    #pragma omp parallel
    {
        double inicio = omp_get_wtime(); // Marca o início do trabalho da thread

        // Cada thread processa uma parte do vetor
        #pragma omp for schedule(static)
        for (int i = 0; i < N; i++)
            a[i] = x[i]*x[i] + y[i]*y[i] + z[i]*z[i];

        double fim = omp_get_wtime(); // Marca o fim da thread
        int tid = omp_get_thread_num();

        // Apenas uma thread executa esta linha para obter o total
        #pragma omp single
        num_threads = omp_get_num_threads();

        // Cada thread imprime quanto tempo levou
        cout << "Thread " << tid << " executou em " << fim - inicio << " s" << endl;
    }

    double tempo_total_fim = omp_get_wtime();
    cout << "Tempo total: " << tempo_total_fim - tempo_total_inicio << " s" << endl;
    cout << "Total de threads: " << num_threads << endl;
    cout << endl;
}

// Exercício 5 — Escalonamento
void exercicio5() {
    cout << "===== EXERCICIO 5 =====" << endl;

    const int N = 1000000;
    vector<double> x(N, 1.0), y(N, 2.0), z(N, 3.0), a(N, 0.0);

    // Testar com diferentes quantidades de threads
    int configs[] = {2, 4, 8};

    for (int t : configs) {
        omp_set_num_threads(t); // Define a quantidade de threads

        // Escalonamento STATIC — divide o trabalho igualmente entre as threads
        auto inicio = chrono::high_resolution_clock::now();
        #pragma omp parallel for schedule(static)
        for (int i = 0; i < N; i++)
            a[i] = x[i]*x[i] + y[i]*y[i] + z[i]*z[i];
        auto fim = chrono::high_resolution_clock::now();
        double tempo_static = chrono::duration<double>(fim - inicio).count();

        // Escalonamento DYNAMIC — distribui blocos de 1000 elementos dinamicamente
        inicio = chrono::high_resolution_clock::now();
        #pragma omp parallel for schedule(dynamic, 1000)
        for (int i = 0; i < N; i++)
            a[i] = x[i]*x[i] + y[i]*y[i] + z[i]*z[i];
        fim = chrono::high_resolution_clock::now();
        double tempo_dynamic = chrono::duration<double>(fim - inicio).count();

        // Mostra os tempos comparativos
        cout << "Threads: " << t
             << " | static: " << tempo_static << " s"
             << " | dynamic(1000): " << tempo_dynamic << " s" << endl;
        cout << endl;
    }

    //O escalonamento 'static' é melhor quando as iterações têm custo semelhante.
    //O 'dynamic' é mais eficiente quando cada iteração leva tempos diferentes, pois redistribui o trabalho em tempo real.
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
