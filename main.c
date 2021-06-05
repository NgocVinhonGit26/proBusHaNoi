#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "bus.h"

void menu(){
    printf("Dich vu Bus Ha Noi han hanh phuc vu quy vi.\n");
    printf("Menu dich vu:\n1.Tu van duong di.\n2.Thoat chuong trinh\n");
}

void clearBuffer(){
    while(getchar() != '\n');
}

void case1(Graph graph, JRB tree){
    char *start = (char *)malloc(sizeof(char) *1000);
    char *to = (char *)malloc(sizeof(char) *1000);

    printf("Chon diem xuat phat: ");
    scanf("%[^\n]", start);
    clearBuffer();
    printf("Chon diem can den: ");
    scanf("%[^\n]", to);
    clearBuffer();
    
    JRB node1 = TraveNode(graph, start);
    JRB node2 = TraveNode(graph, to);

    while(node1 == NULL || node2 == NULL || node1 == node2){
        // printf("test4\n");
        printf("Qui khach nhap sai thong tin!!! Vui long nhap lai: \n");
        printf("Chon diem xuat phat: ");
        scanf("%[^\n]", start);
        clearBuffer();
        printf("Chon diem can den: ");
        scanf("%[^\n]", to);
        clearBuffer();
        node1 = TraveNode(graph, start);
        node2 = TraveNode(graph, to);
    }
    // printf("test3\n");
    int Start = jval_i(node1->val);
    int To = jval_i(node2->val);
    int *path = (int*)malloc(sizeof(int) *10000), length;
    double a = MinPath(graph, Start, To, path, &length);
    // printf("test5\n");
    if(a < MAX) findBus(graph, path, length, tree);
    free(path);
    free(start);
    free(to);

    // if(Start == To) printf("Diem xuat phat va diem den bi trung!!!\n");
}

void case2(Graph graph, FILE *ptr, JRB tree){
    printf("\nCam on quy khach da su dung dich vu. Kinh chao va hen gap lai.\n");
    dropGraph(graph);
    jrb_free_tree(tree);
    fclose(ptr);
}

int main(){
    Graph graph = creartGraph();
    FILE *ptr = fopen("bus.txt", "r");
    JRB tree = caydacbiet();
    int choose, n = 1;

    // printf("test1\n");
    readFile(graph, ptr);
    // printf("test2\n");
    while(n == 1){
        menu();
        printf("Moi ban chon: ");
        scanf("%d", &choose);
        clearBuffer();
        switch(choose){
            case 1:{
                case1(graph, tree);
                break;
            }
            case 2:{
                case2(graph, ptr, tree);
                n = 0;
                break;
            }
        }
    }
    return 0;
}
