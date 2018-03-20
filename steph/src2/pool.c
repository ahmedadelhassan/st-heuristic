#include <stdlib.h>

#include "pool.h"

/**
 *
 * @param n_bools
 * @return
 */
static pool_item_t *pool_item_alloc(size_t n_bools) {
    pool_item_t *p_pool_item = (pool_item_t) malloc(sizeof(pool_item_t));
    if (!p_pool_item) {
        fprintf(stderr, "pool_item_alloc. memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    p_pool_item->p_bvector = bvector_alloc(n_bools);
    p_pool_item->p_next = NULL;

    return (p_pool_item);
}

/**
 *
 * @param p_pool_item
 * @param n_bools
 */
static void pool_item_release(pool_item_t *p_pool_item, size_t n_bools) {
    if (p_pool_item) {
        pool_item_release(p_pool_item->p_next, n_bools);
        bvector_release(p_pool_item->p_bvector);
        memset(p_pool_item, 0x0, sizeof(pool_item_t));
        free(p_pool_item);
    }
}

/**
 *
 * @param size
 * @param n_bools
 * @return
 */
pool_t *pool_alloc(size_t n_bools) {
    pool_t *p_pool = (pool_t) malloc(sizeof(pool_t));
    if (!p_pool) {
        fprintf(stderr, "pool_alloc. memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    p_pool->n_bools = n_bools;
    p_pool->p_pool_ready_items = NULL;
    p_pool->p_pool_empty_items = NULL;

    return (p_pool);
}

/**
 *
 * @param p_pool
 */
void pool_realease(pool_t *p_pool) {
    assert(p_pool);

    pool_item_release(p_pool->p_pool_ready_items, p_pool->n_bools)
    pool_item_release(p_pool->p_pool_used_items, p_pool->n_bools)
    memset(p_pool, 0x0, sizeof(pool_t));
    free(p_pool);
}

/**
 *
 * @param p_pool
 * @return
 */
node *pool_get(pool_t *p_pool) {
    assert(p_pool);

    if (!p_pool->p_pool_items) {
        p_pool->p_pool_items = pool_item_alloc(p_pool->n_bools);
    }

    pool_item_t *p_pool_item = p_pool->p_pool_items;
    bvector_t *p_bvectors = p_pool_item->p_bvectors;
    p_pool_item->p_bvectors = NULL;

    p_pool->p_pool_items = p_pool->p_pool_items->p_next;
    p_pool_item->p_next = p_pool->p_pool_empty_items;
    p_pool->p_pool_empty_items = p_pool_item;

}

/**
 *
 * @param p_pool
 * @param p_bvectors
 */
void pool_return(pool_t *p_pool, bvector_t *p_bvectors) {
    assert(p_pool);
    assert(p_bvectors);
    assert(p_pool->p_pool_empty_items);

    bvector_set_all(p_bvectors, FALSE);

    pool_item_t *p_pool_item = p_pool->p_pool_empty_items;
    p_pool->p_pool_empty_item = p_pool->p_pool_empty_item->p_next;

    p_pool_item->p_bvectors = p_bvectors;

    p_pool_item->p_next = p_pool->p_pool_ready_items;
    p_pool->p_pool_ready_items = p_pool_item;
}