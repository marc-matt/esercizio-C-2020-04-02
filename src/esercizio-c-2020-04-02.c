#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>


#define NUM_CHARS 80

void swap(char * ch_1, char * ch_2){
  char temp = *ch_1;
  *ch_1 = *ch_2;
  *ch_2 = temp;
}

void bubblesort_arrays(char * arr, int dimension){
  unsigned int k = dimension;
  while(k > 0){
    int ultimoCambio = 0;
    for(int i = 0; i < k; i++){
      if(arr[i] > arr[i+1]){
        swap(&arr[i], &arr[i+1]);
       ultimoCambio = i;
     }
    }
  k = ultimoCambio;
  }
}

char getMin(char ** arr, int dimension){
  return *arr[0];
}

char getMax(char ** arr, int dimension){
  return *arr[dimension-1];
}

int get_max_index(int * arr, int dim){
  int index = 0;
  for(int i = 0; i < dim; i++){
    if(arr[i] > index)
      index = arr[i];
  }
  return index;
}

int get_min_index(int * arr, int dim){
  int index = INT_MAX;
  for(int i = 0; i < dim; i++){
    if(arr[i] > 0 && arr[i] < index)
      index = arr[i];
  }
  return index;
}

char getTrend(char * arr, int dimension){
  char arr_1[128];
  int counter[dimension];
  for(int i = 0; i < dimension; i++){
    arr_1[i] = (char) i;
    counter[i] = 0;
  }
  for(int i = 0; i < 128; i++){
    for(int j = 0; j < dimension; j++){
      if(arr[j] == arr_1[i]){
        counter[i]++;
      }
    }
  }
  int index = get_max_index(counter, 128);
  return arr[index];
}

char getOutTrend(char * arr, int dimension){
  char arr_1[dimension];
  int counter[dimension];
  for(int i = 0; i < dimension; i++){
    arr_1[i] = (char) i;
    counter[i] = 0;
  }
  for(int i = 0; i < 128; i++){
    for(int j = 0; j < dimension; j++){
      if(arr[j] == arr_1[i]){
        counter[i]++;
      }
    }
  }
  int index = get_min_index(counter, 128);
  return arr[index];
}

void analisys(char * arr, int dimension){
  char min;
  char max;
  char trend;
  char outTrend;
  trend = getTrend(arr, dimension);
  outTrend = getOutTrend(arr, dimension);
  bubblesort_arrays(arr, dimension);
  min = getMin(&arr, dimension);
  max = getMax(&arr, dimension);
  printf("Il carattere minimo è: %c\n", min);
  printf("Il carattere massimo è: %c\n", max);
  printf("Il carattere con meno occorrenze è: %c\n", outTrend);
  printf("Il carattere con più occorrenze è: %c\n", trend);
}

int main(int argc, char * argv[]){
  char char_array[NUM_CHARS];
  int index = 0;
  char ch;
  int counter = 0;
  printf("Inserisci un carattere per cominciare!\n");
  while((ch = getchar()) != EOF){
    while(index == NUM_CHARS){
      char_array[index] = ch;
      index++;
   }
   pid_t child_pid;
   int wstatus;
   switch(child_pid = fork()){
      case 0:
        printf("Sono il figlio, analizzo l'array ricevuto dal processo padre\n");
        analisys(char_array, NUM_CHARS);
        printf("Mi sopprimo! Grazie e arrivederci!\n");
        exit(EXIT_SUCCESS);
        break;
      case -1:
        printf("fork() ha fallito!\n");
        exit(1);
        break;
     default:
        counter++;
        printf("Sono il processo padre, attendo che il figlio termini l'analisi!\n");
        if(waitpid(child_pid, &wstatus, WUNTRACED | WCONTINUED) == -1){
        perror("wait error");
        } else {
          printf("Il processo figlio ha terminato!\n");
        }
    }
  }
  printf("Ho eseguito %d operazioni", counter);
  return 0;
}
