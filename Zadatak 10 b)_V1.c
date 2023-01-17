#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_SIZE (128)
#define MAX_LINE (1024)

struct _city;
typedef struct _city* Position;
typedef struct _city {
    char city_name[MAX_SIZE];
    int population;
    Position next;
}City;

struct _country;
typedef struct _country* Branch;
typedef struct _country {
    char country_name[MAX_SIZE];
    Branch L;
    Branch D;
    City head;
}Country;

Branch CountryFile(Branch root, char* fileName);
Branch Tree(Branch root, char* imeD, char* fileName);
Branch CreateTree(char* imeD);
int CityFile(Branch el, char* fileName);
int Sort(Position head, char* imeG, int broj);
int Create_Country(Position el, char* imeG, int broj);
int Insert(Position el, Position new);
int Print_Country(Branch root);
int Print_City(Position head);
int Querry_Country(Branch root);
Branch Querry_Tree(Branch root, char* ime);
int Querry_City(Position head, int broj, int* i);
int FreeAll(Branch root);
int FreeList(Position head);

int main()
{
    Branch root =NULL;
    char filename[MAX_SIZE] = { 0 };
    int status = 12;

    printf("\nFrom which file you want to read countries:");
    scanf(" %s",filename);
    root = CountryFile(root, filename);

    Print_Country(root);

    Querry_Country(root);
   
    status = FreeAll(root);
    if (NULL == status) {
        printf("\nDeallocation successfull!");
    }
    else {
        printf("\nDeallocation unsuccessfull!");
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}

Branch CountryFile(Branch root, char* fileName)
{
    FILE* fp = fopen(fileName, "r");
    char BUFFER[MAX_SIZE] = { 0 };
    char temp_country_name[MAX_SIZE] = { 0 };
    char filename_city[MAX_SIZE] = { 0 };

    if (!fp)
    {
        printf("Datoteka nije otvorena!\n");
        return -1;
    }

    while (!feof(fp))
    {
        fgets(BUFFER, MAX_LINE, fp);

        if (sscanf(BUFFER, " %s %s", temp_country_name, filename_city) == 2)
            root = Tree(root, temp_country_name, filename_city);
    }

    fclose(fp);

    return root;
}

Branch Tree(Branch root, char* country_name, char* fileName)
{
    if (!root)
    {
        root = CreateTree(country_name);
        CityFile(root, fileName);
    }

    else if (strcmp(root->country_name, country_name) > 0)
        root->L = Tree(root->L, country_name, fileName);

    else
        root->D = Tree(root->D, country_name, fileName);

    return root;
}

Branch CreateTree(char* country_name)
{
    Branch newTree = NULL;
    newTree = (Branch)malloc(sizeof(Country));
    if (!newTree)
    {
        printf("\nAllocation ERROR!\n");
        return -2;
    }

    strcpy(newTree->country_name, country_name);
    strcpy(newTree->head.city_name, "");
    newTree->head.population = 0;
    newTree->head.next = NULL;
    newTree->L = NULL;
    newTree->D = NULL;

    return newTree;
}

int CityFile(Branch el, char* fileName)
{
    FILE* fp = fopen(fileName, "r");
    char BUFFER[MAX_SIZE] = { 0 };
    char city_name[MAX_SIZE] = { 0 };
    int population = 0;

    if (!fp)
    {
        printf("File is not opened\n");
        return -1;
    }

    while (!feof(fp))
    {
        fgets(BUFFER, 1024, fp);

        if (sscanf(BUFFER, " %s %d", city_name, &population) == 2)
            Sort(&el->head, city_name, population);
    }

    fclose(fp);

    return EXIT_SUCCESS;
}

int Sort(Position head, char* city_name, int population)
{
    Position tempPrev = head;
    Position tempNext = head->next;

    while (tempNext)
    {
        if (tempNext->population > population)
        {
            Create_Country(tempPrev, city_name, population);
            return EXIT_SUCCESS;
        }

        else if (tempNext->population == population)
            if (strcmp(tempNext->city_name, city_name) > 0)
            {
                Create_Country(tempPrev, city_name, population);
                return EXIT_SUCCESS;
            }

        tempPrev = tempPrev->next;
        tempNext = tempNext->next;
    }

    Create_Country(tempPrev, city_name, population);

    return EXIT_SUCCESS;
}

int Create_Country(Position el, char* city_name, int population)
{
    Position new = NULL;

    new = (Position)malloc(sizeof(City));
    if (!new)
    {
        perror("Allocation ERROR!\n");
        return -1;
    }

    strcpy(new->city_name, city_name);
    new->population = population;
    new->next = NULL;

    Insert(el, new);

    return EXIT_SUCCESS;
}

int Insert(Position el, Position new)
{
    new->next = el->next;
    el->next = new;

    return EXIT_SUCCESS;
}

int Print_Country(Branch root)
{
    if (root->L)
        Print_Country(root->L);

    printf("%s\n", root->country_name);
    Print_City(&root->head);

    if (root->D)
        Print_Country(root->D);

    return EXIT_SUCCESS;
}

int Print_City(Position head)
{
    while (head->next)
    {
        printf("    %s\n", head->next->city_name);
        head = head->next;
    }

    return EXIT_SUCCESS;
}

int Querry_Country(Branch root)
{
    char name[MAX_SIZE] = { 0 };
    int num = 0, i = 0;
    Branch temp = NULL;

    printf("What country you want to search?\n");
    scanf(" %s", name);
    temp = Querry_Tree(root, name);
    if (!temp)
    {
        printf("Country does not exist!\n");
        return -1;
    }

    printf("Enter number for comparision:\n");
    scanf(" %d", &num);
    Querry_City(&temp->head, num, &i);

    return EXIT_SUCCESS;
}

Branch Querry_Tree(Branch root, char* ime)
{
    if (!strcmp(root->country_name, ime))
        return root;

    else if (strcmp(root->country_name, ime) > 0)
        Querry_Tree(root->D, ime);

    else
        Querry_Tree(root->L, ime);

    return NULL;
}

int Querry_City(Position head, int broj, int* i)
{
    while (head->next)
    {
        if (head->next->population > broj)
        {
            (*i)++;
            printf("%s\n", head->next->city_name);
        }
        head = head->next;
    }

    return EXIT_SUCCESS;
}

int FreeAll(Branch root)
{
    if (!root)
        return EXIT_SUCCESS;

    FreeList(&root->head);

    FreeAll(root->L);

    FreeAll(root->D);

    free(root);

    return EXIT_SUCCESS;
}

int FreeList(Position head)
{
    Position temp = NULL;

    while (head->next)
    {
        temp = head->next;
        head->next = temp->next;
        free(temp);
    }

    return EXIT_SUCCESS;
}