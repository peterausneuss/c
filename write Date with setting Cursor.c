#include <stdio.h>
#include <time.h> // Required for date and time functions

int main() {
    time_t now;
    struct tm *tm_info;
    char datetime_str[64]; // Buffer for formatted date/time. Max "YYYY-MM-DD HH:MM:SS" is 19 chars + null.
    size_t dt_len;

    // 1. Get current time
    now = time(NULL);
    if (now == (time_t)(-1)) {
        perror("Error getting time"); // perror prints a system error message to stderr
        return 1; // Indicate an error
    }

    // 2. Convert to local time structure
    tm_info = localtime(&now);
    if (tm_info == NULL) {
        fprintf(stderr, "Error converting to local time.\n");
        return 1; // Indicate an error
    }

    // 3. Format date and time string
    // Example format: "YYYY-MM-DD HH:MM:S"
    dt_len = strftime(datetime_str, sizeof(datetime_str), "%Y-%m-%d %H:%M:%S", tm_info);
    if (dt_len == 0) {
        // This means strftime failed (e.g. buffer too small, or an issue with locale)
        fprintf(stderr, "Error formatting date/time string.\n");
        return 1; // Indicate an error
    }

    // --- Screen Manipulation Starts Here ---

    // 4. Clear screen and move cursor to top-left (home position)
    //    \033[2J: Erase entire screen
    //    \033[H:  Move cursor to home position (row 1, column 1)
    printf("\033[2J\033[H");

    // 5. Position cursor for writing in the upper right corner
    //    Strategy:
    //    a. Move cursor to the first row and a very far right column (e.g., 999).
    //       The terminal will clamp this to its actual maximum column.
    //    b. Move the cursor left by the length of the date/time string.
    //    \033[1;999H: Move to row 1, column 999 (effectively far right)
    //    \033[%zuD:   Move cursor %zu positions to the left
    printf("\033[1;999H\033[%zuD", dt_len);

    // 6. Print the date and time string
    printf("%s", datetime_str); // datetime_str is null-terminated by strftime

    // 7. Move cursor to a new line (e.g., 3rd line, 1st column) to ensure
    //    the shell prompt appears cleanly below the printed date/time.
    //    \033[3;1H: Move cursor to row 3, column 1
    printf("\033[3;1H");

    // 8. Flush the output buffer to ensure everything is displayed immediately.
    //    This is important when using ANSI escape codes without necessarily
    //    printing a newline, which might otherwise trigger a flush on line-buffered terminals.
    fflush(stdout);

    return 0; // Indicate successful execution
}


