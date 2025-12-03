/* library_management.c
   Library Book Management System (Option B)
   Fields: Book ID, Title, Author, Year, Status, IssuedTo, IssuedOn
   Features: add, view all, search (id/title), issue, return, view issued, sort by id/title, save/load file
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 200
#define FILENAME "books.txt"

struct Date { int dd, mm, yyyy; };

struct Book {
    int id;
    char title[150];
    char author[100];
    int year;
    char status[12];      // "Available" or "Issued"
    char issuedTo[100];   // borrower's name
    struct Date issuedOn; // date of issue
};

struct Book library[MAX];
int count = 0;

/* Function prototypes */
void loadFromFile();
void saveToFile();
void addBook();
void viewAll();
void searchBook();
void issueBook();
void returnBook();
void viewIssued();
void sortByID();
void sortByTitle();
void loadSampleData();
void trimNewline(char *s);
void pauseAndClear();

int main() {
    int choice;
    char ch;

    loadFromFile();

    printf("Load sample data? (Y/N): ");
    if (scanf(" %c", &ch) == 1) {
        if (ch == 'Y' || ch == 'y') loadSampleData();
    }

    while (1) {
        printf("\n--- Library Book Management System ---\n");
        printf("1. Add Book\n");
        printf("2. View All Books\n");
        printf("3. Search Book (ID/Title)\n");
        printf("4. Issue Book\n");
        printf("5. Return Book\n");
        printf("6. View Issued Books\n");
        printf("7. Sort by Book ID (asc)\n");
        printf("8. Sort by Title (asc)\n");
        printf("9. Save & Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Try again.\n");
            int c; while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        switch (choice) {
            case 1: addBook(); break;
            case 2: viewAll(); break;
            case 3: searchBook(); break;
            case 4: issueBook(); break;
            case 5: returnBook(); break;
            case 6: viewIssued(); break;
            case 7: sortByID(); break;
            case 8: sortByTitle(); break;
            case 9: saveToFile(); printf("Data saved. Exiting...\n"); return 0;
            default: printf("Invalid choice.\n");
        }
    }
    return 0;
}

/* Helper to trim newline from fgets */
void trimNewline(char *s) {
    size_t len = strlen(s);
    if (len > 0 && s[len-1] == '\n') s[len-1] = '\0';
}

/* Load books from file.
   Format per line:
   id;title;author;year;status;issuedTo;dd-mm-yyyy
*/
void loadFromFile() {
    FILE *f = fopen(FILENAME, "r");
    if (!f) return;
    char line[600];
    while (fgets(line, sizeof(line), f)) {
        int id, year;
        char title[150], author[100], status[12], issuedTo[100];
        int dd=0, mm=0, yy=0;
        /* Initialize defaults */
        title[0]=author[0]=status[0]=issuedTo[0]='\0';

        /* line may contain newline at end */
        trimNewline(line);

        /* parse using ; as separator, date as dd-mm-yyyy or 0-0-0 */
        if (sscanf(line, "%d;%149[^;];%99[^;];%d;%11[^;];%99[^;];%d-%d-%d",
                   &id, title, author, &year, status, issuedTo, &dd, &mm, &yy) >= 6) {
            library[count].id = id;
            strncpy(library[count].title, title, sizeof(library[count].title)-1);
            library[count].title[sizeof(library[count].title)-1] = '\0';
            strncpy(library[count].author, author, sizeof(library[count].author)-1);
            library[count].author[sizeof(library[count].author)-1] = '\0';
            library[count].year = year;
            strncpy(library[count].status, status, sizeof(library[count].status)-1);
            library[count].status[sizeof(library[count].status)-1] = '\0';
            strncpy(library[count].issuedTo, issuedTo, sizeof(library[count].issuedTo)-1);
            library[count].issuedTo[sizeof(library[count].issuedTo)-1] = '\0';
            library[count].issuedOn.dd = dd;
            library[count].issuedOn.mm = mm;
            library[count].issuedOn.yyyy = yy;
            count++;
            if (count >= MAX) break;
        }
    }
    fclose(f);
}

/* Save all books to file */
void saveToFile() {
    FILE *f = fopen(FILENAME, "w");
    if (!f) {
        printf("Error: could not open file for writing.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        /* if no issue date, write 0-0-0 */
        fprintf(f, "%d;%s;%s;%d;%s;%s;%d-%d-%d\n",
                library[i].id,
                library[i].title,
                library[i].author,
                library[i].year,
                library[i].status,
                library[i].issuedTo,
                library[i].issuedOn.dd,
                library[i].issuedOn.mm,
                library[i].issuedOn.yyyy);
    }
    fclose(f);
}

/* Add a new book */
void addBook() {
    if (count >= MAX) { printf("Library capacity reached.\n"); return; }

    int id, year;
    char title[150], author[100];

    printf("Enter Book ID (numeric): "); 
    if (scanf("%d", &id) != 1) { printf("Invalid ID.\n"); int c; while ((c = getchar()) != '\n' && c != EOF); return; }
    getchar(); /* consume newline */

    /* Check duplicate ID */
    for (int i = 0; i < count; i++) {
        if (library[i].id == id) {
            printf("Error: Book ID already exists.\n");
            return;
        }
    }

    printf("Enter Title: "); fgets(title, sizeof(title), stdin); trimNewline(title);
    printf("Enter Author: "); fgets(author, sizeof(author), stdin); trimNewline(author);
    printf("Enter Year of Publication: "); if (scanf("%d", &year) != 1) { printf("Invalid year.\n"); int c; while ((c = getchar()) != '\n' && c != EOF); return; }
    getchar();

    library[count].id = id;
    strncpy(library[count].title, title, sizeof(library[count].title)-1);
    library[count].title[sizeof(library[count].title)-1] = '\0';
    strncpy(library[count].author, author, sizeof(library[count].author)-1);
    library[count].author[sizeof(library[count].author)-1] = '\0';
    library[count].year = year;
    strcpy(library[count].status, "Available");
    library[count].issuedTo[0] = '\0';
    library[count].issuedOn.dd = library[count].issuedOn.mm = library[count].issuedOn.yyyy = 0;

    count++;
    printf("Book added successfully.\n");
}

/* View all books */
void viewAll() {
    if (count == 0) { printf("No books in library.\n"); return; }
    printf("\nID\tTitle\t\tAuthor\t\tYear\tStatus\n");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%d\t%s\t\t%s\t\t%d\t%s\n",
               library[i].id,
               library[i].title,
               library[i].author,
               library[i].year,
               library[i].status);
    }
}

/* Search by ID or Title substring */
void searchBook() {
    if (count == 0) { printf("No books to search.\n"); return; }
    int mode;
    printf("Search by: 1) Book ID  2) Title (substring)\nChoice: ");
    if (scanf("%d", &mode) != 1) { printf("Invalid input.\n"); int c; while ((c = getchar()) != '\n' && c != EOF); return; }
    getchar();

    if (mode == 1) {
        int id; printf("Enter Book ID: ");
        if (scanf("%d", &id) != 1) { printf("Invalid ID.\n"); int c; while ((c = getchar()) != '\n' && c != EOF); return; }
        for (int i = 0; i < count; i++) {
            if (library[i].id == id) {
                printf("\nFound:\nID: %d\nTitle: %s\nAuthor: %s\nYear: %d\nStatus: %s\n", 
                       library[i].id, library[i].title, library[i].author, library[i].year, library[i].status);
                if (strcmp(library[i].status, "Issued") == 0)
                    printf("Issued To: %s on %02d-%02d-%04d\n", library[i].issuedTo, library[i].issuedOn.dd, library[i].issuedOn.mm, library[i].issuedOn.yyyy);
                return;
            }
        }
        printf("Book ID %d not found.\n", id);
    } else if (mode == 2) {
        char query[150];
        printf("Enter title substring: ");
        if (!fgets(query, sizeof(query), stdin)) return;
        trimNewline(query);
        int found = 0;
        for (int i = 0; i < count; i++) {
            if (strstr(library[i].title, query) != NULL) {
                found = 1;
                printf("\nID: %d\nTitle: %s\nAuthor: %s\nYear: %d\nStatus: %s\n",
                       library[i].id, library[i].title, library[i].author, library[i].year, library[i].status);
                if (strcmp(library[i].status, "Issued") == 0)
                    printf("Issued To: %s on %02d-%02d-%04d\n", library[i].issuedTo, library[i].issuedOn.dd, library[i].issuedOn.mm, library[i].issuedOn.yyyy);
            }
        }
        if (!found) printf("No titles matched '%s'.\n", query);
    } else {
        printf("Invalid choice.\n");
    }
}

/* Issue a book to a borrower */
void issueBook() {
    if (count == 0) { printf("No books in library.\n"); return; }
    int id;
    printf("Enter Book ID to issue: ");
    if (scanf("%d", &id) != 1) { printf("Invalid ID.\n"); int c; while ((c = getchar()) != '\n' && c != EOF); return; }
    for (int i = 0; i < count; i++) {
        if (library[i].id == id) {
            if (strcmp(library[i].status, "Available") != 0) {
                printf("Book is already issued to %s on %02d-%02d-%04d\n", library[i].issuedTo, library[i].issuedOn.dd, library[i].issuedOn.mm, library[i].issuedOn.yyyy);
                return;
            }
            getchar(); /* consume newline */
            char borrower[100];
            printf("Enter borrower's name: "); fgets(borrower, sizeof(borrower), stdin); trimNewline(borrower);
            int dd, mm, yy;
            printf("Enter issue date (DD MM YYYY): ");
            if (scanf("%d %d %d", &dd, &mm, &yy) != 3) { printf("Invalid date.\n"); int c; while ((c = getchar()) != '\n' && c != EOF); return; }
            /* update */
            strcpy(library[i].status, "Issued");
            strncpy(library[i].issuedTo, borrower, sizeof(library[i].issuedTo)-1);
            library[i].issuedTo[sizeof(library[i].issuedTo)-1] = '\0';
            library[i].issuedOn.dd = dd; library[i].issuedOn.mm = mm; library[i].issuedOn.yyyy = yy;
            printf("Book ID %d issued to %s on %02d-%02d-%04d\n", id, library[i].issuedTo, dd, mm, yy);
            return;
        }
    }
    printf("Book ID %d not found.\n", id);
}

/* Return an issued book */
void returnBook() {
    if (count == 0) { printf("No books in library.\n"); return; }
    int id;
    printf("Enter Book ID to return: ");
    if (scanf("%d", &id) != 1) { printf("Invalid ID.\n"); int c; while ((c = getchar()) != '\n' && c != EOF); return; }
    for (int i = 0; i < count; i++) {
        if (library[i].id == id) {
            if (strcmp(library[i].status, "Issued") != 0) {
                printf("Book is not currently issued.\n");
                return;
            }
            strcpy(library[i].status, "Available");
            library[i].issuedTo[0] = '\0';
            library[i].issuedOn.dd = library[i].issuedOn.mm = library[i].issuedOn.yyyy = 0;
            printf("Book ID %d returned and now available.\n", id);
            return;
        }
    }
    printf("Book ID %d not found.\n", id);
}

/* View only issued books */
void viewIssued() {
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(library[i].status, "Issued") == 0) {
            found = 1;
            printf("\nID: %d\nTitle: %s\nAuthor: %s\nIssued To: %s on %02d-%02d-%04d\n",
                   library[i].id, library[i].title, library[i].author,
                   library[i].issuedTo, library[i].issuedOn.dd, library[i].issuedOn.mm, library[i].issuedOn.yyyy);
        }
    }
    if (!found) printf("No books currently issued.\n");
}

/* Sort by Book ID (ascending) */
void sortByID() {
    if (count < 2) { printf("Not enough books to sort.\n"); return; }
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (library[j].id > library[j+1].id) {
                struct Book tmp = library[j];
                library[j] = library[j+1];
                library[j+1] = tmp;
            }
        }
    }
    printf("Sorted by Book ID (ascending).\n");
}

/* Sort by Title (ascending lexicographic) */
void sortByTitle() {
    if (count < 2) { printf("Not enough books to sort.\n"); return; }
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(library[j].title, library[j+1].title) > 0) {
                struct Book tmp = library[j];
                library[j] = library[j+1];
                library[j+1] = tmp;
            }
        }
    }
    printf("Sorted by Title (ascending).\n");
}

/* Load a few sample records for quick testing */
void loadSampleData() {
    count = 0;
    library[count++] = (struct Book){301, "The Alchemist", "Paulo Coelho", 1988, "Available", "", {0,0,0}};
    library[count++] = (struct Book){302, "C Programming", "K&R", 1978, "Issued", "Amit Kumar", {05,9,2025}};
    library[count++] = (struct Book){303, "Data Structures", "Seymour", 2015, "Available", "", {0,0,0}};
    saveToFile();
    printf("Sample data loaded.\n");
}
