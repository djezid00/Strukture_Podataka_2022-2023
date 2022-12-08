#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 128
#define MAX_LINE 1024

struct _racun;
struct _artikl;
typedef struct _racun* PozicijaR;
typedef struct _artikl* PozicijaA;

typedef struct _artikl {
	char imeArtikla[MAX_SIZE];
	int kolicina;
	int cijena;
	PozicijaA nextA;

}Artikl;

typedef struct _racun {
	char imeRacuna[MAX_SIZE];
	int godina;
	int mjesec;
	int dan;
	PozicijaR nextR;
	Artikl headAr;

}Racun;

int CitanjeRacunaIzDatoteke(PozicijaR head, char* filename);
PozicijaR CreateNewRacun(char* imeracuna);
int CitanjeArtikalaIzDatoteke(PozicijaR temp);
PozicijaA CreateArticle(char* tempImeArtikla, int  temp_kolicina, int temp_cijena);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int UnosUListuArtikala(PozicijaA head, PozicijaA newArticle);
int InsertAfter(PozicijaA head, PozicijaA newArticle);
int UnosUListuSortirano(PozicijaR head, PozicijaR temp);
int IspisiRacune(PozicijaR first);
int IspisiArtikle(PozicijaA first);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~





int main()
{
	char filename[] = "racuni.txt";
	Racun zeroth = { .nextR = NULL,.imeRacuna = {0},.dan = 0,.godina = 0,.mjesec = 0,.headAr.cijena = 0,.headAr.imeArtikla = "",.headAr.kolicina = 0,.headAr.nextA = NULL };
	PozicijaR head = &zeroth;

	CitanjeRacunaIzDatoteke(head, filename);
	//
	IspisiRacune(head->nextR);
	//




	return EXIT_SUCCESS;
}


int CitanjeRacunaIzDatoteke(PozicijaR head, char* filename)
{
	PozicijaR temp = NULL;
	FILE* fp = NULL;
	char BUFFER[MAX_SIZE] = "";
	char imeracuna[MAX_SIZE] = "";

	fp = fopen(filename, "r");
	if (!fp)
	{
		printf("Datoteka racuni.txt nije otvorena!\n");
		return NULL;
	}

	while (!feof(fp))
	{
		fgets(BUFFER, MAX_LINE, fp);
		if (sscanf(BUFFER, " %s", imeracuna) == 1)
		{
			temp = CreateNewRacun(imeracuna);
			CitanjeArtikalaIzDatoteke(temp);
			//
			UnosUListuSortirano(head, temp);
			//
		}

	}

	fclose(fp);






	return EXIT_SUCCESS;
}


PozicijaR CreateNewRacun(char* imeracuna)
{
	PozicijaR newRacun = NULL;

	newRacun = (PozicijaR)malloc(sizeof(Racun));

	if (!newRacun)
	{
		printf("Greska alokacije!\n");
		return NULL;
	}

	strcpy(newRacun->imeRacuna, imeracuna);
	newRacun->godina = 0;
	newRacun->mjesec = 0;
	newRacun->dan = 0;
	newRacun->nextR = NULL;
	strcpy(newRacun->headAr.imeArtikla, "");
	newRacun->headAr.cijena = 0;
	newRacun->headAr.kolicina = 0;
	newRacun->headAr.nextA = NULL;



	return newRacun;
}


int CitanjeArtikalaIzDatoteke(PozicijaR temp)
{
	PozicijaA tempA = NULL;
	char tempImeArtikla[MAX_SIZE] = "";
	int temp_kolicina = 0;
	int temp_cijena = 0;
	char BUFFER[MAX_SIZE] = "";
	FILE* fp = NULL;

	fp = fopen(temp->imeRacuna, "r");

	if (!fp)
	{
		printf("Datoteka %s nije otvorena\n", temp->imeRacuna);
	}

	fgets(BUFFER, MAX_LINE, fp);
	sscanf(BUFFER, " %d %d %d", &temp->godina, &temp->mjesec, &temp->dan);
	while (!feof(fp))
	{
		fgets(BUFFER, MAX_LINE, fp);
		if (sscanf(BUFFER, " %s %d %d", tempImeArtikla, &temp_kolicina, &temp_cijena) == 3)
		{
			tempA = CreateArticle(tempImeArtikla, temp_kolicina, temp_cijena);
			UnosUListuArtikala(&temp->headAr, tempA);//adresa &
		}


	}
	fclose(fp);


	return EXIT_SUCCESS;
}

PozicijaA CreateArticle(char* tempImeArtikla, int  temp_kolicina, int temp_cijena)
{
	PozicijaA newArticle = NULL;

	newArticle = (PozicijaA)malloc(sizeof(Artikl));

	if (!newArticle)
	{
		printf("Greska alokacije aritkla\n");
	}

	strcpy(newArticle->imeArtikla, tempImeArtikla);
	newArticle->kolicina = temp_kolicina;
	newArticle->cijena = temp_cijena;
	newArticle->nextA = NULL;



	return newArticle;
}

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int UnosUListuArtikala(PozicijaA head, PozicijaA newArticle)
{
	PozicijaA temp = head->nextA;

	if (!temp)
	{
		InsertAfter(head, newArticle);
	}

	else if (temp->nextA)
	{
		if (strcmp(newArticle->imeArtikla, temp->imeArtikla) >= 0)
		{
			while (temp->nextA)
			{
				if (strcmp(newArticle->imeArtikla, temp->imeArtikla) >= 0 && strcmp(newArticle->imeArtikla, temp->nextA->imeArtikla) < 0)
				{
					InsertAfter(temp, newArticle);
					return EXIT_SUCCESS;
				}
				temp = temp->nextA;
			}
			if (!temp->nextA)
			{
				InsertAfter(temp, newArticle);
			}
		}
		else
		{
			InsertAfter(head, newArticle);
		}

	}


	else
	{
		if (strcmp(newArticle->imeArtikla, temp->imeArtikla) >= 0)
		{
			InsertAfter(temp, newArticle);
		}
		else
		{
			InsertAfter(head, newArticle);
		}
	}


	return EXIT_SUCCESS;
}



int InsertAfter(PozicijaA head, PozicijaA newArticle)
{
	newArticle->nextA = head->nextA;
	head->nextA = newArticle;

	return EXIT_SUCCESS;
}

int UnosUListuSortirano(PozicijaR head, PozicijaR temp)
{
	//Koristimo ovaj int kako bi rekli element je upisan, moze se zavrsiti funkcija.
	//int Provjera = 0;
	PozicijaR p = head->nextR;

	if (head == NULL)//&& Provjera == 0)
	{
		temp->nextR = head->nextR;
		head->nextR = temp;
	}
	while (p != NULL) //&& Provjera == 0)
	{
		if (temp->godina < p->godina)
		{
			temp->nextR = head->nextR;
			head->nextR = temp;
			//Provjera++;
		}
		else if (temp->godina == p->godina)
		{
			if (temp->mjesec < p->mjesec)
			{
				temp->nextR = head->nextR;
				head->nextR = temp;
				//Provjera++;
			}
			else if (temp->mjesec == p->mjesec)
			{
				if (temp->dan < p->dan)
				{
					temp->nextR = head->nextR;
					head->nextR = temp;
					//Provjera++;
				}
				else if (temp->dan == p->dan)
				{
					temp->nextR = head->nextR;
					head->nextR = temp;
					//Provjera++;
				}
			}
		}
		p = p->nextR;
		head = head->nextR;
	}
	if (head->nextR == NULL)//&& Provjera == 0)
	{
		head->nextR = temp;
	}
	
	return EXIT_SUCCESS;
}


int IspisiRacune(PozicijaR first)
{
	while (first != NULL)
	{
		printf("--------------------------------------\n");
		printf("%s %d-%d-%d\n", first->imeRacuna, first->godina, first->mjesec, first->dan);
		IspisiArtikle(first->headAr.nextA);
		first = first->nextR;
	}
	
	return EXIT_SUCCESS;
}

int IspisiArtikle(PozicijaA first)
{
	while (first != NULL)
	{
		printf("Ime:%s \t Kolicina:%d \t Cijena:%d\n", first->imeArtikla, first->kolicina, first->cijena);
		first = first->nextA;
	}
	printf("--------------------------------------\n");
	
	return EXIT_SUCCESS;
}
