//Ryan Nishikawa
//ryannishikawa48@csu.fullerton.edu
//This course: “CPSC 223C”
//This product: “Final semester test”
//Today’s date: “May 14, 2024”

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "student.h"

void print_data(struct student_info person); //function to print a student's data
void print_records(FILE *bin); //function to print all student records
void print_menu(); //function to print the menu and get selection
void pos_search(int pos, FILE *bin); //function to search the bin file
void CWID_search(long CWID, FILE *bin, struct student_info *found); //function to search based on CWID
void new_student(FILE *bin); //function to add a new student to the records
void remove_student(FILE *bin); //function to logically remove a student from data
void copy(FILE *bin, char *filename);
void help_copy(FILE *bin, FILE *bak);


int main(int argc, char* argv[]) {
    printf("\n===== Begin execution ==========\n\n");

//print intro
    printf("Welcome to student roster file. \n");
    printf("We keep your records current. \n");
    printf("This program is maintained by Ryan Nishikawa \n");

    //get and open bin file
    //get file + check if it opens successfully
    char filename[256];
    FILE *bin = NULL;
    while(bin == NULL) {
        printf("\nPlease enter the name of your file(<ctrl + c> to quit): ");
        scanf("%255s", filename);
        bin = fopen(filename, "rb+");
        if (bin == NULL) {
            printf("Sorry. File was not found. Try again.\n");
        }
        while ((getchar()) != '\n');
    }


//switch statement for menu controls
    char c;
    //an empty struct
    struct student_info empty_student = {
        .name = {""}, 
        .CWID = 0, 
        .major = {""}, 
        .class_level = 5, 
        .zip = {""}, 
        .phoneNum = {""}, 
        .gpa = 0.0
    };
    print_menu();
    printf("\nPlease enter a selection: ");
    while ('X' != (c=getchar())) {
        switch(c) {
//Output all student records (status: done)
            case 'O':
                rewind(bin);
                print_records(bin);
                break;
//Find and show a student by cwid number (status: done)
            case 'F':
                long CWID = 0;
                struct student_info student = empty_student; //initialize an empty student
                printf("Enter the CWID of the student you with to view (All CWID are 8 numbers long): ");
                rewind(bin); //move file pointer to the BOF
                scanf("%ld", &CWID);
                CWID_search(CWID, bin, &student);
                if (student.CWID > 0) { //if found print
                    printf("\nThe data of student [%ld] is this:\n", student.CWID);
                    print_data(student);
                }
                break;
//Position: show the struc data at a given position (status: done)
            case 'P':
                int pos = 0;
                printf("What position do you wish to view? ");
                scanf("%d", &pos);
                rewind(bin);  //move file pointer to the BOF
                pos_search(pos, bin);
                break;
//Menu: show this menu (status: done)
            case 'M':
                print_menu();
                break;
//New student will be added to the student roster file (status: done)
            case 'N':
                new_student(bin);
                break;
            case 'C':
                copy(bin, filename);
                break;
//Remove a student's data (status: WIP)
            case 'R':
                rewind(bin); //move file pointer to the BOF
                while ((getchar()) != '\n'); //clear input buffer after using scanf
                remove_student(bin);
                break;
//Exit from this program (status: done)
            //case 'X':
//something not on the menu is chosen
            default:
                printf("\nNot an option. Try again.\n");
                break;
        }
        printf("\nPlease enter a selection: ");
        while ((getchar()) != '\n'); //clear input buffer to prevent doubles
    }

//end
    fclose(bin);
    printf("\nThank you for using the Student Record System.");
    printf("\nYou may use this software at any time. Bye.\n");
    printf("\n===== End execution ==========\n");
    return 0;
}

void print_data(struct student_info person) {
    printf("\nname = %s\n", person.name);
    printf("CWID = %ld\n", person.CWID);
    printf("major = %s\n", person.major);
    printf("class level = %s\n", class_level(person.class_level));
    printf("zip = %s\n", person.zip);
    printf("phoneNum = %s\n", person.phoneNum);
    printf("gpa = %0.1f\n", person.gpa);
}

void print_menu() {
    //menu options
    printf("\nPlease select one of the following options by entering the associated single character. \n");
    printf("O = Output all student records \n");
    printf("F = Find and show a student by cwid number \n");
    printf("P = Position: show the struct data at a given position. \n");
    printf("M = Menu: show this menu \n");
    printf("N = New student will be added to the student roster file. \n");
    printf("C = Copy to backup file\n");
    printf("R = Remove a student's data \n");
    printf("X = Exit from this program. \n");
}

void print_records(FILE *bin) {
    struct student_info person;
    int counter = 0;  //to get the num of data records in the file
//loop to print each struct
     while (0 < fread(&person, sizeof(struct student_info), 1, bin)) {
        if(person.CWID > 0) { //check for neg CWID
            print_data(person);
            printf("\n-------------------------------------\n");
            ++counter;
        }
    }
    printf("Number of data records is %d.\n", counter);
}

void pos_search(int pos, FILE *bin) {
    struct student_info person;
    int counter = 1; //I want to start counting from 1 instead of 0
    while (0 < fread(&person, sizeof(struct student_info), 1, bin)) {
        if(person.CWID > 0) { //skip negative CWID
            //in case someone enters 0 for pos
            if (pos == 0) {
                printf("\nThere is nothing at position 0. Positions start from 1. Please try again.\n");
                return;
            } else if (counter == pos) { //print info of struc at pos
                printf("\nThe data record at position %d is this:\n", pos);
                print_data(person);
                return;
            }
            ++counter;
        }
    }
    printf("Record not found.\n");
}

void CWID_search(long CWID, FILE *bin, struct student_info *found) {
    struct student_info person;
     while (0 < fread(&person, sizeof(struct student_info), 1, bin)) {
        if(person.CWID == CWID) { //check for matching CWID
            *found = person;
            return;
        }
    }
    printf("Record not found.\n");
}

void new_student(FILE *bin) {
    struct student_info person;
    while ((getchar()) != '\n'); //clear input buffer
//get name
    printf("\nEnter new student name: ");
    fgets(person.name, sizeof(person.name), stdin);
    person.name[strlen(person.name) - 1] = '\0'; //remove newline
//get CWID
    printf("Enter new student CWID (All CWID are 8 numbers long): ");
    scanf("%ld", &person.CWID);
    while ((getchar()) != '\n'); //clear input buffer after using scanf
//get major
    printf("Enter new student major: ");
    fgets(person.major, sizeof(person.major), stdin);
    person.major[strlen(person.major) - 1] = '\0'; //remove newline
//get class level
    printf("Enter new student class level (Freshman=0,Sophomore=1,Junior=2,Senior=3): ");
    int class_lvl;
    scanf("%d", &class_lvl);
    person.class_level = class_lvl;
    while ((getchar()) != '\n'); //clear input buffer after using scanf
//get zip code
    printf("Enter new student zip code: ");
    fgets(person.zip, sizeof(person.zip), stdin);
    person.zip[strlen(person.zip) - 1] = '\0'; //remove newline
//get phone num
    printf("Enter new student phone number (XXX-XXX-XXXX): ");
    fgets(person.phoneNum, sizeof(person.phoneNum), stdin);
    person.phoneNum[strlen(person.phoneNum) - 1] = '\0'; //remove newline
//get gpa
    printf("Enter new student GPA: ");
    scanf("%g", &person.gpa);
//write to file
    int counter = 1;
    rewind(bin);
    struct student_info removed;
    while (0 < fread(&removed, sizeof(struct student_info), 1, bin)) {
       if(removed.CWID < 0) { //check for neg CWID
           fseek(bin, -sizeof(struct student_info), SEEK_CUR); //move file ptr back to person
           fwrite(&person, sizeof(struct student_info), 1, bin); //overwrite person
           printf("Thank you the new data has been added to position %d of the data set.\n", counter);
           return;
       }
       ++counter;
    }
    fseek(bin, 0, SEEK_END); //move file pointer to EOF
    fwrite(&person, sizeof(struct student_info), 1, bin);
    printf("\nThank you. The new data was appended to the end of the data set.\n");
}

void remove_student(FILE *bin) {
    long CWID;
    struct student_info person;
    printf("Enter the CWID of the student to be removed (All CWID are 8 numbers long): ");
    scanf("%ld", &CWID);
    while (0 < fread(&person, sizeof(struct student_info), 1, bin)) {
       if(person.CWID == CWID) { //check for matching CWID
           printf("\nThe data of student [%ld] has been logically removed\n", person.CWID);
           person.CWID = -1;
           fseek(bin, -sizeof(struct student_info), SEEK_CUR); //move file ptr back to person
           fwrite(&person, sizeof(struct student_info), 1, bin); //overwrite person
           return;
       }
    }
    printf("Record not found.\n");
}

void copy(FILE *bin, char *filename) {
    char y;
    rewind(bin);
    while ((getchar()) != '\n'); //clear input buffer to prevent doubles
    printf("Would you like to copy the file \"%s\" to a backup file? (y/n): ", filename);
    while('n' != (y = getchar())) {
        switch (y) {
            case 'y':
                char backup_filename[256];
                strcpy(backup_filename, filename);
                char *dot;
                dot = strrchr(backup_filename, '.');
                strcpy(dot, ".bak");
                printf("The file \"%s\" has been copied to the backup file \"%s\"\n", filename, backup_filename);
                FILE *bak = NULL;
                bak = fopen(backup_filename, "wb");
                help_copy(bin, bak);
                return;
            default:
                printf("Not an option. Please try again.\n");
                printf("Would you like to copy the file \"%s\" to a backup file? (y/n): ", filename);
                break;
        }
        while ((getchar()) != '\n'); //clear input buffer to prevent doubles
    }
    printf("The file \"%s\" will not be copied to a backup file\n", filename);
}

void help_copy(FILE *bin, FILE *bak) {
    struct student_info person;
    while (0 < fread(&person, sizeof(struct student_info), 1, bin)) {
        if(person.CWID > 0) {
            fwrite(&person, sizeof(struct student_info), 1, bak);
        }
    }
    fclose(bak);
}