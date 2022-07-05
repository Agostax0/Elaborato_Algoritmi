#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
typedef struct scatola {
	double x;
	double y;
	double z;
	double volume;
	int precedenti;								//quante scatole contiene
	int indice_scatola;							//indice della prossima scatola
	int id;										//durante il qsort perdo l'indice originale di ogni scatola
}scatola;
double volume(scatola *S,int i) {
	return S[i].x*S[i].y*S[i].z;
}
int cmpfunc(const void * a, const void * b) {
	if (((*(scatola*)a).volume - (*(scatola*)b).volume) > 0) {
		return 1;	
	}											//ordino per volume della scatola
	else {
		return -1;
	}
	//return ((*(scatola*)a).volume - (*(scatola*)b).volume); per evitare warning di possibile perdita dati
}
bool inscatolabile(scatola *S, int scatola1, int scatola2) {
	if (S[scatola1].x > S[scatola2].x && S[scatola1].y > S[scatola2].y && S[scatola1].z > S[scatola2].z) {
		return true;							//true implica che la scatola 1 entra dentro la 2
	}
	else {
		return false;
	}
}
void precedenti(scatola *S, int dim) {
	int i = 0, j;
	int indice_max = 0;
	int precedenti_max = 0;
	int precedente_temp = 0;
	int indice_temp = 0;
	for (i = 0; i < dim; i++) {
		precedenti_max = 1;
		for (j = i; j >= 0; j--) {
			if (inscatolabile(S, i, j)) {						//dall'elenco delle scatole che puo contenere
				precedente_temp = S[j].precedenti + 1;
				indice_temp = j;								
				if (precedente_temp > precedenti_max) {			//estrae quella con maggiore "sotto" scatole
					precedenti_max = precedente_temp;
					indice_max = indice_temp;					
				}
			}
		}
		S[i].precedenti = precedenti_max;						//finito il ciclo sceglie la maggiore
		S[i].indice_scatola = indice_max;
	}
}
int ricerca_max(scatola *S,int dim) {							//restituisce l'indice della scatola con piu "sotto" scatole
	int i = 0, indice_max = 0, temp = 0;
	for (i = 0; i < dim; i++) {
		if (S[i].precedenti > temp) {
			indice_max = i;
			temp = S[i].precedenti;
		}
	}
	return indice_max;
}
int print_func(scatola *S,int indice_max) {
	if(indice_max == 0){
		printf("%d scatole:\n", S[indice_max].precedenti);
		printf("%lf %lf %lf\n", S[0].x, S[0].y, S[0].z);
	}
	else if (indice_max == 1) {
		printf("%d scatole:\n", S[indice_max].precedenti);
		printf("scatola %d: %lf %lf %lf\n", S[indice_max].id, S[indice_max].x, S[S[indice_max].id].y, S[S[indice_max].id].z);
		printf("scatola %d: %lf %lf %lf\n", S[S[indice_max].indice_scatola].id, S[S[indice_max].indice_scatola].x, S[S[indice_max].indice_scatola].y, S[S[indice_max].indice_scatola].z);
	}
	else {
		int i = 0, j = indice_max;
		printf("%d scatole:\n", S[indice_max].precedenti);
		for (i=0; i < S[indice_max].precedenti; i++) {			//error: statement with no effect [-Werror=unused-value] dovrebbe essere sistemato era for(i;i<...)
			printf("scatola %d: %lf %lf %lf\n",S[j].id,S[j].x, S[j].y, S[j].z);
			j = S[j].indice_scatola;
		}
	}
	return 0;
}
int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	int z = 0;
	//FILE *f = fopen("test 1000-6.txt", "r");
	if (f != NULL) {
		int cont = 0;
		fscanf(f, "%d", &cont);
		scatola *S = (scatola *) malloc(sizeof(scatola) * cont );
		int j = 0;
		for (j = 0; j < cont; j++) {
			fscanf(f, "%lf %lf %lf", &S[j].x, &S[j].y, &S[j].z);
			S[j].precedenti = 1;
			S[j].volume = volume(S, j);
			S[j].id = j;					//identificativo originale pre-quicksort
		}
		qsort(S, cont, sizeof(scatola), cmpfunc);
		precedenti(S, cont);
		z=ricerca_max(S, cont);
		print_func(S,z);
		return 0;
	}
	else {
		printf("errore");
		return 0;
	}
}
