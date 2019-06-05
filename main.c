#include "dict.h"

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

int main(int argc, char *argv[]) {
	struct dictionary new_dictionary = newDictionary(2);
	int i, n;
	struct couple temp_couple;
	char *keys[] = {
		"key0", "key1", "key2", "key3", "key4", "key5", "key6", "key7", "key8", "key9",\
		"key10", "key11", "key12", "key13", "key14", "key15", "key16", "key17", "key18", "key19"
	};
	n = sizeof(keys) / sizeof(char *);
	for(i = 0; i < n; i++) {
		/* ENG:
		 * To fill the dictionary we scroll the strings in the keys array using an index i that will be the value for the read key.
		 * ITA:
		 * Per riempire il dizionario scorriamo le stringhe nel vettore keys usando un indice i che sarà il valore associato a ogni chiave letta.
		 */
		temp_couple.k = keys[i], temp_couple.f = i;
		new_dictionary = addCouple(new_dictionary, temp_couple);
	}
	printDictionary(new_dictionary);
	printf("\n\n///////////////////////////////////////\n\n\n");
	new_dictionary = removeCouple(new_dictionary, "key14");
	printDictionary(new_dictionary);
	printf("\n\n///////////////////////////////////////\n\n\n");
	temp_couple.k = "Hello!", temp_couple.f = 3.14;
	new_dictionary = addCouple(new_dictionary, temp_couple);
	printDictionary(new_dictionary);
	return 0;
}