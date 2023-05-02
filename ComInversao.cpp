/* Esta versão do jantar dos filosofos apresenta a solução do problema
 * apresentado no outro código. Esta solução apresenta uma inversão da captura
 * do garfo do primeiro filosofo, onde ao inves de pegar o da esqueda depois
 * direita, pega o da direita e depois o da esquerda. A fim de evitar o impasse
 * por espera circular.
 */

#include <semaphore.h>
#include <unistd.h>

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

const int N = 5;  // Número de filósofos
sem_t garfo[N];   // Array de semáforos para representar os garfos

void medita() {
  sleep(0.5);  // Thread dorme por 0.5 segundos
}

void filosofo(int i) {
  int garfoEsq = i;
  int garfoDir = (i + 1) % N;
  while (true) {
    medita();
    if (i == 0) {
      // Primeiro filósofo pega os garfos em ordem inversa
      sem_wait(&garfo[garfoDir]);  // Bloqueia o garfo à direita do filósofo
      sem_wait(&garfo[garfoEsq]);  // Bloqueia o garfo à esquerda do filósofo
    } else {
      // Outros filósofos pegam os garfos em ordem fixa
      sem_wait(&garfo[garfoEsq]);  // Bloqueia o garfo à esquerda do filósofo
      sem_wait(&garfo[garfoDir]);  // Bloqueia o garfo à direita do filósofo
    }
    cout << "Filósofo " << i << " está comendo." << endl;
    sleep(0.5);                  // Thread fica por 0.5 segundos comendo
    sem_post(&garfo[garfoEsq]);  // Libera o garfo à esquerda
    sem_post(&garfo[garfoDir]);  // Libera o garfo à direita
  }
}

int main() {
  for (int i = 0; i < N; i++) {
    sem_init(&garfo[i], 0, 1);  // Inicializa os semáforos com valor 1
  }
  thread t[N];
  for (int i = 0; i < N; i++) {
    t[i] = thread(filosofo, i);
  }
  for (int i = 0; i < N; i++) {
    t[i].join();
  }
  return 0;
}
