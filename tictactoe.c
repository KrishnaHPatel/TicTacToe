#include <gtk/gtk.h>

// To Compile: gcc -o tictactoe tictactoe.c `pkg-config --cflags --libs gtk+-3.0`
// To Run: ./tictactoe

#include <gtk/gtk.h>

#define SIZE 3          // Size of the Tic Tac Toe board (3x3)
#define BUTTON_SIZE 100 // Size of each button in pixels

// Global variables
GtkWidget *buttons[SIZE][SIZE]; // 3x3 grid of buttons
char current_player = 'X';      // 'X' starts the game

// Function to check if there is a winner
int check_winner()
{
    // Check rows and columns
    for (int i = 0; i < SIZE; i++)
    {
        // Check row
        if (gtk_button_get_label(GTK_BUTTON(buttons[i][0])) &&
            gtk_button_get_label(GTK_BUTTON(buttons[i][0]))[0] != '\0' &&
            gtk_button_get_label(GTK_BUTTON(buttons[i][0]))[0] == gtk_button_get_label(GTK_BUTTON(buttons[i][1]))[0] &&
            gtk_button_get_label(GTK_BUTTON(buttons[i][1]))[0] == gtk_button_get_label(GTK_BUTTON(buttons[i][2]))[0])
        {
            return 1; // Winner found
        }

        // Check column
        if (gtk_button_get_label(GTK_BUTTON(buttons[0][i])) &&
            gtk_button_get_label(GTK_BUTTON(buttons[0][i]))[0] != '\0' &&
            gtk_button_get_label(GTK_BUTTON(buttons[0][i]))[0] == gtk_button_get_label(GTK_BUTTON(buttons[1][i]))[0] &&
            gtk_button_get_label(GTK_BUTTON(buttons[1][i]))[0] == gtk_button_get_label(GTK_BUTTON(buttons[2][i]))[0])
        {
            return 1; // Winner found
        }
    }

    // Check diagonals
    if (gtk_button_get_label(GTK_BUTTON(buttons[0][0])) &&
        gtk_button_get_label(GTK_BUTTON(buttons[0][0]))[0] != '\0' &&
        gtk_button_get_label(GTK_BUTTON(buttons[0][0]))[0] == gtk_button_get_label(GTK_BUTTON(buttons[1][1]))[0] &&
        gtk_button_get_label(GTK_BUTTON(buttons[1][1]))[0] == gtk_button_get_label(GTK_BUTTON(buttons[2][2]))[0])
    {
        return 1; // Winner found
    }

    if (gtk_button_get_label(GTK_BUTTON(buttons[0][2])) &&
        gtk_button_get_label(GTK_BUTTON(buttons[0][2]))[0] != '\0' &&
        gtk_button_get_label(GTK_BUTTON(buttons[0][2]))[0] == gtk_button_get_label(GTK_BUTTON(buttons[1][1]))[0] &&
        gtk_button_get_label(GTK_BUTTON(buttons[1][1]))[0] == gtk_button_get_label(GTK_BUTTON(buttons[2][0]))[0])
    {
        return 1; // Winner found
    }

    // Check for draw
    int filled_cells = 0;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (gtk_button_get_label(GTK_BUTTON(buttons[i][j])) &&
                gtk_button_get_label(GTK_BUTTON(buttons[i][j]))[0] != '\0')
            {
                filled_cells++;
            }
        }
    }

    if (filled_cells == SIZE * SIZE)
    {
        return 2; // Draw
    }

    return 0; // No winner or draw yet
}

// Function to reset the game
void reset_game()
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            gtk_button_set_label(GTK_BUTTON(buttons[i][j]), "");
            gtk_widget_set_name(buttons[i][j], "button"); // Reset the button style
        }
    }
    current_player = 'X';
}

// Function to handle button clicks
void on_button_clicked(GtkWidget *widget, gpointer data)
{
    // Get the button label to see if it is already clicked
    const char *label = gtk_button_get_label(GTK_BUTTON(widget));
    if (label && *label != '\0')
        return; // Button already clicked

    // Set button label to the current player's symbol
    const char *symbol = (current_player == 'X') ? "X" : "O";
    gtk_button_set_label(GTK_BUTTON(widget), symbol);

    // Set button style based on symbol
    gtk_widget_set_name(widget, (current_player == 'X') ? "x" : "o");

    // Check for winner or draw
    int result = check_winner();
    if (result == 1)
    {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                                   "Player %c wins!", current_player);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        reset_game();
    }
    else if (result == 2)
    {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                                   "It's a draw!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        reset_game();
    }

    // Switch player
    current_player = (current_player == 'X') ? 'O' : 'X';
}

int main(int argc, char *argv[])
{
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Tic Tac Toe");
    gtk_window_set_default_size(GTK_WINDOW(window), SIZE * BUTTON_SIZE, SIZE * BUTTON_SIZE);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a grid for the Tic Tac Toe board
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Create buttons for the Tic Tac Toe grid
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            buttons[i][j] = gtk_button_new_with_label("");
            gtk_grid_attach(GTK_GRID(grid), buttons[i][j], i, j, 1, 1);
            gtk_widget_set_size_request(buttons[i][j], BUTTON_SIZE, BUTTON_SIZE); // Set button size
            gtk_widget_set_name(buttons[i][j], "button");                         // Apply initial style
            g_signal_connect(buttons[i][j], "clicked", G_CALLBACK(on_button_clicked), NULL);
        }
    }

    // Show all widgets
    gtk_widget_show_all(window);

    // Run the GTK main loop
    gtk_main();

    return 0;
}
