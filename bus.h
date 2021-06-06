#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jrb.h"
#include "jval.h"
#include "dllist.h"
#include "tools.h"

#define MAX 10000000
#define INFINITIVE_VALUE 10000000

typedef struct Graph {
    JRB edges;
    JRB vertices;
    JRB chieuTree;
} Graph;

typedef struct XE {
    int iD;
    int soxe;
} XE;

Graph createGraph();
void dropGraph(Graph graph);
int outdegree(Graph graph, int v, int* out);
double djikstra_shortest(Graph graph, int s, int t, int *path, int *length);
double getEdgeValue(Graph graph, int v1, int v2);
int tuVanBus(Graph graph, int *S, int length);
void addGraph(Graph graph, JRB node, JRB newNode, int id);
void chonBusTheoTanXuat(int **S, int xe[], int k);
void getLuaChonBus(Graph graph, JRB node1, JRB node2, int **S, int k);
JRB timXe(Graph graph, char *str);
