#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE (128)
#define MAX_LINE (1024)
#define MAX_BODOVI (20)

struct _student;
typedef struct _student {
	char ime[MAX_SIZE];
	char prezime[MAX_SIZE];
	int bodovi;
}Student;

int BrojStudenata(char* ime_datoteke);
Student* Alokacija(int br_st, char* ime_datoteke);
Student MaxBodovi(Student* studenti, int br_st);
void Ispis(int br_st, Student* studenti);


int main()
{
	char filename[MAX_SIZE] = "studenti.txt";
	int brojac = 0;
	int br_st = 0;
	Student* stud = NULL;
	Student s = { .bodovi = 0,.ime = "",.prezime = "" };

	br_st = BrojStudenata(filename);
	stud = Alokacija(br_st, filename);
	s = MaxBodovi(stud, br_st);

	Ispis(br_st, stud);


	return EXIT_SUCCESS;
}


int BrojStudenata(char* ime_datoteke)
{
	char BUFFER[MAX_LINE] = "";
	FILE* datoteka = NULL;
	int brojac = 0;
	datoteka = fopen(ime_datoteke, "r");

	if (!datoteka)
	{
		printf("Datoteka nije otvorena!");
		return -1;
	}

	while (!feof(datoteka)) {
		fgets(BUFFER, MAX_LINE, datoteka);
		if (strcmp("\n", BUFFER) != 0)
		{
			brojac++;
		}
	}
	fclose(datoteka);


	return brojac;
}

Student* Alokacija(int br_st, char* ime_datoteke)
{
	int brojac = 0;
	Student* studenti = NULL;

	FILE* datoteka = NULL;
	datoteka = fopen(ime_datoteke, "r");

	studenti = (Student*)malloc(br_st * (sizeof(Student)));

	if (!datoteka)
	{
		printf("Datoteka nije otvorena!");
		return NULL;
	}
	if (!studenti)
	{
		printf("Greska alociranja!");
		return NULL;
	}

	while (!feof(datoteka))
	{
		fscanf(datoteka, " %s %s %d", studenti[brojac].ime, studenti[brojac].prezime, &studenti[brojac].bodovi);
		brojac++;
	}

	fclose(datoteka);


	return studenti;
}

Student MaxBodovi(Student* studenti, int br_st)
{
	Student temp = studenti[0];
	int i = 0;

	for (i = 1; i < br_st; i++)
	{
		if (temp.bodovi < studenti[i].bodovi)
			temp = studenti[i];
	}

	return temp;
}

void Ispis(int br_st, Student* studenti)
{
	int i = 0;
	double relativni = 0.0;

	while (i < br_st)
	{
		relativni = (((double)studenti[i].bodovi / MAX_BODOVI) * 100);
		printf("Ime: %s \nPrezime: %s \nApsolutni broj bodova: %d \nRelativni broj bodova: %.2lf \n\n", studenti[i].ime, studenti[i].prezime, studenti[i].bodovi, relativni);
		i++;
	}

}



