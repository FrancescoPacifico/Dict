#include "dict.h"

//prototipi liste
struct nodo *nuovaLista(void);
struct nodo *append0(struct nodo *, struct coppia);
struct nodo *append1(struct nodo *, struct coppia);
struct nodo *delete0(struct nodo *);
struct nodo *delete1(struct nodo *);

//prototipi dizionario
int isPrimo(int); //usata dalla funzione nuovoDizionario per verificare se la dimensione in input è un numero primo.
int nextPrimo(int, int); //restituisce il primo numero primo successivo all'intero in input.
struct dizionario nuovoDizionario(int); //crea un nuovo dizionario con un vettore di dimensione m. Ogni lista di trabocco è vuota.
int hash(char *, int);
struct nodo *isKeyIn(struct dizionario, char *);
struct dizionario estendiDizionario(struct dizionario, int);
struct dizionario addCoppia(struct dizionario, struct coppia);
struct dizionario removeCoppia(struct dizionario, char *);
void stampaDizionario(struct dizionario);

//prototipi di utilità
char *itoa(int);
void reverse(char *);

int main(int argc, char *argv[]) {
	struct dizionario prova = nuovoDizionario(2);
	int i;
	float j;
	char *key;
	struct coppia coppiaTemp;
	for(i = 0; i < 20; i++) {
		j = (float) rand() / 999999.0;
		key = itoa(i);
		coppiaTemp.k = key, coppiaTemp.f = j;
		prova = addCoppia(prova, coppiaTemp);
	}
	stampaDizionario(prova);
	prova = removeCoppia(prova, "0015");
	printf("\n////////////////////////////////////////////////////\n");
	coppiaTemp.k = "0018", coppiaTemp.f= 1905;
	stampaDizionario(prova);
	printf("\n////////////////////////////////////////////////////\n");
	prova = addCoppia(prova, coppiaTemp);
	stampaDizionario(prova);
	return 0;
}