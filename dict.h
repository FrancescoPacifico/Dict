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
	struct couple value; //every node is made of a couple - ogni nodo contiene una coppia
	struct nodo *prev, *succ; //the way it works is the same of an integers list - funziona come una lista di interi
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
struct node *newList(void);
struct node *append0(struct node *, struct couple);
struct node *append1(struct node *, struct couple);
struct node *pop0(struct node *);
struct node *pop1(struct node *);

//Dictionaries prototypes - Prototipi Dizionari
int isPrime(int);
int nextPrime(int, int);
struct dictionary newDictionary(int);
int hash(char *, int);
struct node *isKeyIn(struct dictionary, char *);
struct dictionary extendDictionary(struct dictionary, int);
struct dictionary addCouple(struct dictionary, struct couple);
struct dictionary removeCouple(struct dictionary, char *);
void printDictionary(struct dictionary);

//Utility prototypes - Prototipi di utilità

/* ENG:
 * We'll need these functions later.
 * ITA:
 * Avremo bisogno di queste funzioni dopo.
 */

char *itoa(int);
void reverse(char *);

/* ENG:
 * Here we have the main functions to manage a CatList.
 * They are like the ones I used in my CatList repository, but this time we'll use a struct Couple as a value in the nodes of the lists,
 * so we have a couple more instructions, because the value in each node is a Couple of two elements.
 * ITA:
 * Di seguito abbiamo le funzioni principali per gestire una Lista Concatenata.
 * Sono uguali a quelle che ho usato nella mia repository sulle Liste Concatenate, ma questa volta useremo una struct Coppia (Couple) come valore nei nodi delle liste,
 * dunque abbiamo un paio di istruzioni in più, poichè il valore in ogni nodo è una Coppia di due elementi.
 */

struct node *newList(void) {
	return NULL;
}
struct node *append0(struct node *a, struct couple v) {
	struct node *p = malloc(sizeof(struct node));
	p -> value.k = malloc(strlen(v.k) + 1), p -> value.f = v.f, p -> prev = NULL, p -> succ = a;
	strcpy(p -> value.k, v.k);
	if(a != NULL) {
		a -> prev = p;
	}
	return p;
}
struct node *append1(struct node *a, struct couple v) {
	struct node *p;
	if(a != NULL) {
		p = malloc(sizeof(struct node));
		p -> value.k = malloc(strlen(v.k) + 1), p -> value.f = v.f, p -> prev = a, p -> succ = a -> succ;
		strcpy(p -> value.k, v.k);
		a -> succ = p;
		if(p -> succ != NULL) {
			p -> succ -> prev = p;
		}
	}
	return a;
}
struct node *pop0(struct node *a) {
	struct node *temp = a;
	a = a -> succ;
	a -> prev = NULL;
	free(temp -> value.k);
	free(temp);
	return a;
}
struct node *pop1(struct node *a) {
	struct node *temp = a -> succ;
	a -> succ = temp -> succ;
	if(a -> succ != NULL)  {
		a -> succ -> prev = a;
	}
	free(temp -> value.k);
	free(temp);
	return a;
}

/* ENG:
 * Here begins the "new stuff". We introduce the main functions to manage a dictionary.
 * As you can read in the point (9) of the big comment at the beginning, it's better to give to c (the number of cells of the Dynamic Array) a prime value.
 * So here we define two functions that will help us understanding if a number is prime and to find the successive prime number of an integer taken as input.
 * ITA:
 * Qui cominciano le "cose nuove". Introduciamo le funzioni principali per gestire un dizionario.
 * Come potete leggere nel punto (9) del lungo commento iniziale, è meglio dare a c (il numero di celle dell'Array Dinamico) un valore primo.
 * Dunque qui definiamo due funzioni che ci aiuteranno a capire se un numero è primo e a trovare il successivo numero primo di un intero preso in input.
 */

/* ENG:
 * This function returns 1 if the input number is prime, else returns 0.
 * It's not useful to understand how this function works now.
 * ITA:
 * Questa funzione restituisce 1 se il numero in input è primo, 0 altrimenti.
 * Non è utile cercare di capire come funziona ora.
 */

int isPrime(int m) {
	int divider;
	for(divider = 2; divider <= m; divider++) {
		if(m % divider == 0) {
			break;
		}
	}
	return (divider == m) ? 1 : 0;
}

/* ENG:
 * This function finds the k times next prime number of m exploiting a simple recursive algorithm made by me.
 * It's not useful to understand how this function works now.
 * ITA:
 * Questa funzione trova il k-esimo numero primo successivo a m sfruttando un semplice algoritmo ricorsivo fatto da me.
 * Non è utile cercare di capire come funziona ora.
 */
int nextPrime(int m, int k) {
	if(k == 0) {
		if(isPrime(m) == 1) {
			return m;
		}
		return nextPrime(m + 1, 0);
	}
	m++;
	while(isPrime(m) == 0) {
		m++;
	}
	return nextPrime(m, k - 1);
}

/* ENG:
 * The function below creates a new Dictionary made of a Dynamic Array with m cells, with m taken as input.
 * If the value given is not a prime number, m is re-set with nextPrime to the next prime number.
 * Obviously the c value associated to the Dynamic Array must be set to m and the n value must be set to 0, becasue there's still no couple in the dictionary.
 * We must alloc enough memory to the dynArray pointer: it must contain m pointers to node, so we alloc 'm * sizeof(struct node *)' memory.
 * Then we use a for cycle to create a new void list for each cell of the Dynamic Array.
 * ITA:
 * La funzione sotto crea un nuovo Dizionario fatto da un Array Dinamico con m celle, con m preso in input.
 * Se il valore dato non è un numero primo, m è reimpostato con la funzione nextPrime al numero primo successivo.
 * Ovviamente il valore c associato all'Array Dinamico deve essere impostato a m e il valore n a 0, perchè non ci sono ancora coppie nel dizionario.
 * Dobbiamo allocare abbastanza memoria per il puntatore dynArray: esso deve contenere m puntatori a nodo, dunque allochiamo 'm * sizeof(struct node *)' memoria.
 * Poi usiamo un ciclo for per creare una nuova lista vuota per ogni cella dell'Array Dinamico.
 */

struct dictionary newDictionary(int m) {
	struct dictionary output;
	m = (isPrime(m) == 1) ? m : nextPrime(m, 1);
	output.c = m, output.n = 0, output.dynArray = malloc(m * sizeof(struct nodo *));
	for(m = 0; m < output.c; m++) {
		output.dynArray[m] = newList();
	}
	return output;
}

/* ENG:
 * The one below is probably one of the most important functions.
 * It takes a string (the key of a couple) and the c value of a dictionary (the m parameter) as input and returns an integer called temp here.
 * In order to understand how this function works, you must know how the ^ operator works.
 * It's the bit-to-bit xor operator. To understand how it works let's make an example:
 * a = 12 (decimal) = 1100 (binary)
 * b = 6 (decimal) = 0110 (binary)
 * c = a ^ b = 1010 (binary) = 10 (decimal)
 * When two digits are equal (1 and 1 or 0 and 0) the xor between these two digits is 0, while it's 1 when they are different.
 * The bit-to-bit xor operator (^) does it for every couple of bits: one from the first operand and the corresponding for position from the second operand.
 * We want to use this operator between every couple of adjavent characters in the key string:
 * If the key string is "Hello", then we want to return the result of 'H' xor 'e' xor 'l' xor 'l' xor 'o'. (remember that a character must be considered as a small integer).
 * To achieve this we use a while cycle.
 * Every char of the key string is obviously made of 8 bits, so the result of this cycle will be an 8 bits integer, so a not negative number <= 255.
 * We want this number to be "compatible" with our Dynamic Array, so to have a value that represents a valid index; that's why we use the % operator.
 * ITA:
 * Quella qui sotto è probabilmente una delle funzioni più importanti.
 * Prende in input una stringa (la chiave della coppia) e il valore c di un dizionario (il parametro m) e restituisce un intero chiamato temp qui.
 * Per capire come funziona l'algoritmo, occorre sapere come funziona l'operatore ^.
 * Esso è l'operatore xor bit a bit. Per capire come funziona facciamo un esempio:
 * a = 12 (base dieci) = 1100 (base due)
 * b = 6 (base dieci) = 0110 (base due)
 * c = a ^ b = 1010 (base due) = 10 (base dieci)
 * Quando due cifre sono uguali (1 e 1 oppure 0 e 0) lo xor tra di esse è 0, mentre è 1 quando sono diverse.
 * L'operatore xor bit a bit (^) fa questo per ogni coppia di bit: uno del primo operando e il corrispondente per posizione del secondo operando.
 * Vogliamo usare questo operatore tra tutte le coppie di caratteri adiacenti nella stringa chiave:
 * Se la stringa chiave è "Ciao", allora vogliamo restituire il risultato di 'C' xor 'i' xor 'a' xor 'o'. (ricorda che un carattere deve essere considerato come un intero piccolo).
 * Per fare ciò usiamo un ciclo while.
 * Ogni carattere della stringa è ovviamente di 8 bit, dunque il risultato di questo ciclo sarà un intero di 8 bit, dunque un numero non negativo <= 255.
 * Vogliamo che questo numero sia "compatibile" con il nostro Array Dinamico, dunque che abbia un valore che rappresenti un indice valido; per questo usiamo l'operatore %. 
 */

int hash(char *k, int m) {
	char temp = *k++;
	while(*k++) {
		temp ^= *k;
	}
	return temp % m;
}

/* */

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
