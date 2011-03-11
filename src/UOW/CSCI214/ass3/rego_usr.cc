#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#ifdef __ASSERT_FUNCTION
	#define ERROR   perror(__ASSERT_FUNCTION)
#else
	#define ERROR   perror("")
#endif

#define FERROR  ERROR; exit(1)

#define CHECK(x) if(x) { FERROR; }

const char	cTxtFileName[]="rego.txt";
const int	cMaxRecs = 500;

const int	RegNum_len = 10;
const int	EngNum_len = 20;
const int	ChasNum_len = 20;
const int	Make_len = 16;
const int	Model_len = 16;
const int	Shape_len = 10;
const int	Colour_len = 10;
const int	Name_len = 30;
const int	Addr_len = 30;
const int	City_len = 20;

typedef struct _RegoRec{
	long	RecNum;
	char	RegNum[RegNum_len];
	char	EngNum[EngNum_len];
	char	ChasNum[ChasNum_len];
	char	Make[Make_len];
	char	Model[Model_len];
	char	Shape[Shape_len];
	int	Year;
	char	Colour[Colour_len];
	char	Name[Name_len];
	char	Addr[Addr_len];
	char	City[City_len];
} RegoRec;

// Global Data
RegoRec	*gRecs[cMaxRecs] = { NULL };   // Array of rego records
int	gNumRecs;			   // Number of recs in array

// Private Function Prototypes
void DisplayRec(int i);

//======================= DB Function Definitions ============================

int get_line(char *buff, const int buff_len, FILE *file) {
	int	rtcode = 1;
	int	i;

	if (fgets(buff, buff_len, file)) {
		for (i = 0; i < buff_len - 1 && buff[i] != '\0'; i++) {
			if (buff[i] == '\n') {
				buff[i] = '\0';
				break;
			}
		}

		// In case there is more input on the line, put it in the bitbucket
		if (i == buff_len - 1) {
			while (getchar() != '\n');
		}
	} else {
		CHECK(!feof(file));

		rtcode = 0;
	}

	return rtcode;
}

void ReadFile(void){
	const int	buff_len = 4096;

	FILE	*file = NULL;

	char	buff[buff_len] = { '\0' };

	int	i;

	RegoRec	*tmp = NULL;

	CHECK(!(file = fopen(cTxtFileName, "r")));

	for(i = 0; i < cMaxRecs; i++) {
		CHECK(!(tmp = (RegoRec *) calloc(1, sizeof(RegoRec))));

		tmp->RecNum = i;

		if (!get_line(tmp->RegNum, RegNum_len - 1, file)) {
			free(tmp);
			break;
		}

		if (!get_line(tmp->EngNum, EngNum_len - 1, file)) {
			free(tmp);
			break;
		}

		if (!get_line(tmp->ChasNum, ChasNum_len - 1, file)) {
			free(tmp);
			break;
		}

		if (!get_line(tmp->Make, Make_len - 1, file)) {
			free(tmp);
			break;
		}

		if (!get_line(tmp->Model, Model_len - 1, file)) {
			free(tmp);
			break;
		}

		if (!get_line(tmp->Shape, Shape_len - 1, file)) {
			free(tmp);
			break;
		}

		if (fgets(buff, buff_len, file)) {
			tmp->Year = atoi(buff);
		} else {
			free(tmp);
			break;
		}

		if (!get_line(tmp->Colour, Colour_len - 1, file)) {
			free(tmp);
			break;
		}

		if (!get_line(tmp->Name, Name_len - 1, file)) {
			free(tmp);
			break;
		}

		if (!get_line(tmp->Addr, Addr_len - 1, file)) {
			free(tmp);
			break;
		}

		if (!get_line(tmp->City, City_len - 1, file)) {
			free(tmp);
			break;
		}

		// Snarf the space between records
		if (!fgets(buff, buff_len, file)) {
			free(tmp);
			break;
		}

		gRecs[i] = tmp;
	}

	gNumRecs = i;

	printf("%d records read\n", i);

	fclose(file);
}

void SaveFile(void) {
	// Saves recs in gRecs to cTxtFileName
	FILE	*file = NULL;

	int	i, j;

	CHECK(!(file = fopen(cTxtFileName, "w")));

	for (i = 0, j = 0; j < gNumRecs && i < cMaxRecs; i++) {
		if (gRecs[i]) {
			fprintf(file, "%s\n%s\n%s\n%s\n%s\n%s\n%d\n%s\n%s\n%s\n%s\n\n",
				gRecs[i]->RegNum,
				gRecs[i]->EngNum,
				gRecs[i]->ChasNum,
				gRecs[i]->Make,
				gRecs[i]->Model,
				gRecs[i]->Shape,
				gRecs[i]->Year,
				gRecs[i]->Colour,
				gRecs[i]->Name,
				gRecs[i]->Addr,
				gRecs[i]->City
			);

			j++;
		}
	}

	fclose(file);
}

void DisplayRecs(void) {
	char	Ans;
	int	i, j;

	for (i = 0, j = 0; j < gNumRecs && i < cMaxRecs; i++) {
		if (gRecs[i]) {
			putchar('\n');
			DisplayRec(i);
			printf("Display next record (y/n) > ");
			Ans = getchar();
			while(getchar() != '\n');
			if(Ans != 'y') {
				break;
			}
			j++;
		}
	}
}

int find_index(char *RegNum) {
	int	i, j;

	for (i = 0, j = 0; j < gNumRecs && i < cMaxRecs; i++) {
		if (gRecs[i]) {
			if (strcmp(gRecs[i]->RegNum, RegNum) == 0) {
				return(i);
			}
			j++;
		}
	}

	return(-1);
}
	
void SearchRecs(void) {
	char	RegNum[RegNum_len] = { '\n' };
	int	record = -1;

	// ask user to enter rego number
	printf("Enter Reg No. to search for: ");
	get_line(RegNum, RegNum_len, stdin);

	// seach for matching record
	record = find_index(RegNum);

	// if found display record
	if (record != -1) {
		DisplayRec(record);
	} else {
		// else display not found message
		printf("No entry with Reg No: %s\n", RegNum);
	}
}

void ModifyRec(void) {
	const int	field_len = 12;

	char	RegNum[RegNum_len] = { '\n' };
	char	field[field_len] = { '\n' };

	int	record = -1;

	// ask user to enter rego number
	printf("Enter Reg No. to search for: ");
	get_line(RegNum, RegNum_len, stdin);

	// seach for matching record
	record = find_index(RegNum);

	if (record != -1) {
		while (record != -1) {
			//    display record
			DisplayRec(record);
			//    get and ammend record details
			printf("Which field do you wish to modify (Type \"DONE\" to finish)? ");
			get_line(field, field_len, stdin);

			if (strcmp(field, "Reg No") == 0) {
				printf("Enter the new %s: ", field);
				get_line(gRecs[record]->RegNum, RegNum_len - 1, stdin);
			} else if (strcmp(field, "Engine No") == 0) {
				printf("Enter the new %s: ", field);
				get_line(gRecs[record]->EngNum, EngNum_len - 1, stdin);
			} else if (strcmp(field, "Chassis No") == 0) {
				printf("Enter the new %s: ", field);
				get_line(gRecs[record]->ChasNum, ChasNum_len - 1, stdin);
			} else if (strcmp(field, "Make") == 0) {
				printf("Enter the new %s: ", field);
				get_line(gRecs[record]->Make, Make_len - 1, stdin);
			} else if (strcmp(field, "Model") == 0) {
				printf("Enter the new %s: ", field);
				get_line(gRecs[record]->Model, Model_len - 1, stdin);
			} else if (strcmp(field, "Shape") == 0) {
				printf("Enter the new %s: ", field);
				get_line(gRecs[record]->Shape, Shape_len - 1, stdin);
			} else if (strcmp(field, "Year") == 0) {
				printf("Enter the new %s: ", field);
				scanf("%d", &gRecs[record]->Year);

				while(getchar() != '\n');
			} else if (strcmp(field, "Colour") == 0) {
				printf("Enter the new %s: ", field);
				get_line(gRecs[record]->Colour, Colour_len - 1, stdin);
			} else if (strcmp(field, "Name") == 0) {
				printf("Enter the new %s: ", field);
				get_line(gRecs[record]->Name, Name_len - 1, stdin);
			} else if (strcmp(field, "Address") == 0) {
				printf("Enter the new %s: ", field);
				get_line(gRecs[record]->Addr, Addr_len - 1, stdin);
			} else if (strcmp(field, "City") == 0) {
				printf("Enter the new %s: ", field);
				get_line(gRecs[record]->City, City_len - 1, stdin);
			} else if (strcmp(field, "DONE") == 0) {
				record = -1;
			} else {
				printf("Can not modify field \"%s\".\n", field);
			}
		}
	} else {
		// else display not found message
		printf("No entry with Reg No: %s\n", RegNum);
	}
}

void AddRec(void) {
	int	i;
	int	record;

	if (gNumRecs < cMaxRecs) {
		for (i = 0; i < cMaxRecs; i++) {
			if (gRecs[i] == NULL) {
				printf("New record = %d\n", i);
				record = i;
				break;
			}
		}

		CHECK(!(gRecs[record] = (RegoRec *) calloc(1, sizeof(RegoRec))));

		gNumRecs++;

		gRecs[record]->RecNum = record;

		// Gets Record Details
		// and ads new record to DB
		printf("Enter the new Reg No: ");
		get_line(gRecs[record]->RegNum, RegNum_len - 1, stdin);

		printf("Enter the new Engine No: ");
		get_line(gRecs[record]->EngNum, EngNum_len - 1, stdin);

		printf("Enter the new Chassis No: ");
		get_line(gRecs[record]->ChasNum, ChasNum_len - 1, stdin);

		printf("Enter the new Make: ");
		get_line(gRecs[record]->Make, Make_len - 1, stdin);

		printf("Enter the new Model: ");
		get_line(gRecs[record]->Model, Model_len - 1, stdin);

		printf("Enter the new Shape: ");
		get_line(gRecs[record]->Shape, Shape_len - 1, stdin);

		printf("Enter the new Year: ");
		scanf("%d", &gRecs[record]->Year);
		while(getchar() != '\n');

		printf("Enter the new Colour: ");
		get_line(gRecs[record]->Colour, Colour_len - 1, stdin);

		printf("Enter the new Name: ");
		get_line(gRecs[record]->Name, Name_len - 1, stdin);

		printf("Enter the new Address: ");
		get_line(gRecs[record]->Addr, Addr_len - 1, stdin);

		printf("Enter the new City: ");
		get_line(gRecs[record]->City, City_len - 1, stdin);

		printf("The following record was added at index: %d.\n", record);
		DisplayRec(record);
	} else {
		printf("No space left in the database.\n");
	}
}

void DeleteRec(void) {
	char	RegNum[RegNum_len] = { '\n' };
	int	record = -1;

	// Gets Reg Num
	printf("Enter Reg No. to search for: ");
	get_line(RegNum, RegNum_len, stdin);

	// finds record
	record = find_index(RegNum);

	// if found deletes record
	if (record != -1) {
		free(gRecs[record]);
		gRecs[record] = NULL;
		gNumRecs--;
		printf("Record %d with Reg No: %s, has been deleted.\n", record, RegNum);
	} else {
		// else displays not found message
		printf("No entry with Reg No: %s\n", RegNum);
	}
}


//==================== Private Function Definitions ==========================

void DisplayRec(int i) {
	printf("%-12s%d\n", "Record No:", gRecs[i]->RecNum);
	printf("%-12s%s\n", "Reg No:", gRecs[i]->RegNum);
	printf("%-12s%s\n", "Engine No:", gRecs[i]->EngNum);
	printf("%-12s%s\n", "Chassis No:", gRecs[i]->ChasNum);
	printf("%-12s%s\n", "Make:", gRecs[i]->Make);
	printf("%-12s%s\n", "Model:", gRecs[i]->Model);
	printf("%-12s%s\n", "Shape:", gRecs[i]->Shape);
	printf("%-12s%d\n", "Year:", gRecs[i]->Year);
	printf("%-12s%s\n", "Colour:", gRecs[i]->Colour);
	printf("%-12s%s\n", "Name:", gRecs[i]->Name);
	printf("%-12s%s\n", "Address:", gRecs[i]->Addr);
	printf("%-12s%s\n", "City:", gRecs[i]->City);
}

//==================== menu() & main() Functions =============================

char Menu(void) {
	char	Cmd;
	putchar('\n');
	printf("(l)ist records\n");
	printf("(s)earch records\n");
	printf("(m)odify record\n");
	printf("(a)dd record\n");
	printf("(d)elete record\n");
	printf("(q)uit\n");
	printf("Command > ");

	fflush(stdout);
	
	Cmd = getchar();
	while(getchar() != '\n');

	return Cmd;
}

int main(int argc, char **argv){
	int	Quit = 0;

	ReadFile();

	while(!Quit) {
		char	cmd = Menu();
		switch(cmd) {
			case	'l':	DisplayRecs();
					break;

			case	's':	SearchRecs();
					break;

			case	'm':	ModifyRec();
					break;

			case	'a':	AddRec();
					break;

			case	'd':	DeleteRec();
					break;

			case	'q':	Quit = 1;
					break;

			default:	printf("I do not understand the command: %c\n", cmd);
		}
	}

	printf("Thanks for using Rego Database!\n");

	SaveFile();

	// Free the memory used by the records
	for (int i = 0; i < gNumRecs;) {
		if (gRecs[i]) {
			free(gRecs[i]);
			i++;
		}
	}

	return 0;
}

//======================= End of Program Code ================================

