#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jrb.h"
#include "jval.h"
#include "dllist.h"

#define MAX 10000000
#define INFINITIVE_VALUE 10000000

typedef struct Graph {
    JRB edges; //id
    JRB vertices; //string
    JRB chieuTree;
} Graph;

typedef struct XE {
    int iD;
    int soxe;
} XE;

Graph createGraph();
void dropGraph(Graph graph);
int strtolwr(char str[]);
int outdegree(Graph graph, int v, int* out);
double djikstra_shortest(Graph graph, int s, int t, int *path, int *length);
double getEdgeValue(Graph graph, int v1, int v2);
int findBus(Graph graph, int *S, int length);
int addGraph(Graph graph, JRB node, JRB newNode, int id);
int Insert(Graph graph, char *str, int n, int k, int* i);
int chooseBus(int **S, int xe[], int k);
int BusGo(Graph graph, JRB node1, JRB node2, int **S, int k);
JRB caydacbiet();
JRB traverseNode(Graph graph, char *str);
