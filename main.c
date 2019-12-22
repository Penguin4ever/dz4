
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


void Find(char **table, int amount, char arrow[3]){
    int degree[amount], min_degree = 256;
    
    for(int i=0 ;i<amount; i++){//подсчет степени вершины
        degree[i] = 0;
        for(int j=0; j<amount; j++){//собственно он самый
            degree[i] += table[i][j+1];
            degree[i] += table[j][i+1];
        }
        if(min_degree > degree[i]){//находим минимальную вершину
            min_degree = degree[i];
        }
    }
    
    printf("\nвсе ребра с минимальной вершиной = %d:\n",min_degree);
    for(int addr=0; addr < amount; addr++){//проходит по строкам 
        if(degree[addr] == min_degree){//проверка являетца ли данная вершина вершиной с мин степенью  
            for(int i=0; i<amount; i++)//проходит по сталбцам
                if( table[addr][i+1] > 0)//проверка на наличие в яцейки записи больше 0
                    for(int j=0; j<table[addr][i+1]; j++)// если в ячейке больше чем 1 выводит больше таких записей
                        printf("%c%s%c; ", table[addr][0], arrow, table[i][0]);

            for( int i=0; i<amount; i++ )//таже самая ток наоборот 
                if( table[i][addr+1] > 0 && addr != i && degree[i] != min_degree)
                    for( int j=0; j<table[i][addr+1]; j++ )
                        printf("%c%s%c; ", table[i][0], arrow, table[addr][0]);
        }
    }
    printf("\n");
}



int main(){
    int mode;
    printf("Введите 0 для ориентированного графа \nили 1 для неориентированного\n\n");
    scanf("%d", &mode);
    
    if(!( mode == 1 || mode == 0 ))
        exit(101);
    
    printf("Введите количество вершин::\n> ");
    int amount;
    scanf("%d",&amount);
    if (amount <= 0){
        printf("Не пойдет, давай по новой\n");
        exit(102);
    }
    
    getchar(); // без него не работаит. пропускае /n
    printf("Название вершиш\n");
    
    char **table; //указатель на указатель тк двойной массив
    table = calloc(amount, sizeof(char*));// тк массив 2-ой 
    for(int i=0; i<amount; i++){
        table[i] = calloc(amount+1, sizeof(char));
    }
    
    for(int i=0; i<amount; i++){
        printf("%d: ",i);
        table[i][0] = getchar(); // по названию
        getchar(); //пропускает слэщ эн

        if(!(('A' <= table[i][0] && table[i][0] <= 'Z') || ('a' <= table[i][0] && table[i][0] <= 'z'))){
            printf("Введите букву!\n");// проверяет шобы были буковки
            i--; // -- если ошибка
        } else {
            for(int j=0; j<i; j++){
                if( table[i][0] == table[j][0] ){ // если совпадают буквы
                    printf("Введите другую букву\n");
                    i--;
                }
            }
        }
    }
        
    for(int i=0; i<amount; i++){
        for(int j=1; j<amount; j++){
            table[i][j] = 0; // обнуление всего кроме букв
        }
    }
    
    printf("Для каждой вершины введите связь\n");
    for(int i=0; i<amount; i++){
        printf("%c -> ", table[i][0]);
        char c;
        int flag = 1;// пока юзер не ввел слэщ эн
        while(flag){//пока не равен 0
        scanf("%c", &c);
            if(c == '\n'){
                flag = 0;
            } else {// если не равняется то записываем
                if(c-'0' <= amount){
                    table[i][c-'0']++;
                }
            }
        }
    }

    printf("\n");
    int graph_check = 1;
    // проверка крестом, как только появляется крест со всеми 0 граф становится несвязанным
    for(int i=0; i<amount; i++){
        int temp_graph_check = 0; 
        for(int j=0; j<amount; j++){
            if(table[i][j+1] == 1)
                temp_graph_check = 1;
    
            if(table[j][i+1] == 1)
                temp_graph_check = 1;
        }
        if(temp_graph_check == 0)
            graph_check=0;
    }
    
    if(graph_check == 0){
        printf("Граф несвязанный\n");
    } else {
        printf("Граф свзяанный\n");

    }
    printf("--------------------\n");

    for(int i=0; i<amount; i++) {
        
        printf("%c: ", table[i][0]);
        
        for (int j=1; j<amount+1; j++) {
            printf("%d  ", table[i][j]);
        }

        printf("\n");
    }

    char command[1024] = {0};
    char arrow[3] = "";
    
    strcat(command, "echo ' ");
    if(mode == 0){
        strcat(command, "digraph G {");
        strcat(arrow, "->");
    } else {
        strcat(command, "graph G {");
        strcat(arrow, "--");
    }
    
    Find(table, amount, arrow);

    for(int i=0; i<amount; i++){
        char elem[3] = "";
        elem[0] = table[i][0];
        strcat(command, elem);
        strcat(command, "; ");
    }
    for(int i=0; i<amount; i++){
        for(int j=0; j<amount; j++){
            for(int k=0; k<table[i][j+1]; k++){
                char elem[2] = "";
                elem[0] = table[i][0];
                
                strcat(command, elem);
                strcat(command, arrow);
                
                elem[0] = table[j][0];
                strcat(command, elem);
                strcat(command, "; ");
            }
        }
    }
    strcat(command, "}' | dot -Tpng > ./graph.png");
    system(command);
    
    return 0;
}
