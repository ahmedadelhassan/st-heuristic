#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pool.h"

/**
 *
 * @param size
 * @return
 */
static pool_item_t *pool_item_alloc(size_t size) {
    pool_item_t *p_pool_item = (pool_item_t*) malloc(sizeof(pool_item_t));
    if (!p_pool_item) {
        fprintf(stderr, "pool_item_alloc. memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    p_pool_item->p_bvector = bvector_alloc(size);
    p_pool_item->p_next = NULL;

    return (p_pool_item);
}

/**
 *
 * @param p_pool_item
 * @param size
 */
static void pool_item_release(pool_item_t *p_pool_item) {
    while (p_pool_item) {
        pool_item_t *p_next_pool_item = p_pool_item->p_next;
        bvector_release(p_pool_item->p_bvector);
        memset(p_pool_item, 0x0, sizeof(pool_item_t));
        free(p_pool_item);
        p_pool_item = p_next_pool_item;
    }
}

/**
 *
 * @param size
 * @param size
 * @return
 */
pool_t *pool_alloc(size_t size) {
    pool_t *p_pool = (pool_t*) malloc(sizeof(pool_t));
    if (!p_pool) {
        fprintf(stderr, "pool_alloc. memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    p_pool->size = size;
    p_pool->p_bvector_queue = NULL;
    p_pool->p_queue = NULL;

    return (p_pool);
}

/**
 *
 * @param p_pool
 */
void pool_release(pool_t *p_pool) {
    assert(p_pool);

    pool_item_release(p_pool->p_bvector_queue);
    pool_item_release(p_pool->p_queue);
    memset(p_pool, 0x0, sizeof(pool_t));
    free(p_pool);
}

/**
 *
 * @param p_pool
 * @return
 */
bvector_t *pool_get(pool_t *p_pool) {
    assert(p_pool);

    if (!p_pool->p_bvector_queue) {
        p_pool->p_bvector_queue = pool_item_alloc(p_pool->size);
    }

    /* unlink the first pool_item_t item from p_bvector_queue */
    pool_item_t *p_pool_item = p_pool->p_bvector_queue;
    p_pool->p_bvector_queue = p_pool->p_bvector_queue->p_next;

    /* link the pool_item_t to p_queue */
    p_pool_item->p_next = p_pool->p_queue;
    p_pool->p_queue = p_pool_item;

    /* unlink the associated bvector */
    bvector_t *p_bvector = p_pool_item->p_bvector;
    p_pool_item->p_bvector = NULL;

    /* return the associated bvector */
    return (p_bvector);
}

/**
 *
 * @param p_pool
 * @param p_bvector
 */
void pool_return(pool_t *p_pool, bvector_t *p_bvector) {
    assert(p_pool);
    assert(p_bvector);
    assert(p_pool->p_queue);

    bvector_unset_all(p_bvector);

    pool_item_t *p_pool_item = p_pool->p_queue;
    p_pool->p_queue = p_pool->p_queue->p_next;

    p_pool_item->p_bvector = p_bvector;

    p_pool_item->p_next = p_pool->p_bvector_queue;
    p_pool->p_bvector_queue = p_pool_item;
}