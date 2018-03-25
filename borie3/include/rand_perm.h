#ifndef __RAND_PERM_
#define __RAND_PERM_

typedef struct perm_n{
  int* values;
  int size;
  int current;
}Perm_n;


typedef struct perm_set{
  int* values;
  int size;
  int current;
}Perm_set;


void init_perm_n(int nb_edges);
void reset_perm_n(void);
int random_perm_n(void);

void init_perm_set(int nb_edges);
void reset_perm_set(void);
void add_set_perm_set(int* values, int size);
int random_perm_set(void);

#endif
