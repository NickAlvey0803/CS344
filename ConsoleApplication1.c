// Nick Alvey
// Assignment 1

// Some notes: I parts of two scripts from online - I tried to call them out very clearly. I was struggling to pull and then parse a CSV file into a Struct, so I found a method
// to do so from Geeks for Geeks, and modified with some suggestions from Stack Exchange

// ConsoleApplication1.c : This file contains the 'main' function. Program execution begins and ends there.
//

// Reading CSV help from (line 17 to 102 is about 60% from Geeks for Geeks with modifications from Stack Overflow:
// https://www.geeksforgeeks.org/relational-database-from-csv-files-in-c/ and https://stackoverflow.com/questions/16869467/command-line-arguments-reading-a-file/16869485


// The packages included for this program
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Driver Code
int main(int argc, char* argv [])
{
    // Substitute the full file path
    // for the string file_path
    FILE* fp = fopen(argv[1], "r");

    if (!fp)
        printf("Can't open file\n");

    else {
        // Here we have taken size of
        // array 1024 you can modify it
        char buffer[1024];

        // Struct created for each movie
        struct movie {
            char* title;
            int year;
            char* languages;
            //float ratings;
            float ratings;
        };

        //Unused struct as I was planning how to classify by ratings
        struct goodrate {
            char* title;
            int year;
            float ratings;
        };

        // creating an array of structs to input the CSV data into
        struct movie movies[50];

        // initializing values to help pull data from the CSV
        int row = 0;
        int column = 0;
        int i = 0;

        while (fgets(buffer,
            1024, fp)) {
            column = 0;
            row++;

            // To avoid printing of column
            // names in file can be changed
            // according to need
            if (row == 1)
                continue;

            // Splitting the data
            char* value = strtok(buffer, ",");

            while (value) {

                // Column 1
                if (column == 0) {
                    //printf("Title :");
                    movies[i].title = strdup(value);
                }

                // Column 2
                if (column == 1) {
                    //printf("\tYear :");
                    //char* temp = value;
                    //int ftemp = atoi(temp);
                    movies[i].year = atoi(value);
                }

                // Column 3
                if (column == 2) {
                    //printf("\tLanguages :");
                    movies[i].languages = strdup(value);
                }

                // Column 4
                if (column == 3) {
                    //printf("\tRating Value :");
                    //char* tmp = value;
                    //float ftmp = atof(tmp);
                    //movies[i].ratings = atof(str,NULL);
                    movies[i].ratings = strtod(value, NULL);
                }

                //printf("%s", value);
                value = strtok(NULL, ",");
                column++;
            }

            // Troubleshooting print statement to verify that my structure had been created as intended.

            //printf("index i= %i  Title: %s, %i, %s, %1.1f \n", i, movies[i].title, movies[i].year, movies[i].languages, movies[i].ratings);
            i++;
        }


        // Beginning user interface. Starts with greeting ("Processed [filename], # of movies")

        int a = 0;
        printf("Processed file %s and parsed data for %i movies\n\n", argv[1], i);
        while (a < 1) {

            // Menu for user to choose from

            int inputfromuser;
            printf("1. Show movies released in the specified year\n");
            printf("2. SHow highest rated movie for each year\n");
            printf("3. Show the title and year of release of all movies in a specific language\n");
            printf("4. Exit from the program\n\n");
            printf("Enter a choice from 1 to 4: ");
            scanf("%i", &inputfromuser);


            // Beginning of 3 data manipulation sections (based on if the user inputted 1, 2, or 3 (since 4 or a different value either exits or brings the
            // menu up again).

            // Inputting 1 brings another user input to enter a year. A for loop is used to print out every film title from that year

            if (inputfromuser == 1) {
                int inputfromuser2;
                int j;
                printf("Enter the year for which you want to see movies: ");
                scanf("%i", &inputfromuser2);
                for (j = 0; j < i; j++) {
                    if (movies[j].year == inputfromuser2) {
                        printf("\n%s", movies[j].title);
                    };

                };
                printf("\n\n");
            }

            // INputting 2 automatically prints out the highest rated movie from each year. This was done by creating 1 nest for loop to create an array
            // of all years presented, then using that array in another nested for loop to take the highest rated movie that also matched that year

            // While not necessarily efficient, this was the way I could think of doing it without messing up my pointers too badly. Any suggestions on how
            // to do this more efficiently would be appreciated.

            else if (inputfromuser == 2) {
                int arr[50];
                int index = 0;
                int index2 = 0;
                int j;
                int b;
                int c;
                int k;
                int flag;
                float rate;
                for (j = 0; j < i; j++) {
                    flag = 0;
                    for (b = 0; b < i; b++) {
                        if (movies[j].year == arr[b]) {
                           flag = 1;
                        };
                    };
                    if (flag != 1) {
                        arr[index] = movies[j].year;
                        index++;
                    };
                };

                for (c = 0; c < index; c++) {
                    flag = 0;
                    k = 0;
                    rate = 0.0;
                    for (b = 0; b < i; b++) {

                        if (arr[c] == movies[b].year) {

                            if (flag == 0) {
                                rate = movies[b].ratings;
                                k = b;
                                flag = 1;
                            }
                            else {
                                if (movies[b].ratings > rate) {
                                    k = b;
                                    rate = movies[b].ratings;
                                };

                            };


                        };

                    };
                    printf("%i %1.1f %s\n", movies[k].year, movies[k].ratings, movies[k].title);
                };

                printf("\n ");
            }

            // Inputting 3 brings up another user input to enter the language.

            // Instead of going through the trouble of creating another array and parsing based on a semi-colon, I left it as a string and searched for
            // a substring. If found it would print out the required information.

            else if (inputfromuser == 3) {
                char *ret;
                char inputfromuser3[256];
                int j;
                printf("Enter the language for which you want to see movies: ");
                scanf("%s", &inputfromuser3);
                for (j = 0; j < i; j++) {

                    if (movies[j].languages != inputfromuser3) {
                        printf("\n%i %s", movies[j].year, movies[j].title);
                    };
                };
                printf("\n\n");
            }

            //Inputting 4 exits the program

            else if (inputfromuser ==4) {
                a = 1;
            }

            // Inputting anything else requires the user to try again

            else {
                printf("\nYou entered an incorrect choice. Try again.\n\n");
            };

        };

        // Close the file
        fclose(fp);
    }
    return 0;
}

// I kept this since this is my first c script (I'm using Visual Studio). I found it helpful.

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
