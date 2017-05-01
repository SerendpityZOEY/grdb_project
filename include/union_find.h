#ifndef __UNION_FIND_H
#define __UNION_FIND_H

typedef struct {
    int *parent;
    int count;
} UnionFind_t;

void UnionFindCreate(UnionFind_t *uf, int n);

int findRoot(UnionFind_t *uf, int vertex);

bool isConnected(UnionFind_t *uf, int vertex1, int vertex2);

void unionNodes(UnionFind_t *uf, int vertex1, int vertex2);

#endif
