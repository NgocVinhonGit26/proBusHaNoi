#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "filereader.h"

void menu(){
    printf("Dich vu Bus Ha Noi han hanh phuc vu quy vi.\n");
    printf("Menu dich vu:\n1.Tu van duong di.\n2.Thoat chuong trinh\n");
}

void case1(Graph graph){
    char *start = (char *)malloc(sizeof(char) *1000);
    char *to = (char *)malloc(sizeof(char) *1000);

    printf("Chon diem xuat phat: ");
    scanf("%[^\n]", start);
    strtolwr(start);
    clearBuffer();
    printf("Chon diem can den: ");
    scanf("%[^\n]", to);
    strtolwr(to);
    clearBuffer();
    
    JRB node1 = timXe(graph, start);
    JRB node2 = timXe(graph, to);

    while(node1 == NULL || node2 == NULL || node1 == node2){
        printf("Qui khach nhap sai thong tin!!! Vui long nhap lai: \n");
        printf("Chon diem xuat phat: ");
        scanf("%[^\n]", start);
        strtolwr(start);
        clearBuffer();
        printf("Chon diem can den: ");
        scanf("%[^\n]", to);
        strtolwr(to);
        clearBuffer();
        node1 = timBen(graph, start);
        node2 = timBen(graph, to);
    }
    int Start = jval_i(node1->val);
    int To = jval_i(node2->val);
    int *path = (int*)malloc(sizeof(int) *10000), length;
    double a = djikstra_shortest(graph, Start, To, path, &length);
    if(a < MAX) tuVanBus(graph, path, length);
    free(path);
    free(start);
    free(to);
}

void case2(Graph graph, FILE *ptr){
    printf("\nCam on quy khach da su dung dich vu. Kinh chao va hen gap lai.\n");
    dropGraph(graph);
    fclose(ptr);
}

int main(){
    Graph graph = createGraph();
    FILE *ptr = fopen("buslines.txt", "r");
    int choose, n = 1;

    readFile(graph, ptr);
    while(n == 1){
        menu();
        printf("Moi ban chon: ");
        scanf("%d", &choose);
        clearBuffer();
        switch(choose){
            case 1:{
                case1(graph);
                break;
            }
            case 2:{
                case2(graph, ptr);
                n = 0;
                break;
            }
        }
    }
    return 0;
}
