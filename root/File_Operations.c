/* Below program for Student management system */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Students
{
    int id;
    char name[50];
    int age;
    float cgpa;
} stud;

void displayMenu();
void SaveRecord(stud *students, int count);
int LoadRecord(stud *students);
void DisplayRecords();
void SearchRecord(int search_id);
void DeleteRecord(int delete_id);


int main()
{
    stud cap1[100];
    int choice, search_id, delete_id;
    int stud_count = LoadRecord(cap1);

    do 
    {
        displayMenu();
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                printf("Enter ID: ");
                scanf("%d", &cap1[stud_count].id);
                printf("Enter Name: ");
                scanf("%s", &cap1[stud_count].name);
                printf("Enter Age: ");
                scanf("%d", &cap1[stud_count].age);
                printf("Enter cgpa: ");
                scanf("%f", &cap1[stud_count].cgpa);
                stud_count++;
                SaveRecord(cap1, stud_count);
                break;

            case 2:
                printf("Viewing Records...\n");
                if (stud_count==0)
                {
                    printf("No records found!\n");
                } else DisplayRecords();                
                break;

            case 3:
                printf("Enter ID : ");
                scanf("%d", &search_id);
                SearchRecord(search_id);
                break;
            
            case 4:
                printf("Enter ID : ");
                scanf("%d", &delete_id);
                DeleteRecord(delete_id);
                break;
            
            case 5:
                printf("Exiting...\n");
                break;
            
            default:
                printf("INVALID INPUT\n");
                break;
        }

    } while (choice!=5);
    
    return 0;
}

// FUNCTIONS
void displayMenu() 
{
    printf("-----------MAIN MENU-----------\n");
    printf("1. Add Record\n");
    printf("2. View Records\n");
    printf("3. Search Record\n");
    printf("4. Delete Record\n");
    printf("5. Exit\n");
}

void SaveRecord(stud *students, int count)
{
    FILE *file = fopen("students.dat", "wb");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }
    fwrite(students, sizeof(stud), count, file);
    fclose(file);
    printf("Record saved successfully!\n");
}

int LoadRecord(stud *students) 
{
    FILE *file = fopen("students.dat", "rb");
    if (file == NULL) return 0;
    int count = fread(students, sizeof(stud), 100, file);
    fclose(file);
    return count;
}

void DisplayRecords() 
{
    FILE *file = fopen("students.dat", "rb");
    if (file == NULL) {
        printf("No records found.\n");
        return;
    }

    stud temp;
    printf("--- Student Records ---\n");
    
    while (fread(&temp, sizeof(stud), 1, file) == 1) {
        printf("ID: %d\n", temp.id);
        printf("Name: %s\n", temp.name);
        printf("Age: %d\n", temp.age);
        printf("CGPA: %.2f\n", temp.cgpa);
        printf("-----------------------------\n");
    }

    fclose(file);
}

void SearchRecord(int search_id)
{
    FILE *file = fopen("students.dat", "rb");
    if (file == NULL) {
        printf("Records not found.\n");
        printf("-----------------------------\n");
        return;
    }

    stud temp;
    int found = 0;

    while ((fread(&temp, sizeof(stud), 1, file) == 1))
    {
        if (temp.id == search_id)
        {
            printf("\nRecord Found:\n");
            printf("ID: %d\n", temp.id);
            printf("Name: %s\n", temp.name);
            printf("Age: %d\n", temp.age);
            printf("CGPA: %.2f\n", temp.cgpa);
            printf("-----------------------------\n");
            found = 1;
            break;
        }
    }
    if (!found)
    {
        printf("No record found with ID '%d'.\n", search_id);
        printf("-----------------------------\n");
    }
    fclose(file);
}

void DeleteRecord(int delete_id)
{
    FILE *file = fopen("students.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");
    if (file == NULL || temp == NULL) {
        printf("Error opening files!\n");
        printf("-----------------------------\n");
        return;
    }

    stud Students;
    int found = 0;

    while (fread(&Students, sizeof(Students), 1, file))
    {
        if (Students.id != delete_id)
        {
            fwrite(&Students, sizeof(Students), 1, temp);
        }
        else found = 1;
    }

    fclose(file);
    fclose(temp);

    remove("students.dat");
    rename("temp.dat", "students.dat");

    if (found)
    {
        printf("Record deleted successfully!\n");
        printf("-----------------------------\n");
    }
    else 
    {
        printf("Record not found.\n");
        printf("-----------------------------\n");
    }
}