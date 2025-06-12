#pragma once
#include "Deck.hpp"

Deck::Deck() {
    reset();  
}


void Deck::reset() {
	cards.clear();
	for (int suit = 0; suit < 4; ++suit) {
		for (int rank = 1; rank <= 13; ++rank) {
			cards.push_back(Card(static_cast<Suit>(suit), static_cast<Rank>(rank)));
		}
	}
	shuffle();
}

void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
}

Card Deck::drawCard() {
    ASSERT(!isEmpty());

    Card card = cards[cards.size() - 1];
    cards.pop_back();
    return card;
}

void Deck::reShuffle(std::vector<Card>& pile) {
    ASSERT(isEmpty());
    
    cards = pile;
    pile.clear();
    shuffle();
    for (Card& card : cards) {
        card.flip();
    }
}

