#include <gtk/gtk.h>

// To Compile: gcc -o tictactoe tictactoe.c `pkg-config --cflags --libs gtk+-3.0`
// To Run: ./tictactoe

#include <gtk/gtk.h>

#define SIZE 3          
#define BUTTON_SIZE 100 

GtkWidget *buttons[SIZE][SIZE]; // 3x3 grid of buttons
char current_player = 'X'; //Player 1 gets X

int check_winner()
{
   //Goes through each row and column to see if there is a winner horizontally or vertically.
    for (int i = 0; i < SIZE; i++)
    {
        if (gtk_button_get_label(GTK_BUTTON(buttons[i][0])) &&
            gtk_button_get_label(GTK_BUTTON(buttons[i][0]))[0] != '\0' &&
            gtk_button_get_label(GTK_BUTTON(buttons[i][0]))[0] == gtk_button_get_label(GTK_BUTTON(buttons[i][1]))[0] &&
            gtk_button_get_label(GTK_BUTTON(buttons[i][1]))[0] == gtk_button_get_label(GTK_BUTTON(buttons[i][2]))[0])
        {
            return 1; 
        }

        if (gtk_button_get_label(GTK_BUTTON(buttons[0][i])) &&
            gtk_button_get_label(GTK_BUTTON(buttons[0][i]))[0] != '\0' &&
            gtk_button_get_label(GTK_BUTTON(buttons[0][i]))[0] == gtk_button_get_label(GTK_BUTTON(buttons[1][i]))[0] &&
            gtk_button_get_label(GTK_BUTTON(buttons[1][i]))[0] == gtk_button_get_label(GTK_BUTTON(buttons[2][i]))[0])
        {
            return 1; 
        }
    }

    // Checks if there was a winner diagonally.
    if (gtk_button_get_label(GTK_BUTTON(buttons[0][0])) &&
        gtk_button_get_label(GTK_BUTTON(buttons[0][0]))[0] != '\0' &&
        gtk_button_get_label(GTK_BUTTON(buttons[0][0]))[0] == gtk_button_get_label(GTK_BUTTON(buttons[1][1]))[0] &&
        gtk_button_get_label(GTK_BUTTON(buttons[1][1]))[0] == gtk_button_get_label(GTK_BUTTON(buttons[2][2]))[0])
    {
        return 1; 
    }

    if (gtk_button_get_label(GTK_BUTTON(buttons[0][2])) &&
        gtk_button_get_label(GTK_BUTTON(buttons[0][2]))[0] != '\0' &&
        gtk_button_get_label(GTK_BUTTON(buttons[0][2]))[0] == gtk_button_get_label(GTK_BUTTON(buttons[1][1]))[0] &&
        gtk_button_get_label(GTK_BUTTON(buttons[1][1]))[0] == gtk_button_get_label(GTK_BUTTON(buttons[2][0]))[0])
    {
        return 1; 
    }

    // Checks for a draw (all filled, no winner)
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
        return 2; 
    }

    return 0; 
}

//To reset the game at the end
void reset_game()
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            gtk_button_set_label(GTK_BUTTON(buttons[i][j]), "");
            gtk_widget_set_name(buttons[i][j], "button"); 
        }
    }
    current_player = 'X';
}

// Tells what to do when button is clicked.
void on_button_clicked(GtkWidget *widget, gpointer data)
{
    const char *label = gtk_button_get_label(GTK_BUTTON(widget));
    if (label && *label != '\0')
        return;

    //Set to X or O based on who current_player is.
    const char *symbol = (current_player == 'X') ? "X" : "O";
    gtk_button_set_label(GTK_BUTTON(widget), symbol);

    gtk_widget_set_name(widget, (current_player == 'X') ? "x" : "o");

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

    //Switch current_player.
    current_player = (current_player == 'X') ? 'O' : 'X';
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Tic Tac Toe");
    gtk_window_set_default_size(GTK_WINDOW(window), SIZE * BUTTON_SIZE, SIZE * BUTTON_SIZE);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            buttons[i][j] = gtk_button_new_with_label("");
            gtk_grid_attach(GTK_GRID(grid), buttons[i][j], i, j, 1, 1);
            gtk_widget_set_size_request(buttons[i][j], BUTTON_SIZE, BUTTON_SIZE); 
            gtk_widget_set_name(buttons[i][j], "button");                       
            g_signal_connect(buttons[i][j], "clicked", G_CALLBACK(on_button_clicked), NULL);
        }
    }

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}