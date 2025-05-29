#include <stdio.h>
#include <string.h>   // For strlen()
#include <unistd.h>   // For usleep(), ioctl(), STDOUT_FILENO
#include <stdlib.h>   // For exit()
#include <sys/ioctl.h> // For struct winsize, TIOCGWINSZ (on Unix-like systems)
#include <signal.h>   // For signal(), SIGINT

// Function to get the terminal width
int get_terminal_width() {
    struct winsize w;
    // Try to get terminal size
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_col > 0) {
        return w.ws_col;
    }
    return 80; // Default to 80 columns if unable to determine
}

// Signal handler for Ctrl+C (SIGINT)
void handle_sigint(int sig) {
    (void)sig; // Unused parameter
    // Restore cursor visibility
    printf("\033[?25h");
    // Move cursor to a new line and clear screen for a clean exit
    printf("\033[2J\033[H"); // Clear screen and move to home
    fflush(stdout);
    exit(0);
}

int main() {
    const char *message = "hello";
    size_t message_len = strlen(message);
    int terminal_width;
    int current_col;
    int direction = -1; // Start by moving from right to left (-1 for left, 1 for right)
    int min_col = 1;
    int max_start_col;

    // Register signal handler for SIGINT (Ctrl+C)
    signal(SIGINT, handle_sigint);

    // Hide cursor for smoother animation
    printf("\033[?25l");
    fflush(stdout);

    terminal_width = get_terminal_width();

    // If terminal is too narrow for the message, just print it once and exit.
    if (terminal_width < message_len) {
        printf("\033[1;1H\033[2K"); // Clear line 1
        printf("\033[1;1H%s\n", message); // Print at top-left
        printf("Terminal too narrow for animation.\n");
        printf("\033[?25h"); // Show cursor
        fflush(stdout);
        return 1;
    }

    max_start_col = terminal_width - (int)message_len + 1;
    current_col = max_start_col; // Start at the far right

    // Animation loop
    while (1) {
        // 1. Clear the top line and move cursor to its beginning
        //    \033[1;1H: Move cursor to row 1, column 1
        //    \033[2K:   Erase entire line
        printf("\033[1;1H\033[2K");

        // 2. Position cursor and print the message
        //    \033[1;%dH: Move cursor to row 1, column `current_col`
        printf("\033[1;%dH%s", current_col, message);

        // 3. Flush output to ensure it's displayed immediately
        fflush(stdout);

        // 4. Pause for a short duration (e.g., 100 milliseconds)
        usleep(100000);

        // 5. Update position
        current_col += direction;

        // 6. Check boundaries and reverse direction if an edge is hit
        if (direction == -1 && current_col < min_col) { // Moving left, hit left edge
            current_col = min_col;
            direction = 1; // Change direction to right
        } else if (direction == 1 && current_col > max_start_col) { // Moving right, hit right edge
            current_col = max_start_col;
            direction = -1; // Change direction to left
        }
    }

    // The following is unreachable due to the infinite loop and signal handler,
    // but good practice to include a return statement in main.
    // Cleanup (show cursor) would typically be in the signal handler.
    printf("\033[?25h"); // Show cursor
    fflush(stdout);
    return 0;
}
