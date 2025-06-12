#pragma once
#include "util/common.hpp"
#include "Card.hpp"
#include <algorithm>
#include <random>
#include <vector>

/**
 * @class Deck
 * @brief Represents a standard deck of 52 playing cards with shuffle and draw functionality.
 */
class Deck {
public:
    /**
     * @brief Constructs a new Deck and initializes it with 52 cards, shuffled.
     */
    Deck();

    /**
     * @brief Shuffles the deck randomly.
     */
    void shuffle();

    /**
    * @brief Takes pile of card puts into deck and shuffles it.
    * @param pile Pile of cards
    */
    void reShuffle(std::vector<Card>& pile);
    /**
     * @brief Resets the deck to a full 52-card set and shuffles it.
     */
    void reset();

    /**
     * @brief Draws the top card from the deck.
     * @return The drawn Card object.
     */
    Card drawCard();

    /**
     * @brief Checks if the deck is empty.
     * @return true if there are no cards left, false otherwise.
     */
    inline bool isEmpty() const {
        return cards.size() == 0;
    }

    inline std::vector<Card>& getCards() {
        return cards;
    }

    inline void setCards(std::vector<Card> cards) {
        this->cards = cards;
    }

private:
    /// Container holding the cards in the deck.
    std::vector<Card> cards;
};
