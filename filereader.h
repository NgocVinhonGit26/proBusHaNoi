#include "bus.h"

/*
*Cac ham doc thanh phan,
*Neu tra lai -1 (EOF) tuc la khong doc duoc
*/

int getID(FILE* ptr) {
    int n = EOF;
    fscanf(ptr, "{id:%d}", &n);
    return n;
}

int getTuyen(FILE* ptr, char str[]) {
    return fscanf(ptr, "{tuyen:%[^}]}", str);
}

int getChieu(FILE* ptr, char str[]) {
    return fscanf(ptr, "{chieu:%[^}]}", str);
}

void insertGraph(Graph g, int id, char tuyen[], char chieu[], int* i) {
    JRB node = NULL, nodes = NULL;
    char* token;
    token = strtok(tuyen, "-");
    while (token != NULL)
    {
        JRB nodes = jrb_find_str(g.vertices, token);
        if (nodes == NULL) {
            jrb_insert_str(g.vertices, strdup(token), new_jval_i(*i));
            JRB tree = make_jrb();
            nodes = jrb_insert_int(g.edges, *i, new_jval_v(tree));
            addGraph(g, node, nodes, id);
            (*i)++;
        }
        else{
            nodes = jrb_find_int(g.edges, jval_i(nodes->val));
            addGraph(g, node, nodes, id);
        }
        node = nodes;
        token = strtok(NULL, "-");
    }
    jrb_insert_int(g.chieuTree, id, new_jval_s(strdup(chieu)));
}

void readFile(Graph g, FILE* ptr) {
    int i = 0;
    int id; 
    char str1[100000];
    char str2[100000];

    while(((id = getID(ptr)) != EOF)
        && (getTuyen(ptr, str1) != EOF)
        && (getChieu(ptr, str2) != EOF)) {
        fClearBuffer(ptr);
        strtolwr(str1);
        insertGraph(g, id, str1, str2, &i);
    }
}