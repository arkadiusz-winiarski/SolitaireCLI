#pragma once

#include "Deck.hpp"
#include "Card.hpp"
#include "util/common.hpp"
#include <vector>

/**
 * @file Game.hpp
 * @brief Declaration of the Game class for managing Solitaire game logic, including card columns, piles, and reserve slots.
 */

/**
 * @class Game
 * @brief Manages the state and rules of a solitaire-like card game.
 */
class Game {
public:
    static const int columnsSize = 7;        ///< Number of columns in the tableau.
    static const int reserveSlotSize = 4;    ///< Number of reserve slots.

    /**
     * @brief Constructs a new Game object and initializes the deck and columns.
     */
    Game();

    /**
     * @brief Starts a new game by dealing cards to columns.
     */
    void start();

    /**
     * @brief Resets the game, shuffling the deck and clearing all piles.
     */
    void reset();

    /**
     * @brief Draws a card from the deck to the pile.
     * @return True if card can be drawn, false if deck is empty.
     */
    bool drawCard();

    /**
     * @brief Gets a reference to a column of cards.
     * @param index Column index [0, columnsSize).
     * @return Reference to vector of cards in that column.
     */
    std::vector<Card>& getColumn(int index);

    /**
     * @brief Gets the current drawn card from the deck.
     * @return Reference to the current card.
     */
    Card& getCurrentCard();

    /**
     * @brief Gets a reserve slot card by index.
     * @param index Reserve slot index [0, reserveSlotSize).
     * @return Reference to the card in the reserve slot.
     */
    Card& getReserveSlot(int index);

    /**
     * @brief Sets a reserve slot card by index.
     * @param index Reserve slot index.
     * @param card Card to place in reserve slot.
     */
    void setReserveSlot(int index, Card& card);

    /**
     * @brief Gets the pile of drawn cards.
     * @return Reference to vector of cards in the pile.
     */
    std::vector<Card>& getPile();

    /**
     * @brief Checks if the deck is empty
     * @return True if deck is empty, false otherwise.
     */
    bool isDeckEmpty() const;

    /**
     * @brief Moves count cards from one column to another if valid.
     * @param fromCol Source column index.
     * @param toCol Destination column index.
     * @param count Number of cards to move.
     * @return True if move succeeded, false otherwise.
     */
    bool moveCard(int fromCol, int toCol, int count);

    /**
     * @brief Moves top card from pile to a column if rules allow.
     * @param toCol Destination column index.
     * @return True if move succeeded, false otherwise.
     */
    bool moveFromPileToColumn(int toCol);

    /**
     * @brief Moves top card from pile to a reserve slot if allowed.
     * @param slot Reserve slot index.
     * @return True if move succeeded, false otherwise.
     */
    bool moveFromPileToReserve(int slot);

    /**
     * @brief Moves top card from a column to a reserve slot if allowed.
     * @param fromCol Source column index.
     * @param slot Reserve slot index.
     * @return True if move succeeded, false otherwise.
     */
    bool moveFromColumnToReserve(int fromCol, int slot);

    /**
     * @brief Moves a card from a reserve slot back to a column if allowed.
     * @param slot Reserve slot index.
     * @param toCol Destination column index.
     * @return True if move succeeded, false otherwise.
     */
    bool moveFromReserveToColumn(int slot, int toCol);

    bool isGameWon();

    bool saveFileGame(std::string name);

    bool readFileGame(std::string name);

    inline Deck& getDeck() {
        return deck;
    }

    /**
   * @brief Sets up 4 slots of cards K-2 and puts 4 Aces into deck for development purpose
   */
    void test();

private:
    Deck deck;                                   ///< Deck of cards.
    Card currentCard;                            ///< Currently drawn card.
    std::vector<Card> columns[columnsSize];     ///< Tableau columns.
    std::vector<Card> pile;                      ///< Discard pile.
    Card reserveSlots[reserveSlotSize];          ///< Reserve slots.
};
