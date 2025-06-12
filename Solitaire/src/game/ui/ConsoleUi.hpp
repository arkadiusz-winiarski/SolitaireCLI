#pragma once
#include "../Game.hpp"

/**
 * @file ConsoleUi.hpp
 * @brief Declares the ConsoleUi class which provides a console-based user interface for the Game.
 */

/**
 * @class ConsoleUi
 * @brief Provides a console-based user interface for displaying and interacting with the Game.
 */
class ConsoleUi {
public:
    /**
     * @brief Constructs a ConsoleUi with a reference to the game instance.
     * @param game Reference to the Game object to be rendered and interacted with.
     */
    ConsoleUi(Game& game);

    /**
     * @brief Starts the user interface loop (input and render cycle).
     */
    void start();

    /**
    * @brief Draws the current state of the game to the console.
    */
    void draw();

    /// bool for checking if game is running for windows resize console thread
    volatile bool running = true;
    /// bool for checking if game is displayed or main menu
    volatile bool inMainMenu = true;

    /**
    * @brief Draws main menu of a game
    */
    void drawMenu();

    /// Buffer for windows input method
    std::string inputBuffer;
private:
    /// Reference to the game instance.
    Game& game;

    /**
     * @brief Parses and executes a console command for the card game.
     *
     * Supported commands (case-sensitive, with aliases) and their syntax:
     *
     * - "dobierz", "d"
     *   Draws a card from the deck/pile.
     *   No arguments.
     *   Returns success or failure message.
     *
     * - "przenies", "p"
     *   Moves a specified number of cards from one column to another.
     *   Syntax: przenies [from_column] [to_column] [count]
     *   from_column and to_column: integers 1-7
     *   count: number of cards to move (integer)
     *   Validates argument count and ranges.
     *   Returns success or failure messages accordingly.
     *
     * - "z_puli_do_kolumny", "pk"
     *   Moves the top card from the draw pile to a specified column.
     *   Syntax: z_puli_do_kolumny [to_column]
     *   to_column: integer 1-7
     *   Validates argument count and range.
     *   Returns success or failure messages.
     *
     * - "z_puli_do_rezerwy", "pr"
     *   Moves the top card from the draw pile to a specified reserve slot.
     *   Syntax: z_puli_do_rezerwy [to_reserve]
     *   to_reserve: integer 1-4
     *   Validates argument count and range.
     *   Returns success or failure messages.
     *
     * - "z_kolumny_do_rezerwy", "kr"
     *   Moves a card from a specified column to a specified reserve slot.
     *   Syntax: z_kolumny_do_rezerwy [from_column] [to_reserve]
     *   from_column: integer 1-7
     *   to_reserve: integer 1-4
     *   Validates argument count and ranges.
     *   Returns success or failure messages.
     *
     * - "z_rezerwy_do_kolumny", "rk"
     *   Moves a card from a specified reserve slot to a specified column.
     *   Syntax: z_rezerwy_do_kolumny [from_reserve] [to_column]
     *   from_reserve: integer 1-4
     *   to_column: integer 1-7
     *   Validates argument count and ranges.
     *   Returns success or failure messages.
     *
     * - "przetasuj"
     *   Shuffles the deck if the draw pile is empty.
     *   No arguments.
     *   Returns success or failure message if shuffling not possible.
     *
     * - "reset"
     *   Resets the game to initial state.
     *   No arguments.
     *   Returns confirmation message.
     *
     * - "wyjdz"
     *   Exits the game loop by setting running to false.
     *   No arguments.
     *   Returns exit message.
     *
     * - "menu"
     *   Draws or shows the main menu.
     *   No arguments.
     *   Returns empty string.
     *
     * - "zapisz"
     *   Saves the current game state to a file.
     *   Syntax: zapisz [save_name]
     *   save_name: validated as a valid filename (alphanumeric).
     *   Returns success or failure messages.
     *
     * - "pomoc"
     *   Displays help with available commands and usage.
     *   No arguments.
     *   Returns help string.
     *
     * Validation:
     * - Checks number of arguments for each command.
     * - Validates numeric arguments and ranges.
     * - Returns descriptive error messages for invalid inputs.
     *
     * @param command A user-entered string command.
     * @return A string with the result message or error information.
     */
	std::string handleCommand(std::string command);

};
