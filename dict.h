#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ENG:
 * A dictionary is a quite complex structure of datas that associates a key to a value. Every key is unique, but more
 * occurrences of the same value can be found. We'll use strings as keys and floats as values, but you can actually build a dictionary with
 * any key-value couple you want.
 * Before starting to build the dictionary it's better to understand how it's made: the structure described below is close to the best result you can have
 * in terms of time and number of instructions needed to work with it, for example, when you call a function to add a couple to it.
 * But we'll dive deeper in this topic later.
 * The dictionary is made of a Dynamic Array (see my repository about this data structure), that's not more than an array that can get "longer"
 * when it's needed. Every cell of the Dynamic Array contains a CatList (see my repository about this data structure). Every node of every CatList contains
 * a struct that we call Couple.
 * Couples are the core of our dictionary and are made of a pointer to char (string) and a value (float).
 * There's a method wherewith we manage this complex structure:
 * 1) The Dynamic Array is made of cells and every cell is identified by an index (from 0 to c - 1, with c = number of cells).
 * 2) We can "convert" a string (the key of the couple) to an integer index from 0 to c - 1, so the couple will be put in the CatList at that index.
 * 3) To convert the string we use a simple hash function that grants a quite uniform distruibution of the couples among the CatLists.
 * 4) When a string "obatins" its corresponding index thanks to the hash function, we append the couple identified by that string to the TOP of the CatList at that index.
 * 5) We use a Dynamic Array as "bearing structure" and not a "normal array" because we want something that can be extended when needed.
 * 6) We want to keep the number of Couples in every CatList under a certain value (we'll define a constant below), we'll see why later.
 * 7) To keep the number of Couples in every CatList under this constant we need to keep the n / c ratio <= constant.
 * 8) In the Dynamic Array struct: c is the number of CELLS of the array, while n is the total number of COUPLES in the whole dictionary.
 * 9) To have a better result from the hash function it's better for c to be a prime number. We'll build a function to achieve this.
 * ITA:
 * Un dizionario è una complessa struttura dati che associa una chiave ad un valore. Ogni chiave è unica, ma più
 * occorrenze dello stesso valore possono essere trovate. Useremo stringhe come chiavi e float come valori, ma si puuò costruire un dizionario
 * con qualsivoglia coppia chiave-valore.
 * Prima di iniziare a costruire il dizionario è meglio capire come è fatto: la struttura descritta sotto è prossima al miglior risultato ottenibile
 * in termini di numero di istruzioni e tempo necessario per lavorare con essa, per esempio, quando si chiama una funzione per aggiungerci una coppia.
 * Il dizionario è fatto da un Array Dinamico (guarda la mia repository riguardo questa struttura), che non è niente di più che un array che può diventare "più lungo"
 * all'occorrenza. Ogni cella dell'Array Dinamico contiene una Lista Concatenata (guarda la mia repository riguardo questa struttura). Ogni nodo di ogni Lista Concatenata contiene
 * una struct che chiamiamo Coppia.
 * Le Coppie sono il nucleo del nostro dizionario e sono composte da un puntatore a char (stringa) e un valore (float).
 * C'è un metodo con il quale gestiamo questa struttura complessa:
 * 1) L'Array Dinamico è fatto di celle e ogni cella è identificata da un indice (da 0 a c - 1, con c = numero di celle).
 * 2) Possiamo "convertire" una stringa (la chiave della coppia) in un indice intero da 0 a c - 1, dunque la coppia sarà inserita nella Lista Concatenata a quell'indice.
 * 3) Per convertire la stringa usiamo una semplice funzione hash che garantisce una distribuzione abbastanza uniforme delle coppie tra le Liste Concatenate.
 * 4) Quando una stringa "ottiene" il suo indice corrispondente grazie alla funzione hash, aggiungiamo la coppia identificata da tale stringa IN CIMA alla Lista Concatenata a tale indice.
 * 5) Usiamo un Array Dinamico come "struttura portante" e non un "array classico" perchè vogliamo qualcosa che possa essere esteso quando necessario.
 * 6) Vogliamo mantenere il numero di Coppie in ogni Lista Concatenata sotto un certo valore (definiremo una cosante sotto), vedermo il perchè dopo.
 * 7) Per mantenere il numero di Coppie sotto tale costante abbiamo bisogno di mantenere il rapporto n / c <= costante.
 * 8) Nella struct Array Dinamico: c è il numero di CELLE dell'array, mentre n è il numero totale di COPPIE nell'intero dizionario.
 * 9) Per ottenere un risultato migliore dalla funzione hash è meglio che c sia un numero primo. Costruiremo una funzione per questo.
 */

/* ENG:
 * This constant identifies the maximum number of nodes (Couples) for every CatList.
 * By dynamically modifying the dimension of the Dynamic Array that composes the Dictionary, it's
 * possible to keep the n / c ratio constant when you add more couples.
 * ITA:
 * Questa costante indica il numero massimo di nodi per ogni Lista Concatenata.
 * Modificando dinamicamente la dimensione del vettore nel dizionario, è possibile
 * mantenere il rapporto n / c costante all'aumentare delle coppie presenti.
 */

#define MAXLENGHT 5

struct couple {
	char *k; //key string - stringa chiave
	float f; //float value - valore float
};

struct node {
	struct couple valore; //every node is made of a couple - ogni nodo contiene una coppia
	struct nodo *prec, *succ; //the way it works is the same of an integers list - funziona come una lista di interi
};

/* ENG:
 *  dynArry declarator is 'struct node **' because it's an array (so a pointer) to pointers (CatLists are pointer to nodes).
 *  ITA:
 *  il dichiaratore di dynArray è 'struct node **' perchè esso è un array (dunque un puntatore) a puntatori (le Liste concatenate sono puntatori a nodo).
 */

struct dictionary {
	int c, n; //the meaning of c and n is at point (8) of the big comment above - il significato di c e n è al punto (8) del lungo commento di sopra
	struct node **dynArray; 
};

//CatLists prototypes - Prototipi Liste Concatenate
struct node *nuovaLista(void);
struct node *append0(struct nodo *, struct coppia);
struct node *append1(struct nodo *, struct coppia);
struct node *delete0(struct nodo *);
struct node *delete1(struct nodo *);

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

//operazioni sulle liste
struct nodo *nuovaLista(void) {
	return NULL;
}
struct nodo *append0(struct nodo *a, struct coppia v) {
	struct nodo *p = malloc(sizeof(struct nodo));
	p -> valore.k = malloc(strlen(v.k) + 1), p -> valore.f = v.f, p -> prec = NULL, p -> succ = a;
	strcpy(p -> valore.k, v.k);
	if(a != NULL) {
		a -> prec = p;
	}
	return p;
}
struct nodo *append1(struct nodo *a, struct coppia v) {
	struct nodo *p;
	if(a != NULL) {
		p = malloc(sizeof(struct nodo));
		p -> valore.k = malloc(strlen(v.k) + 1), p -> valore.f = v.f, p -> prec = a, p -> succ = a -> succ;
		strcpy(p -> valore.k, v.k);
		a -> succ = p;
		if(p -> succ != NULL) {
			p -> succ -> prec = p;
		}
	}
	return a;
}
struct nodo *delete0(struct nodo *a) {
	struct nodo *temp = a;
	a = a -> succ;
	a -> prec = NULL;
	free(temp -> valore.k);
	free(temp);
	return a;
}
struct nodo *delete1(struct nodo *a) {
	struct nodo *temp = a -> succ;
	a -> succ = temp -> succ;
	if(a -> succ != NULL)  {
		a -> succ -> prec = a;
	}
	free(temp -> valore.k);
	free(temp);
	return a;
}

//operazioni sul dizionario
int isPrimo(int m) {
	int divisore;
	for(divisore = 2; divisore <= m; divisore++) {
		if(m % divisore == 0) {
			break;
		}
	}
	return (divisore == m) ? 1 : 0;
}
int nextPrimo(int m, int k) {
	if(k == 0) {
		if(isPrimo(m) == 1) {
			return m;
		}
		return nextPrimo(m + 1, 0);
	}
	m++;
	while(isPrimo(m) == 0) {
		m++;
	}
	return nextPrimo(m, k - 1);
}
struct dizionario nuovoDizionario(int m) {
	struct dizionario output;
	m = (isPrimo(m) == 1) ? m : nextPrimo(m, 1); //m mantiente il suo valore se primo, altrimenti assume il valore del successivo numero primo
	output.c = m, output.n = 0, output.vettore = malloc(m * sizeof(struct nodo *)); //assegno i valori adatti al nuovo dizionario
	for(m = 0; m < output.c; m++) {
		output.vettore[m] = nuovaLista(); //ogni posizione nel vettore del dizionario rappresenta una lista vuota
	}
	return output;
}
int hash(char *k, int m) {
	char temp = *k++;
	while(*k++) { //scorro i caratteri della chiave finchè non raggiungo '\0'
		temp ^= *k; //ad ogni ciclo calcolo lo xor bit a bit
	}
	return temp % m; //questo numero è compreso tra 0 e m - 1 per m < 256
}
struct nodo *isKeyIn(struct dizionario input, char *chiave) {
	int h = hash(chiave, input.c);
	struct nodo *temp = input.vettore[h]; //dichiaro un puntatore all'inizio della lista di trabocco
	while(temp != NULL && strcmp(temp -> valore.k, chiave) != 0) { //"incremento" tale puntatore finchè non trovo una chiave uguale e finchè la lista ha elementi
		temp = temp -> succ;
	}
	return temp; //restituisce NULL se non ha trovato corrispondenze, altrimenti un puntatore al nodo che contiene la chiave
}
struct dizionario estendiDizionario(struct dizionario input, int m) {
	struct dizionario output = nuovoDizionario(m); //creo un dizionario della dimensione in input
	int i;
	struct nodo *temp;
	for(i = 0; i < input.c; i++) {
		temp = input.vettore[i];
		for(; temp != NULL; temp = temp -> succ) {
			/* copio le coppie del vecchio dizionario in quello nuovo usando la stessa funzione chiamante (addCoppia) senza il pericolo di capitare in un loop infinito
			 * che avverrebbe entrando nell'if(input.n / input.c) >= MAXLENGHT della funzione addCoppia. Questo pericolo è ovviamente evitato grazie al fatto che la dimensione
			 * del vettore del nuovo dizionario è tale che la disuguaglianza sopra citata non potrà mai essere vera*/
			output = addCoppia(output, temp -> valore);
		}
	}
	return output;
}
struct dizionario addCoppia(struct dizionario input, struct coppia inCoppia) {
	struct nodo *temp = isKeyIn(input, inCoppia.k); //verifico se la chiave in input si trova già nel dizionario (la verifica avviene su una copia del dizionario in input)
	int h, newC;
	if(temp != NULL) { //se ho ottenuto un risultato positivo, temp punta al nodo del dizionario-copia che contiene tale chiave
		temp -> valore.f = inCoppia.f; //aggiorno il valore della coppia per quel nodo
	}
	else {
		h = hash(inCoppia.k, input.c); //se la chiave non era presente nel dizionario, mi muovo all'indice del vettore per aggiungere la coppia in input
		input.vettore[h] = append0(input.vettore[h], inCoppia); //aggiungo in testa alla lista in posizione h la coppia in input
		input.n++; //aumento di uno il numero di coppie totali nel dizionario
		if((input.n / input.c) >= MAXLENGHT) { //se il rapporto tra coppie presenti e dimensione del vettore del dizionario è >= MAXLENGHT
			newC = nextPrimo(input.c, 1); //calcolo il valore della nuova dimensione del vettore
			input = estendiDizionario(input, newC); //creo una copia del dizionario che abbia dimensione (del VETTORE) maggiore
		}
	}
	return input; //restituisco la copia modificata del dizionario
}
struct dizionario removeCoppia(struct dizionario input, char *key) {
	struct nodo *temp = isKeyIn(input, key); //dichiaro un puntatore al nodo che contiene la chiave, se esiste
	int h = hash(key, input.c); //trovo l'indice del vettore del dizionario nel quale si dovrebbe trovare la chiave
	if(temp != NULL) { //se la chiave esiste nel dizionario
		if(temp == input.vettore[h]) { //se si trova nel primo nodo di una lista di trabocco
			input.vettore[h] = delete0(input.vettore[h]);
		}
		else { //se si trova in un nodo diverso dal primo
			temp -> prec = delete1(temp -> prec);
		}
	}
	return input;
}
void stampaDizionario(struct dizionario input) {
	int i;
	struct nodo *temp;
	for(i = 0; i < input.c; i++) {
		printf("%d -->\t", i);
		temp = input.vettore[i];
		for(; temp != NULL; temp = temp -> succ) {
			printf("(\"%s\", %0.1f), ", temp -> valore.k, temp -> valore.f);
		}
		printf("\n");
	}
}

//funzioni di utilità

char *itoa(int n) { //converte un intero in stringa
	char *output;
	if(n > 9999) {
		printf("\nNUMBER TOO BIG! MAX 4 DIGITS\n");
		return NULL;
	}
	output = malloc(5);
	int digit, i;
	for(i = 0; n > 0; n /= 10, i++) {
		digit = n % 10;
		output[i] = '0' + digit;
	}
	for(; i < 4; i++) {
		output[i] = '0';
	}
	output[i] = '\0';
	reverse(output);
	return output;
}

void reverse(char *v) {
	int i, j;
	char temp;
	for(i = 0, j = strlen(v) - 1; i < j; i++, j --) {
		temp = v[i];
		v[i] = v[j];
		v[j] = temp;
	}
}
