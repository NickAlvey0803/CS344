// Assignment2.c : This file contains the 'main' function. Program execution begins and ends there.
//

#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

// Driver Code
int main(void)
{
    // Substitute the full file path
    // for the string file_path

    // Beginning user interface. Starts with greeting ("Processed [filename], # of movies")

    int a = 0;

    while (a < 1) {

        // Menu for user to choose from

        int inputfromuser;
        printf("1. Select File to process\n");
        printf("2. Exit the program\n");
        printf("Enter a choice 1 or 2: ");
        scanf("%i", &inputfromuser);
        printf("\n\n");

        if (inputfromuser == 1) {
            int inputfromuser2;
            printf("Which file do you want to process? Note: I only look in the current directory.\n");
            printf("Enter 1 to pick the largest file\n");
            printf("Enter 2 to pick the smallest file\n");
            printf("Enter 3 to specify the name of a file\n\n");
            printf("Enter a choice from 1 to 3: ");
            scanf("%i", &inputfromuser2);
            printf("\n");
            if (inputfromuser2 == 1) {

                // https://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program/17683417

                DIR* d;
                struct dirent* dir;
                d = opendir(".");
                if (d) {
                    while ((dir = readdir(d)) != NULL) {
                        printf("%s\n", dir->d_name);
                    }
                    closedir(d);
                };
            }

            else if (inputfromuser2 == 2) {

            }

            else if (inputfromuser2 == 3) {

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
