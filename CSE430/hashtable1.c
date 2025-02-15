/* Name: Remon Hasan Apu
  ID: 17101086
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACITY 50000 // Size 

unsigned long hash_function(char* str) {
    unsigned long i = 0;
    for (int j=0; str[j]; j++)
        i += str[j];
    return i % CAPACITY;
}

typedef struct Ht_item Ht_item;

//Item 
struct Ht_item {
    char* key;
    char* value;
};


typedef struct LinkedList LinkedList;

// Linkedlist 
struct LinkedList {
    Ht_item* item; 
    LinkedList* next;
};


typedef struct HashTable HashTable;

//Hash Table 
struct HashTable {
    // items
    Ht_item** items;
    LinkedList** overflow_buckets;
    int size;
    int count;
};


static LinkedList* allocate_list () {
    // Allocates memory
    LinkedList* list = (LinkedList*) calloc (1, sizeof(LinkedList));
    return list;
}

static LinkedList* linkedlist_insert(LinkedList* list, Ht_item* item) {
    // Insert
    if (!list) {
        LinkedList* head = allocate_list();
        head->item = item;
        head->next = NULL;
        list = head;
        return list;
    } 
    
    else if (list->next == NULL) {
        LinkedList* node = allocate_list();
        node->item = item;
        node->next = NULL;
        list->next = node;
        return list;
    }

    LinkedList* temp = list;
    while (temp->next) {
        temp = temp->next;
    }
    
    LinkedList* node = allocate_list();
    node->item = item;
    node->next = NULL;
    temp->next = node;
    
    return list;
}

static Ht_item* linkedlist_remove(LinkedList* list) {
    // Removes head
    // return pop
    if (!list)
        return NULL;
    if (!list->next)
        return NULL;
    LinkedList* node = list->next;
    LinkedList* temp = list;
    temp->next = NULL;
    list = node;
    Ht_item* it = NULL;
    memcpy(temp->item, it, sizeof(Ht_item));
    free(temp->item->key);
    free(temp->item->value);
    free(temp->item);
    free(temp);
    return it;
}

static void free_linkedlist(LinkedList* list) {
    LinkedList* temp = list;
    if (!list)
        return;
    while (list) {
        temp = list;
        list = list->next;
        free(temp->item->key);
        free(temp->item->value);
        free(temp->item);
        free(temp);
    }
}

// not implemented
static LinkedList** create_overflow_buckets(HashTable* table) {
    LinkedList** buckets = (LinkedList**) calloc (table->size, sizeof(LinkedList*));
    for (int i=0; i<table->size; i++)
        buckets[i] = NULL;
    return buckets;
}

// not implemented
static void free_overflow_buckets(HashTable* table) {
    LinkedList** buckets = table->overflow_buckets;
    for (int i=0; i<table->size; i++)
        free_linkedlist(buckets[i]);
    free(buckets);
}


Ht_item* create_item(char* key, char* value) {
    // C of pointer new hash item
    Ht_item* item = (Ht_item*) malloc (sizeof(Ht_item));
    item->key = (char*) calloc (strlen(key) + 1, sizeof(char));
    item->value = (char*) calloc (strlen(value) + 1, sizeof(char));
    
    strcpy(item->key, key);
    strcpy(item->value, value);

    return item;
}

HashTable* create_table(int size) {
    //  new HashTable
    HashTable* table = (HashTable*) malloc (sizeof(HashTable));
    table->size = size;
    table->count = 0;
    table->items = (Ht_item**) calloc (table->size, sizeof(Ht_item*));
    for (int i=0; i<table->size; i++)
        table->items[i] = NULL;
    table->overflow_buckets = create_overflow_buckets(table);

    return table;
}

void free_item(Ht_item* item) {
    // Frees an item
    free(item->key);
    free(item->value);
    free(item);
}

void free_hashtable(HashTable* table) {
    // Frees the table
    for (int i=0; i<table->size; i++) {
        Ht_item* item = table->items[i];
        if (item != NULL)
            free_item(item);
    }

    free_overflow_buckets(table);
    free(table->items);
    free(table);
}

void handle_collision(HashTable* table, unsigned long index, Ht_item* item) {
    LinkedList* head = table->overflow_buckets[index];

    if (head == NULL) {
        // the list
        head = allocate_list();
        head->item = item;
        table->overflow_buckets[index] = head;
        return;
    }
    else {
        // Insert to the list
        table->overflow_buckets[index] = linkedlist_insert(head, item);
        return;
    }
 }

void ht_insert(HashTable* table, char* key, char* value) {
    // Create the item
    Ht_item* item = create_item(key, value);

    //index
    int index = hash_function(key);

    Ht_item* current_item = table->items[index];
    
    if (current_item == NULL) {
        // Key does not exist.
        if (table->count == table->size) {
            // Hash Table Full
            printf("Insert Error: Hash Table is full\n");
            // Remove the create item
            free_item(item);
            return;
        }
        
        // Insert directly
        table->items[index] = item; 
        table->count++;
    }

// not implemented
    else {
            // Scenario 1: We only need to update value
            if (strcmp(current_item->key, key) == 0) {
                free(table->items[index]->value);
                table->items[index]->value = (char*) calloc (strlen(value) + 1, sizeof(char));
                strcpy(table->items[index]->value, value);
                free_item(item);
                return;
            }
    
        else {
            // Scenario 2: Collision
            handle_collision(table, index, item);
            return;
        }
    }
}

char* ht_search(HashTable* table, char* key) {
    // Searches the key 
    int index = hash_function(key);
    Ht_item* item = table->items[index];
    LinkedList* head = table->overflow_buckets[index];

    //move to items which are not NULL
    while (item != NULL) {
        if (strcmp(item->key, key) == 0)
            return item->value;
        if (head == NULL)
            return NULL;
        item = head->item;
        head = head->next;
    }
    return NULL;
}

void ht_delete(HashTable* table, char* key) {
    // Delete
    int index = hash_function(key);
    Ht_item* item = table->items[index];
    LinkedList* head = table->overflow_buckets[index];

    if (item == NULL) {
        // Does not exist. Return
        return;
    }
    // not implemented
    else {
        if (head == NULL && strcmp(item->key, key) == 0) {
            // No collision chain. Remove the item
            // and set table index to NULL
            table->items[index] = NULL;
            free_item(item);
            table->count--;
            return;
        }
        else if (head != NULL) {
            // Collision Chain exists
            if (strcmp(item->key, key) == 0) {
                // Remove this item and set the head of the list
                // as the new item
                
                free_item(item);
                LinkedList* node = head;
                head = head->next;
                node->next = NULL;
                table->items[index] = create_item(node->item->key, node->item->value);
                free_linkedlist(node);
                table->overflow_buckets[index] = head;
                return;
            }

            LinkedList* curr = head;
            LinkedList* prev = NULL;
            
            while (curr) {
                if (strcmp(curr->item->key, key) == 0) {
                    if (prev == NULL) {
                        // First element of the chain. Remove the chain
                        free_linkedlist(head);
                        table->overflow_buckets[index] = NULL;
                        return;
                    }
                    else {
                        // This is somewhere in the chain
                        prev->next = curr->next;
                        curr->next = NULL;
                        free_linkedlist(curr);
                        table->overflow_buckets[index] = head;
                        return;
                    }
                }
                curr = curr->next;
                prev = curr;
            }

        }
    }

}

void print_search(HashTable* table, char* key) {
    char* val;
    if ((val = ht_search(table, key)) == NULL) {
        printf("%s does not exist\n", key);
        return;
    }
    else {
        printf("Key:%s, Value:%s\n", key, val);
    }
}

void print_hashtable(HashTable* table) {
    printf("\nHash Table show:\n");
    for (int i=0; i<table->size; i++) {
        if (table->items[i]) {
            printf("Index:%d, Key:%s, Value:%s", i, table->items[i]->key, table->items[i]->value);
            if (table->overflow_buckets[i]) {
                printf(" => Overflow Bucket => ");
                LinkedList* head = table->overflow_buckets[i];
                while (head) {
                    printf("Key:%s, Value:%s ", head->item->key, head->item->value);
                    head = head->next;
                }
            }
            printf("\n");
        }
    }
    printf("\n");
}


int main() {
    HashTable* ht = create_table(CAPACITY);
    ht_insert(ht, "int", "INTEGER");
    ht_insert(ht, "myfunction", "FUNCTION");
    ht_insert(ht, "x", "ID");
    ht_insert(ht, "5", "NUM");
    printf("\nSearching Operation:\n");
    print_search(ht, "1");
    print_search(ht, "x");
    print_search(ht, "3");
    print_hashtable(ht);
    ht_delete(ht, "5");
    ht_delete(ht, "FUNCTION");
    printf("After deletion:");
    print_hashtable(ht);
    free_hashtable(ht);
    return 0;
}

