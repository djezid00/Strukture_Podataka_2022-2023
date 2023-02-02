#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_SIZE (128)
#define MAX_LINE (1024)
#define FILE_OPENING_ERROR (-1)
#define ALLOCATION_ERROR (-2)

struct _artikal;
typedef struct _artikal* PositionA;
typedef struct _artikal {
	char imeArtikla[MAX_SIZE];
	int kolicina;
	double cijena;
	PositionA next;
}Artikal;

struct _date;
typedef struct _date* PositionD;
typedef struct _date {
	int godina;
	int mjesec;
	int dan;
}Date;

struct _racun;
typedef struct _racun* PositionR;
typedef struct _racun {
	char imeRacuna[MAX_SIZE];
	Artikal headA;
	PositionD date;
	PositionR next;
}Racun;


int CitanjeRacuna(PositionR head, char* filename);
PositionR CreateRacun(char* temp_ime);
int CitanjeArtikala(PositionR temp);
PositionA CreateArticle(char* temp_ime_artikla, int temp_kolicina, double temp_cijena);
int SortArticle(PositionA headA, PositionA tempA);
int InsertArticleAfter(PositionA p, PositionA tempA);
int PrintList(PositionR first);
int InsertRacun(PositionR head, PositionR temp);
int SortRacuna(PositionR head, PositionR temp);
int DateCmp(PositionD date1, PositionD date2);
PositionD CreateDate(int dan, int mjesec, int godina);
int IspisiArtikle(PositionA firstA);

bool IsDateInsideOfRange(PositionD date, PositionD from, PositionD to);
PositionA FindArticleByName(PositionA head, char* filter_name);

int main()
{
	Racun zeroth = { .imeRacuna = {0},.next = NULL,.headA.cijena = 0.0,.headA.imeArtikla = {0},.headA.kolicina = 0,.headA.next = NULL,.date = NULL };
	PositionR head = &zeroth;
	char filename[MAX_SIZE] = { 0 };
	char choice = 0;

	printf("Iz koje datoteke zelite citati racune:");
	scanf(" %s", filename);

	CitanjeRacuna(head, filename);

	PrintList(head->next);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	while (1)
	{
		printf("\nDo you want to queery an article (Y/N)\n");
		printf("-> ");
		scanf(" %c",&choice);
		choice = toupper(choice);
		switch (choice)
		{
			case 'Y':
				Querry(head);
				break;
			case 'N':
				return EXIT_SUCCESS;
		}
		
		
	}

	return EXIT_SUCCESS;
}



int CitanjeRacuna(PositionR head, char* filename)
{
	PositionR temp = NULL;
	FILE* fp = NULL;
	fp = fopen(filename, "r");
	char temp_ime[MAX_SIZE] = { 0 };

	if (!fp)
	{
		printf("\nERROR opening file!\n");
		return FILE_OPENING_ERROR;
	}

	while (!feof(fp))
	{

		if (fscanf(fp, " %s", temp_ime) == 1)
		{
			temp = CreateRacun(temp_ime);
			CitanjeArtikala(temp);
			SortRacuna(head, temp);
		}


	}

	fclose(fp);

	return EXIT_SUCCESS;
}

PositionR CreateRacun(char* temp_ime)
{
	PositionR temp = NULL;
	temp = (PositionR)malloc(sizeof(Racun));

	if (!temp)
	{
		printf("\nGreska alokacije!\n");
		return ALLOCATION_ERROR;
	}

	strcpy(temp->imeRacuna, temp_ime);
	temp->next = NULL;

	strcpy(temp->headA.imeArtikla, "");
	temp->headA.cijena = 0.0;
	temp->headA.kolicina = 0;
	temp->headA.next = NULL;

	return temp;
}

int CitanjeArtikala(PositionR temp)
{
	PositionA tempA = NULL;
	char BUFFER[MAX_SIZE] = { 0 };

	char filename_article[MAX_SIZE] = { 0 };
	strcpy(filename_article, temp->imeRacuna);
	char temp_ime_artikla[MAX_SIZE] = { 0 };
	int temp_kolicina = 0;
	double temp_cijena = 0.0;

	int temp_dan = 0;
	int temp_mjesec = 0;
	int temp_godina = 0;

	FILE* fp = NULL;
	fp = fopen(filename_article, "r");


	if (!fp)
	{
		printf("\nFile is not opened!\n");
		return FILE_OPENING_ERROR;
	}

	fgets(BUFFER, 1024, fp);
	if (sscanf(BUFFER, " %d %d %d", &temp_godina, &temp_mjesec, &temp_dan) == 3);
	temp->date = CreateDate(temp_dan, temp_mjesec, temp_godina);

	while (!feof(fp))
	{
		if (fscanf(fp, " %s %d %lf", temp_ime_artikla, &temp_kolicina, &temp_cijena) == 3)
		{
			tempA = CreateArticle(temp_ime_artikla, temp_kolicina, temp_cijena);
			SortArticle(&temp->headA, tempA);

		}
	}


	fclose(fp);

	return EXIT_SUCCESS;
}

PositionA CreateArticle(char* temp_ime_artikla, int temp_kolicina, double temp_cijena)
{
	PositionA tempA = NULL;
	tempA = (PositionA)malloc(sizeof(Artikal));

	if (!tempA)
	{
		printf("\nAllocation ERROR!\n");
		return ALLOCATION_ERROR;
	}

	strcpy(tempA->imeArtikla, temp_ime_artikla);
	tempA->cijena = temp_cijena;
	tempA->kolicina = temp_kolicina;
	tempA->next = NULL;

	return tempA;
}


int SortArticle(PositionA headA, PositionA tempA)
{
	PositionA p = headA;

	while (p->next != NULL && strcmp(p->next->imeArtikla, tempA->imeArtikla) < 0)
	{
		p = p->next;
	}

	InsertArticleAfter(p, tempA);

	return EXIT_SUCCESS;
}

int InsertArticleAfter(PositionA p, PositionA tempA)
{
	tempA->next = p->next;
	p->next = tempA;

	return EXIT_SUCCESS;
}

int PrintList(PositionR first)
{
	printf("--------------------------------------\n");
	while (first != NULL) {

		printf("IME RACUNA:%s %d%d%d\n", first->imeRacuna, first->date->godina, first->date->mjesec, first->date->dan);
		IspisiArtikle(first->headA.next);
		first = first->next;
	}
	return EXIT_SUCCESS;
}

int IspisiArtikle(PositionA firstA)
{
	while (firstA != NULL)
	{
		printf("Ime:%s \t Kolicina:%d \t Cijena:%.2f\n", firstA->imeArtikla, firstA->kolicina, firstA->cijena);
		firstA = firstA->next;
	}
	printf("--------------------------------------\n");
}

int InsertRacun(PositionR head, PositionR temp)
{
	temp->next = head->next;
	head->next = temp;

	return EXIT_SUCCESS;
}

int SortRacuna(PositionR head, PositionR temp)
{
	PositionR p = head;

	while (p->next != NULL && DateCmp(p->next->date, temp->date) < 0)
		p = p->next;
	InsertRacun(p, temp);

	return EXIT_SUCCESS;
}

int DateCmp(PositionD date1, PositionD date2)
{
	int result = date1->godina - date2->godina;

	if (result == 0)
	{
		result = date1->mjesec - date2->mjesec;

		if (result == 0)
		{
			result = date1->dan - date2->dan;
		}
	}


	return result;
}

PositionD CreateDate(int dan, int mjesec, int godina)
{
	PositionD newDate = NULL;

	newDate = (PositionD)malloc(sizeof(Date));

	if (!newDate)
	{
		printf("\nAllocation ERROR!\n");
		return ALLOCATION_ERROR;
	}
	newDate->dan = dan;
	newDate->mjesec = mjesec;
	newDate->godina = godina;

	return newDate;
}


int Querry(PositionR head)
{

	PositionR datum_racuna = head->next;
	PositionA article = &head->headA;
	PositionD from = NULL;
	PositionD to = NULL;
	int temp_godina = 0;
	int temp_mjesec = 0;
	int temp_dan = 0;
	char filter_name[MAX_SIZE] = { 0 };
	int totalCount = 0;
	float totalPrice = 0.0;


	printf("\nInsert beggining date in format: <YEAR>-<MONTH>-<DAY>:");
	scanf(" %d %d %d", &temp_godina, &temp_mjesec, &temp_dan);
	from = CreateDate(temp_dan, temp_mjesec, temp_godina);

	
	printf("\nInsert end date in format: <YEAR>-<MONTH>-<DAY>:");
	scanf(" %d %d %d", &temp_godina, &temp_mjesec, &temp_dan);
	to = CreateDate(temp_dan, temp_mjesec, temp_godina);


	printf("\nUnesite ime artikla:");
	scanf(" %s", filter_name);


	for (datum_racuna; datum_racuna != NULL; datum_racuna = datum_racuna->next)
	{
		if (IsDateInsideOfRange(datum_racuna->date, from, to))
		{

			PositionA article = FindArticleByName(&datum_racuna->headA, filter_name);

			if (article) {
				printf("\r\n");
				totalCount += article->kolicina;
				totalPrice += article->kolicina * article->cijena;
			}

		}

	}
	
	printf("Cijena:%.2lf\nKolicina:%d", totalPrice, totalCount);

	return EXIT_SUCCESS;
}


bool IsDateInsideOfRange(PositionD date, PositionD from, PositionD to)
{
	return DateCmp(date, from) >= 0 && DateCmp(date, to) <= 0;
}

PositionA FindArticleByName(PositionA head, char* filter_name)
{
	PositionA target = head->next;

	while (target != NULL)
	{
		if (strcmp(target->imeArtikla, filter_name) == 0)
			return target;
		target = target->next;
	}

	return EXIT_SUCCESS;
}
