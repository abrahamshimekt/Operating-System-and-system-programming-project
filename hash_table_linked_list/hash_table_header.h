#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#define BUCKET_COUNT 1024
struct HashTable* create();
unsigned int hash(const char* key);
bool add(struct HashTable* table, const char* key, int value);
struct Node* find(struct HashTable* table, const char* key);
bool remove_item(struct HashTable* table, const char* key);
void delete_table(struct HashTable* table);