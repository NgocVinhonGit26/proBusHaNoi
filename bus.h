#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jrb.h"
#include "jval.h"
#include "dllist.h"

#define MAX 10000000
#define INFINITIVE_VALUE 10000000

typedef struct{
    JRB BEN; //id
    JRB DIEM; //string
} Graph;

typedef struct{
    int iD;
    int soxe;
} XE;

Graph creartGraph();
int dropGraph(Graph graph);
int outdegree(Graph graph, int v, int* out);
double MinPath(Graph graph, int s, int t, int *path, int *length);
double getEdgeVal(Graph graph, int v1, int v2);
int findBus(Graph graph, int *S, int length, JRB tree);
int addGraph(Graph graph, JRB node, JRB newNode, int n, int k);
int Insert(Graph graph, char *str, int n, int k, int* i);
int readFile(Graph graph, FILE *ptr);
int chooseBus(int **S, int xe[], int k);
int BusGo(Graph graph, JRB node1, JRB node2, int **S, int k);
int Kituthuong(char str[]);
JRB caydacbiet();
JRB TraveNode(Graph graph, char *str);
