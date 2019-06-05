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

//Utility prototypes - Prototipi di utilità
char *intToArray(int);
void reverse(char *);

int main(int argc, char *argv[]) {
	struct dictionary new_dictionary = newDictionary(2);
	int i;
	float j;
	char *key;
	struct couple temp_couple;
	for(i = 0; i < 40; i++) {
		j = rand() / 999999.0;
		key = intToArray(i);
		temp_couple.k = key, temp_couple.f = j;
		new_dictionary = addCouple(new_dictionary, temp_couple);
	}
	printDictionary(new_dictionary);
	return 0;
}