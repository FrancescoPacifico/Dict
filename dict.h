#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
 * 7) To keep the number of Couples in every CatList under this constant we need to keep the n / c ratio < constant.
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
 * 7) Per mantenere il numero di Coppie sotto tale costante abbiamo bisogno di mantenere il rapporto n / c < costante.
 * 8) Nella struct Array Dinamico: c è il numero di CELLE dell'array, mentre n è il numero totale di COPPIE nell'intero dizionario.
 * 9) Per ottenere un risultato migliore dalla funzione hash è meglio che c sia un numero primo. Costruiremo una funzione per questo.
 */

/* ENG:
 * This constant identifies the maximum number of nodes (Couples) for every CatList.
 * By dynamically modifying the dimension of the Dynamic Array that composes the Dictionary, it's
 * possible to keep the n / c ratio < constant when you add more couples.
 * ITA:
 * Questa costante indica il numero massimo di nodi per ogni Lista Concatenata.
 * Modificando dinamicamente la dimensione del vettore nel dizionario, è possibile
 * mantenere il rapporto n / c < costante all'aumentare delle coppie presenti.
 */

#define MAXLENGTH 5

struct couple {
	char *k; //key string - stringa chiave
	float f; //float value - valore float
};

struct node {
	struct couple value; //every node is made of a couple - ogni nodo contiene una coppia
	struct node *prev, *succ; //the way it works is the same of an integers list - funziona come una lista di interi
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

/* ENG:
 * We said that every key in the dictionary must be unique. So, when we add a new couple to a dictionary, we must first check if the key of this couple
 * is already used in a couple of the dictionary. If we find it we update its corresponding value, else we add the entire new couple.
 * This function helps us checking if a key is in the dictionary. It takes a dictionary and a string (key) as input. It first calculates the index of the Dynamic Array where
 * that string should be using the hash function, then it checks every node of the CatList at that index to see if there's a couple with that key.
 * To keep everything cleaner I used a support pointer to node in the while cycle. To check if the input key is equal to one of the couples keys I used the strcmp function from the string header.
 * This function returns 0 only when its two arguments are equal, so I run the cycle until there's a node to check and until I don't find a correspondence.
 * If no correspondence was found the function returns NULL, else it returns a pointer to the node that contains the couple with that key.
 * ITA:
 * Abbiamo detto che ogni chiave nel dizionario deve essere unica. Dunque, quando aggiungiamo una nuova coppia ad un dizionario, dobbiamo prima controllare se la chiave di questa coppia
 * è già utilizzata in una coppia del dizionario. Se la troviamo aggiorniamo il suo valore corrispondente, altrimenti aggiungiamo interamente la nuova coppia.
 * Questa funzione ci aiuta a controllare se una chiave è nel dizionario. Prende in input un dizionario e una stringa (chiave). Prima calcola l'indice dell'Array Dinamico in cui
 * dovrebbe essere quella stringa usando la funzione hash, poi controlla ogni nodo della Lista Concatenata a quell'indice per vedere se c'è una coppia con tale chiave.
 * Per mantenere il tutto più pulito ho usato un puntatore a nodo di supporto nel ciclo while. Per controllare se la chiave in input è uguale a una delle chiavi delle coppie ho usato la funzione strcmp
 * dell'intestazione string. Questa funzione restituisce 0 solo qunado i suoi due argomenti sono uguali, quindi eseguo il ciclo finchè ci sono nodi da controllare e finchè non trovo una corrispondenza.
 * Se non viene trovata alcuna corrispondenza la funzione restituisce NULL, altrimenti restituisce un puntatore al nodo che contiene la coppia con tale chiave.
 */

struct node *isKeyIn(struct dictionary input, char *key) {
	int h = hash(key, input.c);
	struct node *temp = input.dynArray[h];
	while(temp != NULL && strcmp(temp -> value.k, key) != 0) {
		temp = temp -> succ;
	}
	return temp;
}

/* ENG:
 * Now we must build a function to actually insert a couple in a dictionary, or to update the value in a couple if the key of the input couple already exists.
 * So we first call the isKeyIn function with the input dictionary and the input couple key as parameters. If this function returns something different from NULL, it means there's
 * already a couple with that key, so we just need to update its value.
 * Else, it means there's no couple with that key, so we append the entire couple to the top of the CatList at the index given by hash function.
 * Just if we added an entire new couple we must increase n, because there's a new couple in the dictionary.
 * We said that we must keep the n / c ratio < constant (MAXLENGTH), so if the condition n / c >= MAXLENGTH is true after we added the couple, the Dynamic Array of the dictionary must be extended 
 * by creating a larger one and every couple of the old dictionary must be re-assigned to the new one.
 * To do this we must first calculate the next prime number to the actual dimension; to do this we use the nextPrime function. Then we pass the value returned, together with the old dictionary,
 * to the extendDictionary function, that's described later.
 * ITA:
 * Ora dobbiamo costruire una funzione per inserire effettivamente una coppia in un dizionario, o per aggiornare il valore in una coppia se la chiave della coppia in input già esiste.
 * Quindi prima chiamiamo la funzione isKeyIn con il dizionario in input e la chiave della coppia in input come parametri. Se questa funzione restituisce qualcosa diverso da NULL, significa
 * che c'è già una coppia con tale chiave, dunque dobbiamo solo aggiornare il suo valore.
 * Altrimenti, significa che non c'è alcuna coppia con quella chiave, dunque aggiungiamo l'intera coppia in testa alla Lista Concatenata all'indice restituito dall funzione hash.
 * Solo se abbiamo aggiunto una coppia intera nuova dobbiamo incrementare n, perchè c'è una nuova coppia nel dizionario.
 * Abbiamo detto che dobbiamo mantenere il rapporto n / c < costante (MAXLENGTH), dunque se la condizione n / c >= MAXLENGTH è vera dopo aver aggiunto la coppia, l'Array Dinamico del dizionario deve essere esteso creandone
 * uno più spazioso e ogni coppia del vecchio dizionario deve essere ri-assegnata a quello nuovo.
 * Per fare ciò dobbiamo prima calcolare il numero primo successivo alla dimensione attuale; per farlo utilizziamo la funzione nextPrime. Poi passiamo il valore restituito, insieme al vecchio dizionario,
 * alla funzione extendDictionary, che è descritta dopo.
 */

struct dictionary addCouple(struct dictionary input, struct couple inCouple) {
	struct node *temp = isKeyIn(input, inCouple.k);
	int h, newC;
	if(temp != NULL) {
		temp -> value.f = inCouple.f;
	}
	else {
		h = hash(inCouple.k, input.c);
		input.dynArray[h] = append0(input.dynArray[h], inCouple);
		input.n++;
		if((input.n / input.c) >= MAXLENGTH) {
			newC = nextPrime(input.c, 1);
			input = extendDictionary(input, newC);
		}
	}
	return input;
}

/* ENG:
 * The function below is called by the function addCouple only when the dictionary must be extended.
 * The first thing to do is to create a new void dictionary with m as dimension for its Dynamic Array (m is calculated in the addCouple function. It's a prime number,
 * bigger than the dimension of the actual dictionary Dynamic Array).
 * By using two for cycles we scroll through every node of every CatList of the old Dynamic Array. The outer cycle scrolls the Dynamic Array cells, while the inner one scrolls
 * the CatList for that cell.
 * Every node checked by these cycles contains a copy that's passed as argument, together with the new dictionary, to the addCouple function, that will cyclically build the new dictionary.
 */

struct dictionary extendDictionary(struct dictionary input, int m) {
	struct dictionary output = newDictionary(m);
	int i;
	struct node *temp;
	for(i = 0; i < input.c; i++) {
		temp = input.dynArray[i];
		for(; temp != NULL; temp = temp -> succ) {
			/* ENG:
			 * I copy the old dictionary couples in the new one by using the caller function itself (addCouple) without the possibility of falling into a never ending loop,
			 * that could happen only by re-entering the 'if(input.n / input.c) >= MAXLENGTH' of the addCouple function. We obviously avoid this danger because the dimension of the Dynamic Array
			 * of the new dictionary is big enough to never make the aforementioned inequality true.
			 * ITA: 
			 * Copio le coppie del vecchio dizionario in quello nuovo usando la stessa funzione chiamante (addCouple) senza il pericolo di capitare in un loop infinito,
			 * che avverrebbe entrando in 'if(input.n / input.c) >= MAXLENGTH' della funzione addCouple. Questo pericolo è ovviamente evitato grazie al fatto che la dimensione
			 * del vettore del nuovo dizionario è tale che la disuguaglianza sopra citata non potrà mai essere vera.
			 */
			output = addCouple(output, temp -> value);
		}
	}
	return output;
}

/* ENG:
 * The function below is used to remove a couple from a dictionary by taking the dictionary itself as input and the key that represents the couple to remove.
 * We first call the isKeyIn function to know if that key is actually in the dictionary. If it is, we use the index returned by the hash function to reach the right CatList
 * and, if the first node of the CatList is the same returned by the isKeyIn function (it's called temp here) we call the pop0 function, that removes the first node of a CatList.
 * Else, we move to the node immediately before temp and we call the pop1 function to remove temp (it works because temp is the node in position 1 respect to its previous node.
 * Check my CatList repository to better understand this concept).
 * ITA:
 * La funzione qui sotto è usata per rimuovere una coppia da un dizionario prendendo il dizionario stesso come input e la chiave che rappresenta la coppia da rimuovere.
 * Prima chiamiamo la funzione isKeyIn per sapere se quella chiave è effettivamente nel dizionario. Se lo è, usiamo l'indice restituito dalla funzione hash per raggiungere la giusto Lista Concatenata
 * e, se il primo nodo della Lista Concatenata è lo stesso restituito dalla funzione isKeyIn (è chiamato temp qui) chiamiamo la funzione pop0, che rimuove il primo nodo di una Lista Concatenata.
 * Altrimenti, ci spostiamo al nodo immediatamente prima di temp e chiamiamo la funzione pop1 per rimuovere temp (funziona perchè temo è il nodo in posizione 1 rispetto al suo precedente.
 * Guarda la mia repository sulle Liste Concatenate "CatList" per comprendere meglio questo concetto).
 */

struct dictionary removeCouple(struct dictionary input, char *key) {
	struct node *temp = isKeyIn(input, key);
	int h = hash(key, input.c);
	if(temp != NULL) {
		if(temp == input.dynArray[h]) {
			input.dynArray[h] = pop0(input.dynArray[h]);
		}
		else {
			temp -> prev = pop1(temp -> prev);
		}
		input.n--;
	}
	return input;
}

/* ENG:
 * The last function we'll build needs to print a dictionary.
 * It's very simple. We just need to scroll every node of every CatList, like we did in the extendDictionary function.
 * ITA:
 * L'ultima funzione che costruiremo serve a stampare un dizionario.
 * E' molto semplice. Dobbiamo solamente scorrere ogni nodo di ogni Lista Concatenata, come abbiamo fatto nella funzione extendDictionary.
 */

void printDictionary(struct dictionary input) {
	int i;
	struct node *temp;
	for(i = 0; i < input.c; i++) {
		printf("%d -->\t", i);
		temp = input.dynArray[i];
		for(; temp != NULL; temp = temp -> succ) {
			printf("(\"%s\", %0.3f), ", temp -> value.k, temp -> value.f);
		}
		printf("\n");
	}
}

/* ENG:
 * Calculating the cost of these function is not very easy.
 * The dictionary, as we built it, keeps the number of nodes for each CatList lower than a certain constant (MAXLENGTH), or, at least, it happens
 * in the average case. In fact, we rely on the hash function to evenly distribute the nodes among the indexes of the Dynamic Array.
 * But it doesn't always happen, in fact, if we try to build and print a dictionary from the main function, we can notice that some lists have more nodes (many of them sometimes)
 * than others and, generally, more than the constant.
 * This happens becasue the hash function can't perfectly distribute the strings, both becasue when it calculates the index of two different strings, it can obatin the same result,
 * and because strings are distributed in a limited space.
 * But, let's suppose that we had exactly less than MAXLENGTH nodes for each list, and that the hash function worked perfectly to keep this condition.
 * It will come the time when every CatList contains exactly MAXLENGTH nodes, except one of them that has MAXLENGTH - 1 nodes. When a new node will be added, the n / c
 * ratio will be exactly MAXLENGTH and this is the time when the extendDictionary function will be called, which, by re-distributing the couples in the dictionary (by always using the hash function but with
 * the new length of the Dynamic Array) will have every new list less than MAXLENGTH nodes (THIS IS AN HYPOTHETICAL CASE, THIS WILL NEVER HAPPEN).
 * The actuality is that this case only happens in an approximate way, so, when the condition n / c >= MAXLENGTH to extend the dictionary is achieved, we don't have, in that moment,
 * exactly MAXLENGTH nodes in each CatList, and it's certain that, during the re-distribution of the couples by extendDictionary, the couples will be distributed evenly.
 * So, right after the dictionary extension, we could already have lists with more than MAXLENGTH elements.
 * Nevertheless, the job made by the hash function is not so bad to completely cancel its utility. In fact, it's thanks to this function that the costs of many functions
 * are drastically reduced IN THE AVERAGE CASE, namely the case in which we can say that ON AVERAGE there are MAXLENGTH node in each CatList.
 * Before analyzing the costs it's necessary to remember that "to reach" an index in an array requires constant time, while to move inside a CatList requires, in the worst case,
 * a time linear in the list length (that is scrolling each node of it).
 * The functions to search, add and remove couples have, IN THE AVERAGE CASE, a cost linear in MAXLENGTH.
 * This statement is true becasue:
 * 1) When I look for a key in the dictionary, I first calculate the corresponding index with the hash function, that makes a number of instructions that depends on the length of the string (negligible, because it doesn't depend on the number of couples in the dictionary).
 * 2) I reach that index in a Dynamic Array in constant time.
 * 3) I scroll the corresponding CatList that, IN THE AVERAGE CASE, has MAXLENGTH node at most, so the time to scroll it is linear in MAXLENGTH.
 * 4) The cost is the same in the functions to add and remove couples, becasue the instructions are quite similar.
 * The function to extend the dictionary and the one to print it have instead an easily calculable cost. In fact, they necessarely have to scroll each node of each CatList
 * in the dictionary. So their cost is linear in n.
 * ITA:
 * Calcolare il costo di queste funzioni non è molto semplice.
 * Il dizionario, così come lo abbiamo costruito, mantiene il numero di nodi per ogni Lista Concatenata inferiore ad una certa costante (MAXLENGTH), o almeno così
 * è nel caso medio. Infatti, noi facciamo affidamento alla funzione hash per far sì che i nodi vengano distribuiti omogeneamente tra gli indici dell'Array Dinamico.
 * Ma questo non sempre avviene, anzi, provando a generare qualche dizionario dalla funzione main e stampandolo, ci accorgiamo che alcune liste avranno più nodi (anche molti a volte)
 * rispetto ad altre e, in generale, rispetto alla costante che abbiamo stabilito.
 * Questo avviene perchè la funzione hash non è in grado di distribuire in maniera perfetta le stringhe, sia perchè calcolando l'indice corrispondente a due stringhe diverse può ottenere lo stesso risultato,
 * sia perchè la distribuzione delle stringhe avviene in uno spazio limitato.
 * Assumiamo però di avere esattamente meno di MAXLENGTH nodi per ogni lista, e che la funzione di hash lavori perfettamente, in modo da mantenere questa condizione.
 * Arriverà un momento in cui ogni Lista Concatenata contiene esattamente MAXLENGTH nodi, tranne una che ne contiene MAXLENGTH - 1. Quando andremo ad aggiungere un nuovo nodo, il rapporto
 * n / c sarà esattamente uguale a MAXLENGTH, ed è allora che verrà invocata la funzione extendDictionary, la quale, ri-distribuendo le coppie nel dizionario (usando sempre la funzione hash, ma sulla nuova
 * lunghezza dell'Array Dinamico) farà in modo che tutte le nuove liste abbiano meno di MAXLENGTH nodi (QUESTO E' UN CASO IMMAGINARIO, CIO' NON AVVERRA' MAI).
 * La realtà è che questa situazione si verifica solamente in maniera approssimata, dunque, quando viene raggiunta la condizione n / c >= MAXLENGTH per estendere il dizionario, in realtà
 * in quel momento, non abbiamo esattamente MAXLENGTH coppie per ogni Lista Concatenata e non è detto che durante la ri-distribuzione delle coppie da parte di extendDictionary,
 * queste vengano a loro volta distribuite omogeneamente. Dunque, subito dopo l'estensione del dizionario, potremmo avere già liste che hanno superato i MAXLENGTH elementi.
 * Nonostante ciò, il lavoro svolto dalla funzione hash non è così pessimo da vanificare completamente la sua utilità. Anzi, è proprio grazie a questa funzione che i costi
 * di molte funzioni si riducono drasticamente NEL CASO MEDIO, cioè il caso in cui possiamo dire che MEDIAMENTE ci sono MAXLENGTH nodi per ogni Lista Concatenata.
 * Prima di analizzare i costi è necessario ricordare che "raggiungere" un indice all'interno di un array richiede tempo costante, mentre muoversi all'interno di una
 * Lista Concatenata richiede, nel caso peggiore, tempo lineare nella lunghezza della lista (ovvero scorrere tutti i nodi di essa).
 * Le funzioni di ricerca, aggiunta e rimozione hanno, NEL CASO MEDIO, un costo lineare in MAXLENGTH.
 * Questa affermazione è vera perchè:
 * 1) Quando cerco una chiave nel dizionario, prima ne calcolo l'indice corrispondente con hash, che fa un numero di operazioni dipendente dalla lunghezza della stringa (trascurabile perchè non dipende dal numero di coppie nel dizionario).
 * 2) Raggiungo tale indice in un Array Dinamico in tempo costante.
 * 3) Scorro la corrispondente Lista Concatenata che, NEL CASO MEDIO, ha al massimo MAXLENGTH nodi, dunque il tempo impiegato per scorrerla è lineare in MAXLENGTH.
 * 4) Il costo è lo stesso nelle funzioni di aggiunta e rimozione perchè le operazioni svolte sono pressochè le stesse.
 * La funzione di estensione del dizionario e quella per stamparlo hanno invece un costo facilmente calcolabile. Esse, infatti, devono necessariamente scorrere ogni coppia di ogni Lista Concatenata
 * nel dizionario. Dunque il costo di queste funzioni è lineare in n.
 */