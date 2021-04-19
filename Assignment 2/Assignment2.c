// Assignment2.c : This file contains the 'main' function. Program execution begins and ends there.
//

// #include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>

// Begins the main function
int main(void) {

    int a = 0;
    int tflag = 0; // Tflag - (third flag) is set for if the user picks 3. If the file is not found, tflag is set to 1 and 
    // the user will be brought to the second menu.

    while (a < 1) {

        // Menu for user to choose from

        int inputfromuser;
        if (tflag == 0) {
            printf("1. Select File to process\n");
            printf("2. Exit the program\n");
            printf("Enter a choice 1 or 2: ");
            scanf("%i", &inputfromuser);
            printf("\n\n");
        }

        else {
            inputfromuser = 1;
        }

        // Second menu

        if (inputfromuser == 1) {
            int inputfromuser2;
            printf("Which file do you want to process? Note: I only look in the current directory.\n");
            printf("Enter 1 to pick the largest file\n");
            printf("Enter 2 to pick the smallest file\n");
            printf("Enter 3 to specify the name of a file\n\n");
            printf("Enter a choice from 1 to 3: ");
            scanf("%i", &inputfromuser2);
            printf("\n");

            // If the user inputs 1, the current directory is opened and the maximum file size is searched

            if (inputfromuser2 == 1) {

                // The first 10 lines were assisted with resources from: https://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program/17683417
                // and: https://www.geeksforgeeks.org/c-program-find-size-file/
                // These served to add more detail to the exploration

                DIR* d;
                struct dirent* dir;
                d = opendir(".");
                int s = 0;
                long int maximum;
                char filename[255];
                int flag = 0;
                if (d) {
                    while ((dir = readdir(d)) != NULL) {
                        // printf("%s\n", dir->d_name);
                        FILE* fp = fopen(dir->d_name, "r");

                        // if statement to verify that movies_ and .csv are contained within the file name

                        if (strstr(dir->d_name, "movies_") != NULL && strstr(dir->d_name, ".csv") != NULL) {
                            flag = 1;
                            fseek(fp, 0L, SEEK_END);
                            long int res = ftell(fp);
                            fclose(fp);
                            // printf("Size of the file is %ld bytes \n", res);
                            if (s == 0) {
                                maximum = res;
                                strcpy(filename, dir->d_name);
                                s++;
                            }

                            else {
                                if (res > maximum) {
                                    maximum = res;
                                    strcpy(filename, dir->d_name);
                                    s++;
                                }
                                else {
                                    s++;
                                }
                            }
                        }
                                                
                    }
                    if (flag == 1) {
                        printf("Reading file name %s\n", filename);
                        printf("The size of the file is %ld bytes\n\n", maximum);
                    } else {
                        printf("No files that start with movies_ or have an extension of .csv\n\n");
                    }
                    

                    // generate random number

                    srand(time(0));

                    int i = rand() % 99999;
                    char output[25];

                    // printf("The random number generated was %d\n\n",i);

                    snprintf(output, 25, "alveyn.movies.%d", i);

                    printf("Created directory with name: %s\n", output);

                    mkdir(output, 0755);

                    closedir(d);
                };
            }

            // If the user inputs 2, the current directory is opened and the minumum file size is searched

            else if (inputfromuser2 == 2) {

                // The first 10 lines were assisted with resources from: https://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program/17683417
                // and: https://www.geeksforgeeks.org/c-program-find-size-file/
                // These served to add more detail to the exploration

                DIR* d;
                struct dirent* dir;
                d = opendir(".");
                int s = 0;
                long int minimum;
                char filename[255];
                int flag = 0;
                if (d) {
                    while ((dir = readdir(d)) != NULL) {
                        printf("%s\n", dir->d_name);
                        FILE* fp = fopen(dir->d_name, "r");

                        // if statement to verify that movies_ and .csv are contained within the file name

                        if (strstr(dir->d_name, "movies_") != NULL && strstr(dir->d_name, ".csv") != NULL) {
                            flag = 1;
                            fseek(fp, 0L, SEEK_END);
                            long int res2 = ftell(fp);
                            fclose(fp);
                            // printf("Size of the file is %ld bytes \n\n", res2);
                            if (res2 == -1) {
                                minimum = 0;
                            } 

                            else {
                                if (s == 0) {
                                minimum = res2;
                                strcpy(filename, dir->d_name);
                                s++;
                                }

                                else {
                                    if (res2 < minimum) {
                                        minimum = res2;
                                        strcpy(filename, dir->d_name);
                                        s++;
                                    }
                                    else {
                                        s++;
                                    }
                                }   
                            }
                        }

                        
                    }
                    if (flag == 1) {
                        printf("Reading file name %s\n", filename);
                        printf("The size of the file is %ld bytes\n\n", minimum);
                    } else {
                        printf("No files that start with movies_ or have an extension of .csv\n\n");
                    }

                    // generate random number

                    srand(time(0));

                    int i = rand() % 99999;
                    char output[25];

                    // printf("The random number generated was %d\n\n",i);

                    snprintf(output, 25, "alveyn.movies.%d", i);

                    printf("Created directory with name: %s\n", output);

                    mkdir(output, 0755);

                    closedir(d);
                };
            }

            else if (inputfromuser2 == 3) {

                // The first 10 lines were assisted with resources from: https://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program/17683417
                // and: https://www.geeksforgeeks.org/c-program-find-size-file/
                // These served to add more detail to the exploration

                DIR* d;
                struct dirent* dir;
                d = opendir(".");
                int s = 0;
                int flag = 0;
                char filename[255];
                char string_input[255];
                printf("Enter the complete file name: ");
                scanf("%s", &string_input);
                printf("Now looking for %s\n\n", string_input);
                if (d) {
                    while ((dir = readdir(d)) != NULL) {
                        // printf("%s\n", dir->d_name);
                        FILE* fp = fopen(dir->d_name, "r");

                        if (fp == NULL) {
                            printf("File Not Found!\n\n");
                            tflag = 1;
                        }

                        else if (strstr(dir->d_name, string_input) != NULL){
                            flag = 1;
                            printf("Found it!\n\n");
                            tflag = 0;
                        }
                    }
                    if (flag != 1){
                        printf("File Not Found!\n\n");
                        tflag = 1;
                    }
                }
            }

            else {
                printf("Wrong input detected, try again!\n\n");
            };


        }

        else if (inputfromuser == 2) {
            a = 1;
            printf("Goodbye!\n\n");
        }

        else {
            printf("Wrong input detected, try again!\n\n");
        };
    };
};

        

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
