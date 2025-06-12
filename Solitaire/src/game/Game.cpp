#include "Game.hpp"
#include "util/fs.hpp"

Game::Game() : deck(), currentCard()  {}

void Game::reset() {
    deck = Deck();
    currentCard = Card();
    for (int i = 0; i < columnsSize;i++) {
        columns[i].clear();
    }
    for (int i = 0; i < reserveSlotSize; i++) {
        reserveSlots[i] = Card();
    }
    pile.clear();
    start();
}

void Game::start() {
    for (int i = 0; i < columnsSize; i++) {
        for (int j = 0; j < i + 1; j++) {
            columns[i].push_back(deck.drawCard());
        }
        int size = columns[i].size();
        columns[i][size - 1].flip();
    }
}
bool Game::drawCard() {
    if (deck.isEmpty()) {
        return false;
    }

    currentCard = deck.drawCard();
    currentCard.flip();
    pile.push_back(currentCard);
    return true;
}

std::vector<Card>& Game::getColumn(int index)
{
    ASSERT(index < columnsSize && index >= 0);

    return columns[index];
}


Card& Game::getCurrentCard() {
    return currentCard;
}

Card& Game::getReserveSlot(int index)
{
    ASSERT(index < 4 && index >= 0);
    return reserveSlots[index];
}

void Game::setReserveSlot(int index, Card& card)
{
    ASSERT(index < 4 && index >= 0);
    reserveSlots[index] = card;
}

std::vector<Card>& Game::getPile()
{
    return pile;
}


bool Game::isDeckEmpty() const {
    return deck.isEmpty();
}

bool Game::moveCard(int fromCol, int toCol, int count) {
    ASSERT(fromCol < columnsSize && fromCol >= 0);
    ASSERT(toCol < columnsSize && toCol >= 0);

    auto& from = columns[fromCol];
    auto& to = columns[toCol];

    if (count <= 0 || count > from.size())
        return false;

    Card& startCard = from[from.size() - count];
    if (!startCard.isFacingUp())
        return false;

    if (to.empty()) {
        if (startCard.getRank() != Rank::King)
            return false;
    }
    else {
        Card& top = to.back();
        if (top.isRed() == startCard.isRed() || static_cast<int>(top.getRank()) != static_cast<int>(startCard.getRank()) + 1)
            return false;
    }

    std::vector<Card> movingCards(from.end() - count, from.end());
    to.insert(to.end(), movingCards.begin(), movingCards.end());
    from.erase(from.end() - count, from.end());

    if (!from.empty() && !from.back().isFacingUp())
        from.back().flip();

    return true;
}

bool Game::moveFromPileToColumn(int toCol) {
    ASSERT(toCol < columnsSize && toCol >= 0);

    if (pile.empty())
        return false;

    Card card = pile.back();
    auto& to = columns[toCol];

    if (to.empty()) {
        if (card.getRank() != Rank::King)
            return false;
    }
    else {
        Card& top = to.back();
        if (top.isRed() == card.isRed() || static_cast<int>(top.getRank()) != static_cast<int>(card.getRank()) + 1)
            return false;
    }

    to.push_back(card);
    pile.pop_back();
    return true;
}

bool Game::moveFromPileToReserve(int slot) {
    ASSERT(slot < reserveSlotSize && slot >= 0);

    if (pile.empty())
        return false;

    Card card = pile.back();

    bool cardValid = reserveSlots[slot].isValid();
    if ((!cardValid && card.getRank() == Rank::Ace && static_cast<int>(card.getSuit()) == slot) || (cardValid && card.getRank() > reserveSlots[slot].getRank()) && static_cast<int>(card.getSuit()) == slot) {
        reserveSlots[slot] = card;
        pile.pop_back();
        return true;
    }

    return false;
}

bool Game::moveFromColumnToReserve(int fromCol, int slot) {
    ASSERT(fromCol < columnsSize && fromCol >= 0);
    ASSERT(slot < reserveSlotSize && slot >= 0);

    auto& column = columns[fromCol];
    if (column.empty())
        return false;

    Card& card = column.back();
    if (!card.isFacingUp())
        return false;

    bool cardValid = reserveSlots[slot].isValid();
    if ((!cardValid && card.getRank() == Rank::Ace && static_cast<int>(card.getSuit()) == slot) || (cardValid && card.getRank() > reserveSlots[slot].getRank()) && static_cast<int>(card.getSuit()) == slot) {
        reserveSlots[slot] = card;
        column.pop_back();

        if (!column.empty() && !column.back().isFacingUp())
            column.back().flip();

        return true;
    }

    return false;
}

bool Game::moveFromReserveToColumn(int slot, int toCol) {
    ASSERT(slot < reserveSlotSize && slot >= 0);
    ASSERT(toCol < columnsSize && slot >= 0);

    auto& column = columns[toCol];
    Card card = reserveSlots[slot];

    if (column.empty()) {
        if (card.getRank() != Rank::King)
            return false;
    }
    else {
        Card& top = column.back();
        if (top.isRed() == card.isRed() || static_cast<int>(top.getRank()) != static_cast<int>(card.getRank()) + 1)
            return false;
    }

    column.push_back(card);
    if (card.getRank() != Rank::Ace) {
        reserveSlots[slot] = Card(card.getSuit(), static_cast<Rank>(static_cast<int>(card.getRank()) - 1));
        reserveSlots[slot].flip();
    }
    else {
        reserveSlots[slot] = Card();
    }
    return true;
}

bool Game::isGameWon()
{
    int cnt = 0;
    for (int i = 0; i < 7; i++) {
        auto& column = columns[i];
        if (column.size() != 13) continue;

        bool columnSet = true;
        for (int j = 0; j < 13; j++) {
            Rank expected = static_cast<Rank>(13 - j);
            if (column[j].getRank() != expected || !column[j].isFacingUp()) {
                columnSet = false;
                break;
            }

            if (j > 0 && column[j].isRed() == column[j - 1].isRed()) {
                columnSet = false;
                break;
            }
        }

        if (columnSet) cnt++;
    }

    return cnt == 4;
}
bool Game::saveFileGame(std::string name) {
    BufferedIO::BufferedFileWriter writer(name + ".sot");
    if (!writer.isOpen()) return false;
    const char* magicNumber = "Solitaire";
    writer.write(magicNumber, 9);
    std::vector<Card>& deckCards = deck.getCards();

    writer.writeInt(deckCards.size());
    for (Card& card : deckCards) {
        card.writeCard(writer);
    }
    
    for (int i = 0; i < columnsSize; i++) {
        std::vector<Card>& cards = columns[i];
        writer.writeInt(cards.size());
        for (Card& card : cards) {
            card.writeCard(writer);
        }
    }
    
    writer.writeInt(pile.size());
    for (Card& card : pile) {
        card.writeCard(writer);
    }

    for (int i = 0; i < reserveSlotSize; i++) {
        reserveSlots[i].writeCard(writer);
    }
    writer.flush();
}
bool Game::readFileGame(std::string name)
{
    BufferedIO::BufferedFileReader reader(name + ".sot");
    if (!reader.isOpen()) return false;
    const char* magicNumber = "Solitaire";

    char verify[9];
    reader.read(verify, 9);

    if (strncmp(magicNumber, verify, 9) != 0) return false; // does not check null terminator

    int deckSize = reader.readInt();
    std::vector<Card> deckCards(deckSize);
    for (int i = 0; i < deckSize; i++) {
        Card card;
        card.readCard(reader);
        deckCards.push_back(card);
    }
    deck.setCards(deckCards);


    for (int i = 0; i < columnsSize; i++) {
        int size = reader.readInt();
        std::vector<Card> cards(size);
        for (Card& card : cards) {
            card.readCard(reader);
        }
        columns[i] = cards;
    }

    int pileSize = reader.readInt();
    std::cout << pileSize << "\n";
    std::vector<Card> pileCards(pileSize);
    for (int i = 0; i < pileSize; i++) {
        pileCards[i].readCard(reader);
    }
    pile = pileCards;

    for (int i = 0; i < 4; i++) {
        reserveSlots[i].readCard(reader);
    }

    return true;
}
void Game::test()
{
    columns[0].clear();
   // columns[4].clear();
    columns[5].clear();
    columns[6].clear();
    while (!deck.isEmpty()) deck.drawCard();
    std::vector<Card> cards;
    cards.push_back(Card(Suit::Hearts, Rank::Ace));
    cards.push_back(Card(Suit::Hearts, Rank::Ace));
    cards.push_back(Card(Suit::Spades, Rank::Ace));
    cards.push_back(Card(Suit::Spades, Rank::Ace));
    for (Card& card : cards) {
        card.flip();
    }
    deck.reShuffle(cards);
    for (int i = 1; i < 5; i++) {
        columns[i].clear();
        int suit = i % 2 == 0 ? 1 : 2;
        for (int j = 13; j > 0; j--) {
            Card card  = Card(suit % 2 == 0 ? Suit::Hearts : Suit::Spades, static_cast<Rank>(j));
            card.flip();
            if (j == 1) continue;
            columns[i].push_back(card);

            suit++;
        }
    }
}



