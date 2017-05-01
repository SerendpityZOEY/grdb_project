#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "cli.h"
#include "HashMap.h"
#include "union_find.h"
#include "quick_sort.h"

void cli_graph_update_tuples(schema_type_t st, int old_schema_size);

double get_cost(tuple_t t, enum_list_t el){
    attribute_t attr;
    int offset, i;

    for (attr = t->s->attrlist; attr != NULL; attr = attr->next) {
        offset = tuple_get_offset(t, attr->name);
        if(offset>=0){
            i = tuple_get_double(t->buf + offset);
            //printf("%d", i);
        }
    }
    return i;
}

void create_edges(graph_t ng, int i, int j){
    vertex_t v, w, nv;
    tuple_t t;
    edge_t e;
    v = graph_find_vertex_by_id(ng, i);
    w = graph_find_vertex_by_id(ng, j);

    if (v == NULL && w == NULL) {
        printf("At least one vertex must exist in graph, creating..\n");
        /* Create first vertex in graph */
        nv = (vertex_t) malloc(sizeof(struct vertex));
        assert (nv != NULL);
        vertex_init(nv);
        nv->id = i;
        graph_insert_vertex(ng, nv);
        v = graph_find_vertex_by_id(ng, i); //update v
    }
    if (v == NULL) {
        // Create a new vertex with i as its id
        v = (vertex_t) malloc(sizeof(struct vertex));
        assert (v != NULL);
        vertex_init(v);
        v->id = i;

        /* Create the vertex tuple based on its schema */
        if (ng->sv != NULL) {
            t = (tuple_t) malloc(sizeof(struct tuple));
            assert (t != NULL);
            tuple_init(t, ng->sv);
            v->tuple = t;
        }
        graph_insert_vertex(ng, v);

    } else if (w == NULL) {
        // Create a new vertex with j as its id
        w = (vertex_t) malloc(sizeof(struct vertex));
        assert (w != NULL);
        vertex_init(w);
        w->id = j;

        /* Create the vertex tuple based on its schema */
        if (ng->sv != NULL) {
            t = (tuple_t) malloc(sizeof(struct tuple));
            assert (t != NULL);
            tuple_init(t, ng->sv);
            w->tuple = t;
        }
        graph_insert_vertex(ng, w);
    }
    e = (edge_t) malloc(sizeof(struct edge));
    assert (e != NULL);
    edge_init(e);
    edge_set_vertices(e, i, j);

    /* Create the edge tuple based on its schema */
    if (ng->se != NULL) {
        t = (tuple_t) malloc(sizeof(struct tuple));
        assert (t != NULL);
        tuple_init(t, ng->se);
        e->tuple = t;
    }
    graph_insert_edge(ng, e);
}

static void
cli_graph_schema_add_base(
    graph_t ng,
    int old_schema_size,
    schema_type_t st,
    char *type,
    char *name)
{
    int i;

    for (i = 0; i < BASE_TYPES_MAX; i++) {
        if (strcasecmp(type, base_types_str[i]) == 0) {
#if _DEBUG
            printf("add %s to schema\n", type);
#endif
            attribute_t attr;
    
            attr = (attribute_t)
                malloc(sizeof(struct attribute));
            assert(attr != NULL);
            schema_attribute_init(attr, name, i, NULL);
            if (st == EDGE) {
                if (ng->se == NULL)
                    schema_init(&(ng->se));
                schema_attribute_insert(
                    ng->se, attr);
            } else if (st == VERTEX) {
                if (ng->sv == NULL)
                    schema_init(&(ng->sv));
                schema_attribute_insert(
                    ng->sv, attr);
            }
#if _DEBUG
            printf("update tuples\n");
#endif
            cli_graph_update_tuples(st, old_schema_size);

            break;
        }
    }
}

ArrayList get_low_cost(ArrayList* connections){
    ArrayList result;
    ArrayList nullRes;
    init(&nullRes);
    init(&result);
    int index = 0;
    //Initialize a HashTable
    dummyItem = (struct DataItem*) malloc(sizeof(struct DataItem));
    dummyItem->data = -1;  
    dummyItem->key = -1;

    for(int i=0;i<=connections->current;i++){
        if(!search(connections->elements[i].vertex1)){
            insert(connections->elements[i].vertex1,index);
            index++;
        }
        if(!search(connections->elements[i].vertex2)){
            insert(connections->elements[i].vertex2,index);
            index++;
        }
    }
    quickSort(connections, 0, connections->current);

    UnionFind_t uf;
    UnionFindCreate(&uf, index);

    for(int i=0;i<=connections->current;i++){
        int vertex1 = search(connections->elements[i].vertex1)->data;
        int vertex2 = search(connections->elements[i].vertex2)->data;
        if(isConnected(&uf,vertex1,vertex2)) continue;

        add(&result,connections->elements[i]);
        unionNodes(&uf,vertex1,vertex2);
    }

    if(uf.count!=1) return nullRes;
    return index-2==result.current?result:nullRes;
}


void cli_mst(char *cmdline, int *pos){

    ArrayList list;
    init(&list);

    graph_t g = graphs;
    edge_t e;
 
    for(g = graphs; g!=NULL; g = g->next){
        for(e = g->e; e != NULL; e = e->next){
            add(&list, (Element){get_cost(e->tuple,g->el),e->id1,e->id2});
        }
    }

    printf("Raw data:%d\n",list.current);
    print(&list);

    ArrayList res = get_low_cost(&list);
    printf("After ranking:%d\n",res.current);
    print(&res);

    /* Generate output graph */
    printf("Generating output graph:\n");
    graph_t ng;
    edge_t ne;

    /* Create new graph */
    ng = (graph_t) malloc(sizeof(struct graph));
    assert (ng != NULL);
    graph_init(ng);
    
    cli_graphs_insert(ng);
    cli_graph_schema_add_base(ng, 0, EDGE, "DOUBLE", "j");
    current = current->next;

    /*Create edges*/
    for (int i=0; i<=res.current; i++)
    {
    Element e = res.elements[i];
    create_edges(ng, (int)e.vertex1, (int)e.vertex2);
    ne = graph_find_edge_by_ids(current, (int)e.vertex1, (int)e.vertex2);


    char weight[15];
    sprintf(weight, "%f", e.cost);

    if (tuple_set(ne->tuple, "j", weight) < 0) {
        printf("Set edge tuple value failed\n");
        return;
    }

    }

}