#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 8
#define MAX_USERNAME_LENGTH 50


// Structure to represent coordinates on the game board
typedef struct {
    int row;
    int y;
} Coordinate;


// Structure to represent a ship
typedef struct {
    Coordinate cells;
    int isHit;
} Ship;



// Structure to represent the game state
typedef struct {
    char playerBoard[BOARD_SIZE][BOARD_SIZE];
    char enemyBoard[BOARD_SIZE][BOARD_SIZE];
    Ship playerShips[5];
    Ship enemyShips[5];
    char username[MAX_USERNAME_LENGTH];
    int current_Id;
} Game;


// Function prototypes
void initializeGame(Game* game);
void printBoard(const char board[BOARD_SIZE][BOARD_SIZE]);
Coordinate getCoordinate();
void placePlayerShips(Game* game);
void placeEnemyShips(Game* game);
int checkHit(Game* game, Coordinate attack, int isPlayer, int* shipNumber);
void start_logo();
void start_menu();
void new_game(Game* game, int* playerShipNumber, int* enemyShipNumber);
void saveGame(char PlayerField[BOARD_SIZE][BOARD_SIZE], char ComputerField[BOARD_SIZE][BOARD_SIZE], int current_Id, char username[MAX_USERNAME_LENGTH]);
int loadGame(char PlayerField[BOARD_SIZE][BOARD_SIZE], char ComputerField[BOARD_SIZE][BOARD_SIZE], int* current_Id, char username[MAX_USERNAME_LENGTH], int GameId);
void parseField(const char* fieldString, char field[BOARD_SIZE][BOARD_SIZE]);
void game_procces(Game* game, int* playerShipNumber, int* enemyShipNumber);
void listAllSavedGames();
void listAllSavedGamesByUsername(const char username[MAX_USERNAME_LENGTH]);
//int isValidMove(Game* game, Coordinate move);

// Main function of the Battleship game
int main() {
    // Display the game logo
    start_logo();

    // Initialize the game structure
    Game game;
    initializeGame(&game);

    // Initialize ship counts for both player and enemy
    int playerShipNumber = 5;
    int enemyShipNumber = 5;

    // Variables for user menu choices
    int choice;
    int choice2;

    // Main game loop
    do {
        // Display the main menu
        start_menu();

        // Get the user's choice
#pragma warning(suppress : 4996)
        scanf("%d", &choice);

        // Process the user's choice
        switch (choice) {
        case 1:
            // Start a new game
            new_game(&game, &playerShipNumber, &enemyShipNumber);
            game_procces(&game, &playerShipNumber, &enemyShipNumber);
            break;
        case 2:
            // Load game submenu
            while (1) {
                printf("\nChoose a game:\n\n");
                printf("> Press 1 to List all Saved Games\n");
                printf("> Press 2 to Load the Last Game by ID\n");
                printf("> Press 3 to Search Games by Username\n");
                printf("> Press 4 to Return to Main Menu\n\n");

                // Get the choice for the load game submenu
                int choice2;
#pragma warning(suppress : 4996)
                scanf("%d", &choice2);

                switch (choice2) {
                case 1:
                    // List all saved games
                    listAllSavedGames();
                    break;
                case 2: {
                    // Load a specific game by ID
                    printf("> Enter game ID to load: ");
                    int gameId;
#pragma warning(suppress : 4996)
                    scanf("%d", &gameId);

                    // Attempt to load the game
                    Game loadedGame;
                    int success = loadGame(loadedGame.playerBoard, loadedGame.enemyBoard, &loadedGame.current_Id, loadedGame.username, gameId);

                    if (success) {
                        printf("Game loaded successfully!\n");
                        game_procces(&loadedGame, &playerShipNumber, &enemyShipNumber);
                    }
                    else {
                        printf("Failed to load the game.\n");
                    }
                    break;
                }
                case 3: {
                    // Search games by username
                    printf("> Enter username to list saved games: ");
                    char username[MAX_USERNAME_LENGTH];
#pragma warning(suppress : 4996)
                    scanf("%s", username);
                    listAllSavedGamesByUsername(username);
                    break;
                }
                case 4:
                    // Return to the main menu
                    printf("\n(Returning to the Main Menu)\n\n");
                    break;
                default:
                    // Invalid command in the load game submenu
                    printf("\nUnknown command (Try Again)\n");
                    break;
                }

                // Exit the load game submenu if the user chooses option 4
                if (choice2 == 4) {
                    break;
                }
            }
            break;
        case 3:
            // Exit the game
            printf("\nSee you!\n");
            exit(0);
            break;
        default:
            // Invalid command in the main menu
            printf("\nUnknown command (Try Again)\n");
        }
    } while (choice != 4);

    // Exit the program
    return 0;
}



// Function to print the start logo
void start_logo()
{
    printf("\n");
    printf("  ____   *    _   _   _   *  ____ _                 ____  *                 *\n");
    printf(" | __ )  __ _| |_| |_| | ___/ ___|| |__ (_)_ __    / ___| __ _ _ __ ___   ___ \n");
    printf(" |  _ \\ / _` | __| __| |/ _ \\___ \\| '_ \\| | '_ \\  | |  _ / _` | '_ ` _ \\ / _ \\ \n");
    printf(" | |_) | (_| | |_| |_| |  __/___) | | | | | |_) | | |_| | (_| | | | | | |  __/ \n");
    printf(" |____/ \\__,_|\\__|\\__|_|\\___|____/|_| |_|_| .__/   \\____|\\__,_|_| |_| |_|\\___| \n");
    printf("\n");
    printf("   *                      *                    *                  *          \n");
    printf("\n");
    printf("\n");

}

// Function to display the start menu
void start_menu()
{
    printf("*******************************************************************************\n");
    printf("*                                Main Menu                                    *\n");
    printf("*******************************************************************************\n");
    printf("\n");
    printf(" > Press 1 for a New Game\n");
    printf(" > Press 2 to Load the Game\n");
    printf(" > Press 3 for Exit\n");
    printf("\n");
}


// Function to start a new game
void new_game(Game* game, int* playerShipNumber, int* enemyShipNumber) {
    printf("\nEnter your name:\n");

#pragma warning(suppress : 4996)
    scanf("%s", game->username);

    placePlayerShips(game);

    placeEnemyShips(game);

}

// Function to handle the game process, including player and enemy turns
void game_procces(Game* game, int* playerShipNumber, int* enemyShipNumber) {
    // Continue the game loop as long as both player and enemy have ships remaining
    while (*playerShipNumber > 0 && *enemyShipNumber > 0) {
        // Display the current state of the game boards
        printf("\n**Enemy's board**\n\n");
        printBoard(game->enemyBoard);
        printf("\n**Your's board**\n\n");
        printBoard(game->playerBoard);

        // Player's turn: Get valid coordinates from the player
        Coordinate player_attack = getCoordinate();
        //while (!isValidMove(game, player_attack));

        // Enemy's turn: Generate valid random coordinates for the enemy
        Coordinate enemy_attack = {1 + rand() % BOARD_SIZE, 1 + rand() % BOARD_SIZE};
         //while (!isValidMove(game, enemy_attack));

        // Process and display the result of player's attack
        if (checkHit(game, player_attack, 1, enemyShipNumber)) {
        }
        else {
            printf("No, you missed!\n");
        }

        // Process and display the result of enemy's attack
        if (checkHit(game, enemy_attack, 0, playerShipNumber)) {
            printf("The Bot hit your ship!\n");
        }

        // Save the current state of the game after each turn
        saveGame(game->playerBoard, game->enemyBoard, &game->current_Id, game->username);

        if (*enemyShipNumber == 0) {
            printf("\n\nCongratulations! You win!\n\n");
            break;
        }
        if (*playerShipNumber == 0) {
            printf("\n\nGame over! All your ships have been destroyed. The enemy wins.\n\n");
            break; 
        }
    }
}


// Function to save the current state of the game to a file
void saveGame(char PlayerBoard[BOARD_SIZE][BOARD_SIZE], char EnemyBoard[BOARD_SIZE][BOARD_SIZE], int current_Id, char username[MAX_USERNAME_LENGTH]) {
    // Open the original save file for reading.
    FILE* originalFile = fopen("saves.csv", "r");
    // Open a temporary file for writing.
    FILE* tempFile = fopen("tempfile.csv", "w");

    // Check if the temporary file was successfully opened.
    if (tempFile == NULL) {
        printf("Error opening temp file for writing.\n");
        // If the original file was opened, close it.
        if (originalFile != NULL) fclose(originalFile);
        return;
    }

    // If the original file was opened, proceed with processing.
    if (originalFile != NULL) {
        char line[256];  // Buffer to store each line read from the original file.
        int lineID;      // Variable to store the game ID read from each line.

        // Read each line from the original save file.
        while (fgets(line, sizeof(line), originalFile)) {
#pragma warning(suppress : 4996)
            sscanf(line, "%d,", &lineID); // Extract the game ID from the line.

            // If the game ID in the line is different from the current game ID, copy the line to the temp file.
            if (lineID != current_Id) {
                fputs(line, tempFile);
            }
        }

        // Close the original file after processing.
        fclose(originalFile);
    }

    // Write the current game data to the temp file.
    fprintf(tempFile, "%d,%s,", current_Id, username); // Write the game ID and username.

    // Write the player's field data.
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            fprintf(tempFile, "%c", PlayerBoard[i][j]);
        }
    }

    fprintf(tempFile, ","); // Add a comma separator.

    // Write the computer's field data.
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            fprintf(tempFile, "%c", EnemyBoard[i][j]);
        }
    }

    fprintf(tempFile, "\n"); // Add a newline at the end.
    // Close the temporary file.
    fclose(tempFile);

    // Replace the original save file with the temporary file.
    remove("saves.csv"); // Remove the original file.
    rename("tempfile.csv", "saves.csv"); // Rename the temp file to the original file's name.

    // Print a confirmation message with the current game ID.
    printf("Game saved with ID %d\n ", current_Id);
}


// Function to parse a string representing a field into a 2D array
void parseField(const char* fieldString, char field[BOARD_SIZE][BOARD_SIZE]) {
    int index = 0;
    // Iterate over rows and columns of the field array.
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            // Assign each character from the string to the corresponding element in the 2D array.
            field[i][j] = fieldString[index++];
        }
    }
}

// Function to load a game state from a file
int loadGame(char PlayerBoard[BOARD_SIZE][BOARD_SIZE], char EnemyBoard[BOARD_SIZE][BOARD_SIZE], int* current_Id, char username[MAX_USERNAME_LENGTH], int GameId) {
    // Open the save file for reading. If it fails to open, print an error message and return.
    FILE* file = fopen("saves.csv", "r");
    if (!file) {
        printf("Error opening file for reading.\n");
        return 0;
    }

    char line[1024];
    int found = 0;

    // Read each line from the file.
    while (fgets(line, sizeof(line), file)) {

        int id;
        char playerName[50];
        char playerFieldString[BOARD_SIZE * BOARD_SIZE + 1];
        char computerFieldString[BOARD_SIZE * BOARD_SIZE + 1];

        // Parse the line to extract game ID, player name, and field data strings.
#pragma warning(suppress : 4996)
        sscanf(line, "%d,%49[^,],%64[^,],%64s", &id, playerName, playerFieldString, computerFieldString);\

        // Check if the parsed game ID matches the requested GameId.
        if (id == GameId) {
            // Update the current game ID, username, and populate player and computer boards.
            *current_Id = id;
            strcpy(username, playerName);
            parseField(playerFieldString, PlayerBoard);
            parseField(computerFieldString, EnemyBoard);
            found = 1;
            break;
        }
    }

    // Close the file after reading.
    fclose(file);

    // If the requested game was not found, print a message and return 0.
    if (!found) {
        printf("Game with ID %d for user not found.\n", GameId);
        return 0;
    }

    // Return 1 to indicate successful loading of the game state.
    return 1;
}

// Function to initialize the game state
void initializeGame(Game* game) {
    // Initialize player and enemy boards with empty sea tiles.
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            game->playerBoard[i][j] = '.';
            game->enemyBoard[i][j] = '.';
        }
    }

    // Initialize player and enemy ships' hit status.
    for (int i = 0; i < 5; i++) {
        game->playerShips[i].isHit = 0;
        game->enemyShips[i].isHit = 0;
    }
}

// Function to print the game board
void printBoard(const char board[BOARD_SIZE][BOARD_SIZE]) {
    // Print column labels.
    printf("  A B C D E F G H\n");

    // Print each row of the board.
    for (int i = 0; i < BOARD_SIZE; i++) {
        // Print row number.
        printf("%d ", i + 1);

        // Print each element of the row.
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%c ", board[i][j]);
        }

        // Move to the next line for the next row.
        printf("\n");
    }
}

// Function to get user input for coordinates
Coordinate getCoordinate() {
    Coordinate coord;
    char y;
    // Prompt the user to input coordinates.
    printf("\nChoose coordinates (for example '1 A'): \n");

    // Read row and column coordinates from the user input.
#pragma warning(suppress : 4996)
    scanf("%d %c", &coord.row, &y);

    // Convert the column coordinate to a numerical index ('A' -> 0, 'B' -> 1, etc.).
    coord.y = y - 'A';


    return coord;
}


// Function to place player ships on the board
void placePlayerShips(Game* game) {
    printf("\nNow it's time to place your ships!");
    // Iterate through each player ship and place it on the board.
    for (int i = 0; i < 5; i++) {
        // Get coordinates from the user for placing the ship.
        Coordinate shipCoord = getCoordinate();
        // Update player ship information and mark the board with 'S' for the ship.
        game->playerShips[i].cells = shipCoord;
        game->playerBoard[shipCoord.row - 1][shipCoord.y] = '*';

        // Display the updated player board.
        printf("\nYour board:\n\n");
        printBoard(game->playerBoard);
    }
}

// Function to place enemy ships on the board
void placeEnemyShips(Game* game) {
    printf("\nEnemy is placing ships...\n");
    // Seed the random number generator with the current time.
    srand(time(NULL));
    Coordinate shipCoord;
    // Iterate through each enemy ship and randomly place it on the board.
    for (int i = 0; i < 5; i++) {
        // Generate random coordinates until an empty spot on the player's board is found.
        do {
            shipCoord.row = rand() % BOARD_SIZE;
            shipCoord.y = rand() % BOARD_SIZE;
        } while (game->playerBoard[shipCoord.row][shipCoord.y] != '.');

        // Update enemy ship information and mark the board with 'E' for the enemy ship.
        game->enemyShips[i].cells = shipCoord;
        //game->enemyBoard[shipCoord.row][shipCoord.y] = '*';
    }
}

// Function to check if a hit is successful
int checkHit(Game* game, Coordinate attack, int isPlayer, int* shipNumber) {
    if (isPlayer) {
        // Check if the player's attack hits an enemy ship.
        for (int i = 0; i < 5; i++) {
            if (game->enemyShips[i].cells.row == attack.row - 1 &&
                game->enemyShips[i].cells.y == attack.y) {
                // Check if the target ship cell has already been hit.
                if (game->enemyShips[i].isHit != 0) {
                    printf("You've already hit this coordinate");
                    return 1;  // Hit unsuccessful (cell already hit).
                }
                else {
                    // Mark the enemy ship as hit, update the board, and decrement ship count.
                    printf("Yes, you hit an enemy ship!\n");
                    game->enemyShips[i].isHit = 1;
                    game->enemyBoard[attack.row - 1][attack.y] = 'X';
                    *shipNumber -= 1;
                    return 1;  // Hit successful.
                }
            }
        }
        // Mark the player's board with 'O' for a missed shot.
        game->enemyBoard[attack.row - 1][attack.y] = 'O';
        return 0;  // Hit unsuccessful (missed shot).
    }
    else {
        // Check if the enemy's attack hits a player ship.
        for (int i = 0; i < 5; i++) {
            if (game->playerShips[i].cells.row == attack.row &&
                game->playerShips[i].cells.y == attack.y) {
                // Check if the target ship cell has already been hit by the enemy.
                if (game->playerShips[i].isHit == 1) {
                    printf("Bot's already hit this coordinate");
                    return 1;  // Hit unsuccessful (cell already hit by the enemy).
                }
                else {
                    // Mark the player ship as hit, update the board, and decrement ship count.
                    game->playerShips[i].isHit = 1;
                    game->playerBoard[attack.row - 1][attack.y] = 'X';
                    *shipNumber -= 1;
                    return 1;  // Hit successful.
                }
            }
        }
        // Mark the player's board with 'O' for a missed shot by the enemy.
        printf("The Bot missed (%d %d)\n", attack.row, attack.y);
        game->playerBoard[attack.row - 1][attack.y - 1] = 'O';
        return 0;  // Hit unsuccessful (missed shot by the enemy).
    }
}


// Function to list all saved games
void listAllSavedGames() {
    // Open the save file for reading. If it fails to open, print an error message and return.
    FILE* file = fopen("saves.csv", "r");
    if (!file) {
        printf("Error opening file for reading.\n");
        return;
    }

    // Define a buffer to store each line read from the file.
    char line[1024];

    // Read the file line by line.
    while (fgets(line, sizeof(line), file)) {
        int id;
        char playerName[50];
        char playerFieldString[BOARD_SIZE * BOARD_SIZE + 1];
        char computerFieldString[BOARD_SIZE * BOARD_SIZE + 1];

        // Parse the line to extract game ID, player name, and field data strings.
#pragma warning(suppress : 4996)
        sscanf(line, "%d,%49[^,],%64[^,],%64s", &id, playerName, playerFieldString, computerFieldString);

        printf("ID: %d, User: %s\n", id, playerName);
    }

    // Close the file.
    fclose(file);
}

// Function to list saved games for a specific username
void listAllSavedGamesByUsername(const char username[MAX_USERNAME_LENGTH]) {
    // Open the saves.csv file for reading.
    FILE* file = fopen("saves.csv", "r");
    if (!file) {
        printf("Error opening file for reading.\n");
        return;
    }

    // Display a header for the list of saved games.
    printf("\n\nList of saved games for user '%s':\n", username);

    char line[1024];
    // Read each line in the file to find saved games for the specified username.
    while (fgets(line, sizeof(line), file)) {
        int id;
        char playerName[50];
        char playerFieldString[BOARD_SIZE * BOARD_SIZE + 1];
        char computerFieldString[BOARD_SIZE * BOARD_SIZE + 1];

        // Parse the line to extract game information.
#pragma warning(suppress : 4996)
        if (sscanf(line, "%d,%49[^,],%64[^,],%64s", &id, playerName, playerFieldString, computerFieldString) == 4) {
            // Check if the username in the line matches the specified username.
            if (strcmp(playerName, username) == 0) {
                // Display the Game ID for saved games belonging to the specified user.
                printf("Game ID: %d\n", id);
            }
        }
    }

    // Close the file after processing.
    fclose(file);
}

// Function to check if a move is valid
//int isValidMove(Game* game, Coordinate move) {
  //  int row = move.row;
    //int col = move.y;

    // Check if the coordinates are within the valid range of the game board.
   // return row >= 1 && row < BOARD_SIZE+1 && col >= 1 && col < BOARD_SIZE+1 &&
        // Check if the cell on the player's game board does not contain 'X' or 'O'.
        //game->playerBoard[row][col] != 'X' && game->playerBoard[row][col] != 'O';
//}
