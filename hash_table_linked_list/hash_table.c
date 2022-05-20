#include "hash_table.h"
struct Node
{
    char* key;
    int value;
    struct Node* next;
};

struct HashTable
{
    struct Node* buckets[BUCKET_COUNT];
};

unsigned int hash(const char *key)
{
    unsigned long int value = 0;
    unsigned int i = 0;
    unsigned int key_len = strlen(key);
    for (; i < key_len; ++i)
    {
        value = value * 37 + key[i];
    }
    value = value % BUCKET_COUNT;
    return value;
}

struct Node* hashTable_pair(const char* key, int value)
{
    struct Node* node = (struct Node*) malloc(sizeof(struct Node));
    node->key = malloc(strlen(key) + 1);
    strcpy(node->key, key);
    node->value= value;
    node->next = NULL;
    return node;
}

struct HashTable *create(void)
{
    struct HashTable *buckets =(struct HashTable*) malloc(sizeof(struct HashTable));
    int i = 0;
    for (; i < BUCKET_COUNT; ++i)
    {
        buckets->buckets[i] = NULL;
    }

    return buckets;
}

bool add(struct HashTable *bucket, const char *key, int value)
{
    unsigned int hashIndex = hash(key);
    struct Node* node = (struct Node*) malloc(sizeof(struct Node));
    node = bucket->buckets[hashIndex];

    // no entry means slot empty, insert immediately
    if (node == NULL)
    {
        bucket->buckets[hashIndex] = hashTable_pair(key, value);
        return true;
    }

    struct Node *prev;

    // walk through each entry until either the end is
    // reached or a matching key is found
    while (node != NULL)
    {
        // check key
        if (strcmp(node->key, key) == 0)
        {
            // match found, replace value
            node->value = value;
            return false;
        }

        // walk to next
        prev = node;
        node = prev->next;
    }

    // end of chain reached without a match, add new
    prev->next = hashTable_pair(key, value);
    return true;
}

struct Node *find(struct HashTable *bucket, const char *key)
{
    unsigned int hashIndex = hash(key);

    // try to find a valid slot
    struct Node *node = bucket->buckets[hashIndex];

    // walk through each entry in the slot, which could just be a single thing
    while (node != NULL)
    {
        // return value if found
        if (strcmp(node->key, key) == 0)
        {
            return node;
        }

        // proceed to next key if available
        node = node->next;
    }

    // reaching here means there were >= 1 entries but no key match
    return NULL;
}

bool remove_item(struct HashTable *bucket, const char *key)
{
    unsigned int hashIndex = hash(key);

    // try to find a valid bucket
    struct Node *node = bucket->buckets[hashIndex];

    // no bucket means no node
    if (node == NULL)
    {
        return false;
    }

    struct Node *prev;
    int idx = 0;

    // walk through each entry until either the end is reached or a matching key is found
    while (node != NULL)
    {
        // check key
        if (strcmp(node->key, key) == 0)
        {
            // first item and no next node
            if (node->next == NULL && idx == 0)
            {
                bucket->buckets[hashIndex] = NULL;
            }

            // first item with a next node
            if (node->next != NULL && idx == 0)
            {
                bucket->buckets[hashIndex] = node->next;
            }

            // last item
            if (node->next == NULL && idx != 0)
            {
                prev->next = NULL;
            }

            // middle item
            if (node->next != NULL && idx != 0)
            {
                prev->next = node->next;
            }

            // free the deleted entry
            free(node->key);
            free(node);

            return true;
        }

        // walk to next
        prev = node;
        node = prev->next;

        ++idx;
    }
    return false;
}
void delete_table(struct HashTable* table){
    struct Node* crawler;
    struct Node* temp;
     for(int i = 0; i < BUCKET_COUNT; i++)
        {   
            if (table->buckets[i] != NULL)
            {    
                crawler = table->buckets[i];
                while (crawler != NULL)
                {
                    temp = crawler->next;
                    free(crawler);
                    crawler = temp;

                }
            }        
        }

}

void display(struct HashTable *bucket)
{
    for (int i = 0; i < BUCKET_COUNT; ++i)
    {
        struct Node *node = bucket->buckets[i];

        if (node == NULL)
        {
            continue;
        }

        printf("slot[%4d]: ", i);

        for (;;)
        {
            printf("%s=%d ", node->key, node->value);

            if (node->next == NULL)
            {
                break;
            }

            node = node->next;
        }

        printf("\n");
    }
}

int main(int argc, char **argv)
{
    struct HashTable* table = create();
    assert( find(table, "Test Key") == NULL);
    assert( add(table, "Test Key", 11) == true);

    assert( add(table, "Test Key", 12) == false);
    // struct Node* node =  find(table, "Test Key");
    // assert(node != NULL &&  node->value == 11);
    delete_table(table);
    display(table);
    return 0;
}