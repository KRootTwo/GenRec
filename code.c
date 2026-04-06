#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>

size_t l = 0;
typedef struct {
    int id;
    char *name;
    float price;
    int amt;
} items;

void dispStore(items *j, size_t s) {
    printf("+-------------------------+--------+\n");
    printf("\033[31m| %s | %-18s | %6s |\033[0m\n", "sl", "item name", "price");
    for (size_t i = 0; i < s; i++) {
        printf("| %zu  | %-18s | %6.2f |\n", i+1, j[i].name, j[i].price);
    }
    printf("+-------------------------+--------+\n");
}
void purchase(items *j, size_t s) {
    int il = (int) s;
    char blehh[20];
    printf("\nEnter sl no. to purchase that pdt (0 to exit)\n");
    int i;
    while (true) {
        start:
        printf("sl: ");
        fgets(blehh, sizeof(blehh), stdin);
        sscanf(blehh, "%d", &i);
        if (i == 0) break;
        else if (i > il) {
            printf("Invalid sl\n");
            goto start;
        }
        size_t k;
        for (k = 0; k < s; k++) {
            if (j[k].id == i) goto sel;
        }
        sel :
        printf("How many %s pens you wanna buy? ", j[k].name);
        fgets(blehh, sizeof(blehh), stdin);
        sscanf(blehh, "%d", &j[k].amt);
    }
    printf("Done purchasing nigga\n");
}
float totalAmt(items *j, size_t s) {
    float t = 0;
    for (size_t i = 0; i < s; i++) {
        if (j[i].amt != 0) {
            t += j[i].amt * j[i].price;
        }
    }
    return t;
}
int totalPc(items *j, size_t s) {
    int t = 0;
    for (size_t i = 0; i < s; i++) {
        if (j[i].amt != 0) {
            t += j[i].amt;
        }
    }
    return t;
}
int main() {
    char line[100];
    int n = 0;

    // init fp
    FILE *fp = fopen("/home/ktv/CodeShit/D/genRec/items.csv", "r");
    if (fp == NULL) {
        printf("Error opening file nigga\n");
        return 1;
    }

    // getting no. of items
    while (fgets(line, sizeof(line), fp) != NULL) {
        l++;
    }

    // rewind that bitch
    rewind(fp);

    // dma & init
    items *i = malloc(sizeof(items) * l);
    while (fgets(line, sizeof(line), fp)) {
        i[n].id = atoi(strtok(line, ","));

        char *name = strtok(NULL, ",");
        i[n].name = malloc(strlen(name) + 1);
        strcpy(i[n].name, name);

        i[n].price = atof(strtok(NULL, ","));
        n++;
    }
    // close file pointer
    fclose(fp);

    // display`
    dispStore(i, l);
    purchase(i, l);

    FILE *fp2 = fopen("/home/ktv/CodeShit/D/genRec/rC.txt", "r+");
        if (fp2 == NULL) {
        printf("Error opening file nigga\n");
        return 1;
    }

    // receipt name
    int g = 0;
    while (fgets(line, sizeof(line), fp2)) {
        g = atoi(line);
    }
    char filename[50];
    sprintf(filename, "/home/ktv/CodeShit/D/genRec/Rs/reciept_%d.txt", g++);
    rewind(fp2);
    fprintf(fp2, "%d", g);

    // receipt generation
    FILE *fp3 = fopen(filename, "w");
    fprintf(fp3, "%28s", "RECEIPT\n");

    fprintf(fp3, "+-------------------------+---------+--------+\n");

    for (size_t j = 0; j < l; j++) {
        if (i[j].amt != 0) {
            fprintf(fp3, "| %2zu   %-18s |   %3d   | %6.2f |\n",
                    j+1, i[j].name, i[j].amt, i[j].price * i[j].amt);
        }
    }

    fprintf(fp3, "+-------------------------+---------+--------+\n");

    fprintf(fp3, "| %-23s |   %3d   | %6.2f |\n",
            "Total", totalPc(i, l), totalAmt(i, l));

    fprintf(fp3, "+-------------------------+---------+--------+\n");

    fclose(fp2);
    fclose(fp3);
    // free
    for (size_t k = 0; k < l; k++) {
        free(i[k].name);
    }
    free(i);
    i = NULL;
    return 0;
}