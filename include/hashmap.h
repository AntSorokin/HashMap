#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * Used to store the key and value in the hashmap.
 * @note User is responsible for handling the memory of each pair they create.
 */
typedef struct _hashmap_pair {
    void *key;
    void *val;
} hashmap_pair_t;

/**
 * Computes the hash of a key
 * @returns The hash of key
 */
typedef uint64_t (*hash_func)(void *key);

/**
 * Compares values and returns if they are equal
 * @param a Key 1
 * @param b Key 2
 * @returns True if a and b are considered equal, else returns false
 */
typedef bool (*equal_func)(void *a, void *b);
/**
 * Used to free each hashmap_pair_t added to the hashmap if needed. Allows for cleaning up memory when hashmap_free is called.
 * @param state Any internal state required
 * @param pair Pair that may need to be cleaned up
 */
typedef void (*free_func)(hashmap_pair_t *pair, void *state);

typedef struct _hashmap_node {
    hashmap_pair_t *pair;
    struct _hashmap_node *next; 
    uint64_t hash;
} hashmap_node_t;

typedef struct {
    hashmap_node_t **buckets;
    int size;
    uint64_t capacity;
    hash_func hash;
    equal_func equal;
} hashmap_t;
/**
*    Initializes the hashmap
*    @param init_capacity The initial number of buckets
*    @param hash A function ptr that is able to take the hash of the key
*    @param equal A function ptr that is used to check if keys are equal
*    @warning init_capacity must be >= 1
*    @returns hashmap, if memory could not be allocated returns null
*/
hashmap_t* hashmap_init(uint64_t init_capacity, hash_func hash, equal_func equal);

/**
 * Frees the hashmap
 * @param map Hashmap to free
 * @param free_pair Called on every hashmap_pair_t in hashmap
 * @param state State to pass into free_pair
 */
void hashmap_free(hashmap_t *map, free_func free_pair, void *state);

/**
 * Gets value using the key provided from hashmap
 * @param map Hashmap to get value from
 * @param key Key to get value from
 * @warning Do not modify the key in a way that changes the hash
 * @returns The hashmap_pair_t containg the key and the val, if not found returns null
 */
hashmap_pair_t* hashmap_get(hashmap_t *map, void *key);

/**
 * Removes the hashmap_pair_t containing the key
 * @param map Hashmap to remove from
 * @param key Key of hashmap_pair_t to remove from
 * @returns False if memory could not be allocted, else true
 * @returns hashmap_pair_t if found, else null
 * @note The user is responsible for freeing the memory they allocated for the removed hashmap_pair_t
 */
hashmap_pair_t* hashmap_remove(hashmap_t *map, void *key);

/**
 * Adds hashmap_pair_t to hashmap
 * @param map Hashmap to add to
 * @param pair The pair being added
 * @warning Do not add duplicate keys to hashmap
 * @note User is responsible for allocating memory for hashmap_pair_t
 * @internal The key is what is hashed
 */
bool hashmap_add(hashmap_t *map, hashmap_pair_t *pair);

#endif
