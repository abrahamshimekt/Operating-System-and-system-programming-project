# Operating System and system programming project
## Implementing Hash Table using Linked list in c
### Structs
- struct Node{
const char* key;
int value;
struct Node* next;
}
- struct HashTable{
struct Node* buckets[BUCKET_COUNT]
}
### Interfaces
- struct HashTable* create();
- unsigned int hash(const char* key);
- bool add(struct HashTable* table, const char* key, int value);
- struct Node* find(struct HashTable* table, const char* key);
- bool remove(struct HashTable* table, const char* key);
- void delete_table(struct HashTable* table);
## De-comment Program
I have used Deterministic Finite Automaton(DFA) for the implementation of the De-commenting program. DFA is A Finite Automata(FA) since corresponding to an input symbol, there is single resultant state i.e. there is only one transition. A deterministic finite automata is set of five tuples and represented as:
- ![image](https://user-images.githubusercontent.com/87600631/169592461-90f36f56-e58a-4cbc-b887-f0b2b537bade.png)

