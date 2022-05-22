#include "hash_table_header.h"
struct Binding
{
    char* key;
    int value;
    struct Binding* next;
};

struct HashTable
{
    struct Binding* buckets[BUCKET_COUNT];
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

struct Binding* hashTable_pair(const char* key, int value)
{
    struct Binding* binding = (struct Binding*) malloc(sizeof(struct Binding));
    binding->key = malloc(strlen(key) + 1);
    strcpy(binding->key, key);
    binding->value= value;
    binding->next = NULL;
    return binding;
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
    struct Binding* binding = (struct Binding*) malloc(sizeof(struct Binding));
    binding = bucket->buckets[hashIndex];

    // no entry means slot empty, insert immediately
    if (binding == NULL)
    {
        bucket->buckets[hashIndex] = hashTable_pair(key, value);
        return true;
    }

    struct Binding *prev;

    // walk through each entry until either the end is
    // reached or a matching key is found
    while (binding != NULL)
    {
        // check key
        if (strcmp(binding->key, key) == 0)
        {
            // match found, replace value
            binding->value = value;
            return false;
        }

        // walk to next
        prev = binding;
        binding = prev->next;
    }

    // end of chain reached without a match, add new
    prev->next = hashTable_pair(key, value);
    return true;
}

struct Binding *find(struct HashTable *bucket, const char *key)
{
    unsigned int hashIndex = hash(key);

    // try to find a valid slot
    struct Binding *binding = bucket->buckets[hashIndex];

    // walk through each entry in the slot, which could just be a single thing
    while (binding != NULL)
    {
        // return value if found
        if (strcmp(binding->key, key) == 0)
        {
            return binding;
        }

        // proceed to next key if available
        binding = binding->next;
    }

    // reaching here means there were >= 1 entries but no key match
    return NULL;
}

bool remove_item(struct HashTable *bucket, const char *key)
{
    unsigned int hashIndex = hash(key);

    // try to find a valid bucket
    struct Binding *binding = bucket->buckets[hashIndex];

    // no bucket means no node
    if (binding == NULL)
    {
        return false;
    }

    struct Binding *prev;
    int idx = 0;

    // walk through each entry until either the end is reached or a matching key is found
    while (binding != NULL)
    {
        // check key
        if (strcmp(binding->key, key) == 0)
        {
            // first item and no next node
            if (binding->next == NULL && idx == 0)
            {
                bucket->buckets[hashIndex] = NULL;
            }

            // first item with a next node
            if (binding->next != NULL && idx == 0)
            {
                bucket->buckets[hashIndex] = binding->next;
            }

            // last item
            if (binding->next == NULL && idx != 0)
            {
                prev->next = NULL;
            }

            // middle item
            if (binding->next != NULL && idx != 0)
            {
                prev->next = binding->next;
            }

            // free the deleted entry
            free(binding->key);
            free(binding);

            return true;
        }

        // walk to next
        prev = binding;
        binding = prev->next;

        ++idx;
    }
    return false;
}
void delete_table(struct HashTable* table){
    struct Binding* crawler;
    struct Binding* temp;
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
        free(table);

}

void display(struct HashTable *bucket)
{
    for (int i = 0; i < BUCKET_COUNT; ++i)
    {
        struct Binding *binding = bucket->buckets[i];

        if (binding == NULL)
        {
            continue;
        }

        printf("slot[%4d]: ", i);

        for (;;)
        {
            printf("%s=%d ", binding->key, binding->value);

            if (binding->next == NULL)
            {
                break;
            }

            binding = binding->next;
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