#include <stdio.h>
#include <time.h>

/*
asctime(const struct tm *timeptr): No formatting options.
Always outputs in "Www Mmm dd hh:mm:ss yyyy\n" format.

strftime(char *str, size_t maxsize, const char *format, const struct tm *timeptr):
Highly configurable formatting via the format string.


*/
int main() {
    int a = 5; // Declare an integer variable 'a' and initialize it to 5
    double b = 3.14; // Declare a double variable 'b' and initialize it to 3.14
    char c = 'A'; // Declare a character variable 'c' and initialize it to 'A'

// --- Corrected time handling ---
    time_t raw_time;      // Variable to hold the raw calendar time (seconds since epoch)
    struct tm *time_info; // Pointer to a struct tm to hold broken-down local time

    // 1. Get the current calendar time
    time(&raw_time);
    // Alternatively: raw_time = time(NULL);

    // 2. Convert the raw time to local time structure
    time_info = localtime(&raw_time);

    // 3. Print the current time using asctime
    //    asctime takes a 'const struct tm *' and returns a string.
    //    It's good practice to check if time_info is NULL (localtime can fail).
    if (time_info != NULL) {
        printf("Current time: %s", asctime(time_info)); // asctime's string includes a newline
        // Example using strftime for custom formatting
        char custom_time_str[100]; // Buffer to hold the formatted string
        // Format: YYYY-MM-DD HH:MM:SS
        if (strftime(custom_time_str, sizeof(custom_time_str), "%Y-%m-%d %H:%M:%S", time_info) > 0) {
            printf("Current time (strftime): %s\n", custom_time_str);
            strftime(custom_time_str, sizeof(custom_time_str), "%y%m%d %H:%M", time_info);
            printf("Current time (strftime): %s\n", custom_time_str);

        } else {
            fprintf(stderr, "Failed to format time with strftime.\n");
        }
    } else {
        fprintf(stderr, "Failed to get local time.\n");
    }

    // Print the values of the variables to the console
    printf("Integer: %d\n", a);
    printf("Double: %.2f\n", b);
    printf("Character: %c\n", c);
    // Print the size of each variable type
    printf("Size of int: %zu bytes\n", sizeof(a));
    printf("Size of double: %zu bytes\n", sizeof(b));
    printf("Size of char: %zu bytes\n", sizeof(c));
    // Print the size of the int type
    printf("Size of int type: %zu bytes\n", sizeof(int));
    // Print the size of the double type
    printf("Size of double type: %zu bytes\n", sizeof(double));
    // Print the size of the char type
    printf("Size of char type: %zu bytes\n", sizeof(char));
    // Print the size of the main function

    printf("Zahl:");scanf("%d", &a); // Read an integer from user input and store it in 'a'
    printf("You entered: %d an der Speicherstelle %p\n", a,&a); // Print the value entered by the user
    printf("Size of Adresse: %zu bytes\n", sizeof(&a)); // Print the size of the address of 'a'

    printf("Hello, World!\n");
    
    // Return 0 to indicate successful execution
    return 0;
}