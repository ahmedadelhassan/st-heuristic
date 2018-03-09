int union_find_find_recursive_compression(union_find_t* uf, int i) {
    assert(uf != NULL);
    assert((uf->size == 0) || (uf->ranks != NULL));
    assert((uf->parents == 0) || (uf->parents != NULL));
    assert((i >= 0) && (i < uf->size));

    if (uf->parents[i] != uf->parents[uf->parents[i]]) {
        uf->parents[i] = union_find_find_recursive_compression(uf, uf->parents[i]);
    }

    return(uf->parents[i]);
}

int union_find_find_iterative_splitting(union_find_t* uf, int i) {
    assert(uf != NULL);
    assert((uf->size == 0) || (uf->ranks != NULL));
    assert((uf->parents == 0) || (uf->parents != NULL));
    assert((i >= 0) && (i < uf->size));

    int j = uf->parents[i];
    while (j != uf->parents[j]) {
        uf->parents[i] = uf->parents[j];
        i = j;
        j = uf->parents[j];
    }

    return(j);
}

int union_find_find_iterative_halving(union_find_t* uf, int i) {
    assert(uf != NULL);
    assert((uf->size == 0) || (uf->ranks != NULL));
    assert((uf->parents == 0) || (uf->parents != NULL));
    assert((i >= 0) && (i < uf->size));

    while (uf->parents[i] != uf->parents[uf->parents[i]]) {
        uf->parents[i] = uf->parents[uf->parents[i]];
        i = uf->parents[i];
    }

    return(uf->parents[i]);
}

int union_find_find_iterative_compression(union_find_t* uf, int i) {
    assert(uf != NULL);
    assert((uf->size == 0) || (uf->ranks != NULL));
    assert((uf->parents == 0) || (uf->parents != NULL));
    assert((i >= 0) && (i < uf->size));

    int p = uf->parents[i];
    int j = i;

    /* pass 1 */
    while (p != uf->parents[p]) {
        p = uf->parents[p];
    }

    /* pass 2 */
    while (j != p) {
        uf->parents[i] = p;
        i = j;
        j = uf->parents[i];
    }

    return(p);
}
