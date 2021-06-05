#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dllist.h"
#include "bus.h"

Graph creartGraph(){
    Graph g;
    g.BEN = make_jrb();
    g.DIEM = make_jrb();
    return g;
}

int dropGraph(Graph graph){
    jrb_free_tree(graph.BEN);
    jrb_free_tree(graph.DIEM);
    return 0;
}

int Kituthuong(char str[]){
    int i;
    for( i = 0; i < strlen(str); i++){
        str[i] = tolower(str[i]);
    }
    return 1;
}

int addGraph(Graph graph, JRB node, JRB nodes, int n, int k){
    JRB node1, node2;

    if( node == NULL || nodes == NULL) return 1;
    node1 = jrb_find_int((JRB)jval_v(node->val), jval_i(nodes->key));
    if(node1 != NULL){
        jrb_insert_int((JRB)jval_v(node1->val), (n*10+k), new_jval_i(1));
    }
    else{
        JRB tree = make_jrb();
        jrb_insert_int((JRB)jval_v(node->val), jval_i(nodes->key), new_jval_v(tree));
        jrb_insert_int(tree, (n*10+k), new_jval_i(1));
    }
    return 1;
}

int Insert(Graph graph, char *str, int n, int k, int *i){
    JRB node = NULL, nodes = NULL;
    char *token = (char *)malloc(sizeof(char) *1000);

    token = strtok(str, "-");
    while (token != NULL)
    {
        Kituthuong(token);
        nodes = jrb_find_str(graph.DIEM, token);
        if(nodes == NULL){
            JRB tree = make_jrb();
            jrb_insert_str(graph.DIEM, token, new_jval_i(*i));
            nodes = jrb_insert_int(graph.BEN, *i, new_jval_v(tree));
            (*i)++;
            addGraph(graph, node, nodes, n, k);
        }
        else{
            nodes = jrb_find_int(graph.BEN, jval_i(nodes->val));
            addGraph(graph, node, nodes, n, k);
        }
        node = nodes;
        token = strtok(NULL, "-");
    }
   
}

int readFile(Graph graph, FILE *ptr){
    // FILE *ptr = fopen("bus.txt", "r");
    int n, i = 0, h = 0;

    while(!feof(ptr)){
        char *s1 = (char *)malloc(sizeof(char)*10000000);
        char *s2 = (char *)malloc(sizeof(char)*10000000);
        int k = fscanf(ptr, "%d", &n);

        fseek(ptr, sizeof(char), SEEK_CUR);
        fscanf(ptr, "%[^\n]", s1);
        fseek(ptr, sizeof(char), SEEK_CUR);
        fscanf(ptr, "%[^\n]", s2);
        Insert(graph, s1, n, 8, &i);
        Insert(graph, s2, n, 9, &i);
    }
    // return 1;
}

int outdegree(Graph graph, int v, int* out){
    JRB tree, node;
    int total = 0;
    
    node  = jrb_find_int(graph.BEN, v);
    if(node == NULL) return 0;
    tree = (JRB) jval_v(node->val);
    jrb_traverse(node, tree){
        out[total] = jval_i(node->key);
        total++;
    }
    return total;
}

double getEdgeVal(Graph graph, int v1, int v2){
    JRB node = jrb_find_int(graph.BEN, v1);
    
    if(node == NULL) return INFINITIVE_VALUE;

    JRB tree = (JRB)jval_v(node->val);
    JRB nodes = jrb_find_int(tree, v2);

    if(nodes == NULL) return INFINITIVE_VALUE;
    return 1;
}

double MinPath(Graph graph, int s, int t, int *path, int *length){
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
        int neighbor = outdegree(graph, u, out);

        for(i = 0; i < neighbor; i++){
            int v = out[i];
            double w = getEdgeVal(graph, u, v);
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

int chooseBus(int **S, int xe[], int k){
    int count[1000000], i;

    for(i = 0; i < k; i++) xe[i] = 0;
    for(i = 0; i < 1000000; i++) count[i] = 0;
    for(i = 0; i < k; i++){
        for(int j = 0; S[i][j] != 0; j++){
            count[S[i][j]]++;
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
            for (int j = 0; S[h][j] != 0; j++){
                if(S[h][j] == i){
                    if(xe[h] == 0) xe[h] = i;
                }
            }
        }
        for(int p = 0; xe[p] != 0; p++){
            if(p == (k-1)) return 1;
        }
        count[i] = 0;
    }
}

int BusGo(Graph graph, JRB node1, JRB node2, int **S, int k){
    if(node1 == NULL || node2 == NULL) return 1;
    JRB node3 = jrb_find_int(graph.BEN, jval_i(node1->val));

    node3 = jrb_find_int((JRB)jval_v(node3->val), jval_i(node2->val));

    node3 = (JRB)jval_v(node3->val);

    JRB node4;
    int i = 0;

    jrb_traverse(node4, node3){
        S[k][i++] = jval_i(node4->key);
    }
    S[k][i] = 0;
    return 1;
}

int findBus(Graph graph, int *S, int length, JRB tree){
    JRB nodes = NULL;
    int i;

    printf("Chi dan :\n");
    char **str = (char **)malloc(sizeof(char) * MAX);
    for(i = 0; i < 100; i++) str[i] = (char *)malloc(sizeof(char)* MAX);
    int  **B = (int**)malloc(sizeof(int *) *1000), k = -1;
    for(i = 0; i < 1000; i++){
        B[i] = (int *)malloc(sizeof(int) * 1000);
    }
    for(i = length - 1; i >= 0; i--){
        JRB node;
        jrb_traverse(node, graph.DIEM){
            if(jval_i(node->val) == S[i]){
                strcpy(str[k+1], jval_s(node->key));
                BusGo(graph, nodes, node, B, k++);
                nodes = node;
            }
        }
    }

    int xe[1000000], j = 0;
    // int i;

    chooseBus(B, xe, k);;
    for(i = 0; i < k; i++){
        JRB Node = jrb_find_int(tree, xe[i]);
        if(Node != NULL){
            if(xe[i] == xe[i-1] && i >= 1){
                printf("-> %s", str[++j]);
            }
            else{
                printf("\n-%s : %s", jval_s(Node->val), str[j]);
                printf("-> %s", str[++j]);
            }
        }
        else{
            if(xe[i] == xe[i-1] && i >= 1){
                printf(" -> %s", str[++j]);
                int j = 0;
            }
            else{
                printf("\n- %d : %s", xe[i], str[j]);
                printf(" -> %s", str[++j]);
            }
        }
    }
    printf("\n");
}

JRB caydacbiet(){
    JRB tree = make_jrb();
    FILE *ptr = fopen("chieuxe.txt", "r");
    int n;
    char *str;

    while(!feof(ptr)){
        str = (char *)malloc(sizeof(char)*10);
        fscanf(ptr, "%d %[^\n]", &n, str);
        jrb_insert_int(tree, n, new_jval_s(str));
    }
    fclose(ptr);
    free(str);
    return tree;
}

JRB TraveNode(Graph graph, char *str){
    char *Data = (char *)malloc(sizeof(char)*1000);
    JRB Node;
    Kituthuong(str);
    int i;
    Node = jrb_find_str(graph.DIEM, str);
    if(Node == NULL){
        jrb_traverse(Node, graph.DIEM){
            i = 0;
            strcpy(Data, jval_s(Node->key));
            if(strlen(Data) > strlen(str))
            {
                while(i < strlen(Data)){
                if(strncmp(str, Data+i, strlen(str)) == 0) return Node;
                i++;
                }
            }
            else
            {
                while(i < strlen(Data)){
                if(strncmp(str, Data+i, strlen(str)) == 0) return Node;
                i++;
                }
            }   
        }
        return NULL;
    }
    else return Node;
}

