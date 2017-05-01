#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "union_find.h"

void UnionFindCreate(UnionFind_t *uf, int n){
    uf -> parent = (int *)malloc(sizeof(int) * n);
    uf -> count = n;
    for(int i=0;i<uf->count;i++){
        uf->parent[i]=i;
    }
}

int findRoot(UnionFind_t *uf, int vertex){
    while(uf->parent[vertex]!=vertex){
        vertex = uf->parent[vertex];
    }
    return vertex;
}

bool isConnected(UnionFind_t *uf, int vertex1, int vertex2){
    return findRoot(uf, vertex1)==findRoot(uf, vertex2);
}

void unionNodes(UnionFind_t *uf, int vertex1, int vertex2){
    int parenta = uf->parent[vertex1];
    int parentb = uf->parent[vertex2];
    if(parenta!=parentb){
        uf->parent[parenta] = parentb;
        uf->count--;
    }
}