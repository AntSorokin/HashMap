#include "hashmap.h"

/**
 * Rehashes the hashmap
 * @param map hashmap to rehash
 * @param new_capacity new capacity of the hashmap
 * @returns false if memory could not be allocated, else true
 * @warning new_capacity must be >= 1
 */
bool rehash(hashmap_t *map, uint64_t new_capacity);

hashmap_t* hashmap_init(uint64_t init_capacity, hash_func hash, equal_func equal) {
    hashmap_t *temp = malloc(sizeof(hashmap_t));

    if(!temp) {
        return 0;
    }

    temp -> buckets = calloc(init_capacity, sizeof(hashmap_node_t*));
    if(!temp -> buckets) {
        free(temp);
        return 0;
    }

    temp -> capacity = init_capacity;
    temp -> size = 0;
    temp -> hash = hash;
    temp -> equal = equal;

    return temp;
}

void hashmap_free(hashmap_t *map, free_func free_pair, void *state) {
    //Iterates through every node in hashmap
    for(uint64_t i = 0;i < map -> capacity; ++i) {
        hashmap_node_t *it = map -> buckets[i];
        while(it) {
            //Calls function ptr used to free memory if needed
            free_pair(it -> pair, state);
            hashmap_node_t *next = it -> next;
            free(it);
            it = next;
        }
    }

    free(map -> buckets);
    free(map);
}

hashmap_pair_t* hashmap_get(hashmap_t *map, void *key) {
    if(!map) {
        return 0;
    }

    uint64_t h = map -> hash(key) % map -> capacity;

    hashmap_node_t *it = map -> buckets[h];

    while(it) {
        //Compares linked list for key at hash index till it finds it
        if(map -> equal(it -> pair -> key, key)) {
            return it -> pair;
        }
        it = it -> next;
    }

    return 0;
}

bool hashmap_add(hashmap_t *map, hashmap_pair_t *pair) {
    if(!map || !pair) {
        return false;
    }

    //Resizes buckets if too small
    if((double)map -> size / map -> capacity >= .75) {
        if(!rehash(map, map -> capacity * 2)) {
            return false;
        };
    }

    uint64_t h = map -> hash(pair -> key) % map -> capacity;

    hashmap_node_t *temp = malloc(sizeof(hashmap_node_t));
    if(!temp) {
        return false;
    }

    //Hash is cached as to not recompute when hashmap is rehashed
    temp -> hash = h;
    temp -> pair = pair;
    temp -> next = map -> buckets[h];

    map -> buckets[h] = temp;
    map -> size++;

    return true;
}

hashmap_pair_t* hashmap_remove(hashmap_t *map, void *key) {
    if(!map || map -> size == 0) {
        return 0;
    }

    uint64_t h = map -> hash(key) % map -> capacity;
    hashmap_node_t *it = map -> buckets[h];

    //Special rule if node is the first element in bucket
    if(it) {
        if(map -> equal(key, it -> pair -> key)) {
            map -> buckets[h] = it -> next;
            hashmap_pair_t *temp = it -> pair;
            free(it);
            map -> size--;
            return temp;
        }
    }

    hashmap_node_t *prev_node = it;
    it = it -> next;

    //Iterates through the rest of the linked list to attempt to remove the hashmap_node_t with the key
    while(it != 0) {
        if(map -> equal(key, it -> pair -> key)) {
            prev_node -> next = it -> next;
            hashmap_pair_t *temp = it -> pair;
            free(it);
            map -> size--;
            return temp;
        }

        prev_node = it;
        it = it -> next;
    }

    return 0;
}

inline bool rehash(hashmap_t *map, uint64_t new_capacity) {
    hashmap_node_t** temp_buckets = calloc(new_capacity, sizeof(hashmap_node_t*));
    if(!temp_buckets) {
        return false;
    }

    //Uses the precomputed hash for each value and moves each node to its new bucket
    for(uint64_t i = 0; i < map -> capacity; ++i) {
        hashmap_node_t *it = map -> buckets[i];
        while(it) {
            hashmap_node_t *next = it -> next;
            uint64_t h = it -> hash % new_capacity;
            
            it -> next = temp_buckets[h];
            temp_buckets[h] = it;

            it = next;
        }
    }

    free(map -> buckets);
    map -> buckets = temp_buckets;
    map -> capacity = new_capacity;

    return true;
}

