#pragma once
#include "util/fs.hpp"

/**
 * @file card.hpp
 * @brief Defines playing card related enums and the Card class representing a single playing card.
 */

 /**
  * @enum Suit
  * @brief Represents the suit of a playing card.
  */


enum class Suit : unsigned char {
    Hearts,  
    Diamonds,
    Clubs, 
    Spades 
};

/**
 * @enum Rank
 * @brief Represents the rank (value) of a playing card.
 */
enum class Rank : unsigned char {
    Ace = 1,
    Two,    
    Three,  
    Four,   
    Five,  
    Six,     
    Seven,  
    Eight,  
    Nine,   
    Ten,    
    Jack,   
    Queen,   
    King     
};

/**
 * @class Card
 * @brief Represents a playing card with a suit, rank, and facing state.
 */
class Card {
public:
    /**
     * @brief Default constructor creates an invalid card.
     */
    Card();

    /**
     * @brief Constructs a card with the given suit and rank, facing down by default.
     * @param suit The suit of the card.
     * @param rank The rank of the card.
     */
    Card(Suit suit, Rank rank);

    /**
     * @brief Checks if the card is currently facing up.
     * @return True if the card is face up, false otherwise.
     */
    bool isFacingUp() const;

    /**
     * @brief Flips the card: face down becomes face up, and vice versa.
     */
    void flip();

    /**
     * @brief Gets the suit of the card.
     * @return The card's suit.
     */
    Suit getSuit() const;

    /**
     * @brief Gets the rank of the card.
     * @return The card's rank.
     */
    Rank getRank() const;

    /**
     * @brief Checks if the card's suit is red (Hearts or Diamonds).
     * @return True if the suit is red, false otherwise.
     */
    bool isRed() const;

    /**
     * @brief Checks if the card is valid (properly initialized).
     * @return True if the card is valid, false otherwise.
     */
    bool isValid() const;

    /**
    * @brief Writes card into buffered writer
    * @param writer Reference to writer 
    */
    void writeCard(BufferedIO::BufferedFileWriter& writer);

    /**
    * @brief Writes card into buffered reader
    * @param writer Reference to reader
    */
    void readCard(BufferedIO::BufferedFileReader& reader);

private:
    Suit suit;      
    Rank rank;      
    bool facingUp;  
    bool valid;   
};
