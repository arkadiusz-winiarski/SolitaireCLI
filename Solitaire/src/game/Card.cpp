#include "Card.hpp"


Card::Card() : valid(false) {}
Card::Card(Suit suit, Rank rank)
    : suit(suit), rank(rank), facingUp(false), valid(true) {
}

bool Card::isFacingUp() const {
    return facingUp;
}

void Card::flip() {
    facingUp = !facingUp;
}

Suit Card::getSuit() const {
    return suit;
}

Rank Card::getRank() const {
    return rank;
}

bool Card::isRed() const {
    return suit == Suit::Hearts || suit == Suit::Diamonds;
}

bool Card::isValid() const {
    return valid;
}

void Card::writeCard(BufferedIO::BufferedFileWriter& writer) {
    writer.writeInt(static_cast<int>(suit));
    writer.writeInt(static_cast<int>(rank));
    writer.writeBool(facingUp);
    writer.writeBool(valid);
}

void Card::readCard(BufferedIO::BufferedFileReader& reader) {
    suit = static_cast<Suit>(reader.readInt());
    rank = static_cast<Rank>(reader.readInt());
    facingUp = reader.readBool();
    valid = reader.readBool();
}

