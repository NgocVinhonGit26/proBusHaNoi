#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dllist.h"
#include "bus.h"

Graph createGraph() {
   Graph g; 
   g.edges = make_jrb();  
   g.vertices = make_jrb();
   g.chieuTree = make_jrb();
   return g;
}

void dropGraph(Graph graph) {
    JRB node, tree;
    jrb_traverse(node, graph.edges)
    {
        tree = (JRB) jval_v(node->val);
        jrb_free_tree(tree);
    }
    jrb_free_tree(graph.edges);
    jrb_free_tree(graph.vertices);
    jrb_free_tree(graph.chieuTree);
}

void addGraph(Graph graph, JRB node, JRB nodes, int id){
    JRB node1, node2;

    if( node == NULL || nodes == NULL) return;
    node1 = jrb_find_int((JRB)jval_v(node->val), jval_i(nodes->key));
    if(node1 != NULL){
        jrb_insert_int((JRB)jval_v(node1->val), id, new_jval_i(1));
    }
    else{
        JRB tree = make_jrb();
        jrb_insert_int((JRB)jval_v(node->val), jval_i(nodes->key), new_jval_v(tree));
        jrb_insert_int(tree, id, new_jval_i(1));
    }
}

int outdegree (Graph graph, int v, int* output)
{
    JRB tree, node;
    int total = 0;
    
    node  = jrb_find_int(graph.edges, v);
    if(node == NULL) return 0;
    tree = (JRB) jval_v(node->val);
    jrb_traverse(node, tree){
        output[total] = jval_i(node->key);
        total++;
    }
    return total;  
}

double getEdgeValue(Graph graph, int v1, int v2) {
    JRB node, tree;
    node = jrb_find_int(graph.edges, v1);
    if (node==NULL)
       return INFINITIVE_VALUE;
    tree = (JRB) jval_v(node->val);
    node = jrb_find_int(tree, v2);
    if (node==NULL)
       return INFINITIVE_VALUE;
    else
       return 1;       
}

double djikstra_shortest(Graph g, int s, int t, int *path, int *length){
    double Distance[100000], min;
    int Previous[100000] ,Visit[100000], u;
    int i;
    for(i = 0; i < 100000; i++){
        Distance[i] = INFINITIVE_VALUE;
        Visit[i] = 0;
        Previous[i] = 0;
    }
    Distance[s] = 0;
    Visit[s] = 1;
    Previous[s] = s;

    Dllist queue, ptr, node;

    queue = new_dllist();
    dll_append(queue, new_jval_i(s));

    while(!dll_empty(queue)){
        min = INFINITIVE_VALUE;
        dll_traverse(ptr, queue){
            u = jval_i(ptr->val);
            if(min > Distance[u]){
                min = Distance[u];
                node = ptr;
            }
        }
        u = jval_i(node->val);
        Visit[u] = 1;
        dll_delete_node(node);
        if(u == t) break;
        
        int out[100];
        int neighbor = outdegree(g, u, out);

        for(i = 0; i < neighbor; i++){
            int v = out[i];
            double w = getEdgeValue(g, u, v);
            if(Distance[v] > Distance[u] + w){
                Distance[v] = Distance[u] + w;
                Previous[v] = u;
            }
            if(Visit[v] == 0){
                dll_append(queue, new_jval_i(v));
            }
        }
    }

    double distance_s_t = Distance[t];
    int count = 0;
    if(Distance[t] != INFINITIVE_VALUE){
        path[count++] = t;
        while( t != s){
            t = Previous[t];
            path[count++] = t;
        }
        *length = count;
    }
    return distance_s_t;  
}

void chonBusTheoTanXuat(int **B, int xe[], int k){
    int count[1000000], i;

    for(i = 0; i < k; i++) xe[i] = 0;
    for(i = 0; i < 1000000; i++) count[i] = 0;
    for(i = 0; i < k; i++){
        for(int j = 0; B[i][j] != 0; j++){
            count[B[i][j]]++;
        }
    }
    int ln;
    while(1){
        ln = count[0]; 
        i = 0;
        for(int j = 0; j < 1000000; j++){
            if(count[j] > ln){
                ln = count[j];
                i = j;
            }
        }
        for (int h = 0; h < k; h++){
            for (int j = 0; B[h][j] != 0; j++){
                if(B[h][j] == i){
                    if(xe[h] == 0) xe[h] = i;
                }
            }
        }
        for(int p = 0; xe[p] != 0; p++){
            if(p == (k-1)) return;
        }
        count[i] = 0;
    }
}

void getLuaChonBus(Graph graph, JRB truoc, JRB sau, int **B, int k){
    if(truoc == NULL || sau == NULL) return;
    JRB tree = jrb_find_int(graph.edges, jval_i(truoc->val));

    tree = jrb_find_int((JRB)jval_v(tree->val), jval_i(sau->val));

    tree = (JRB)jval_v(tree->val);

    JRB node;
    int i = 0;

    jrb_traverse(node, tree){
        B[k][i++] = jval_i(node->key);
    }
    B[k][i] = 0;
}

int tuVanBus(Graph graph, int *path, int length){
    JRB nodes = NULL;
    int i;
    int k = -1; //So lua chon
    
    char **str = (char **)malloc(sizeof(char) * length);
    for(i = 0; i < length; i++) 
        str[i] = (char *)malloc(sizeof(char)* MAX);
    
    int  **B = (int**)malloc(sizeof(int *) * length);
    for(i = 0; i < length; i++)
        B[i] = (int *)malloc(sizeof(int) * 1000);
        
    for(i = length - 1; i >= 0; i--){
        JRB node;
        jrb_traverse(node, graph.vertices){
            if(jval_i(node->val) == path[i]){
                strcpy(str[k+1], jval_s(node->key));
                getLuaChonBus(graph, nodes, node, B, k++);
                nodes = node;
            }
        }
    }

    int xe[1000000], j = 0;

    chonBusTheoTanXuat(B, xe, k);
    
    printf("Chi dan :\n");
    for(i = 0; i < k; i++){
        JRB Node = jrb_find_int(graph.chieuTree, xe[i]);
        if(Node != NULL){
            if(xe[i] == xe[i-1] && i >= 1){
                printf("-> %s", str[++j]);
            }
            else{
                printf("\n-%s : %s", jval_s(Node->val), str[j]);
                printf("-> %s", str[++j]);
            }
        }
        else printf("Error: Khong tim thay ben xe.\n");
    }
    printf("\n");
}

JRB timXe(Graph graph, char *str) {
    char *Data = (char *)malloc(sizeof(char)*1000);
    JRB Node;
    strtolwr(str);
    int i;
    Node = jrb_find_str(graph.vertices, str);
    if(Node == NULL) {
        jrb_traverse(Node, graph.vertices) {
            i = 0;
            strcpy(Data, jval_s(Node->key));
            if(strlen(Data) > strlen(str)) {
                while(i < strlen(Data)) {
                    if(strncmp(str, Data+i, strlen(str)) == 0) return Node;
                    i++;
                }
            }
            else {
                while(i < strlen(Data)) {
                    if(strncmp(str, Data+i, strlen(str)) == 0) return Node;
                    i++;
                }
            }   
        }
        return NULL;
    }
    else return Node;
}

