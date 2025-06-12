#include "ConsoleUi.hpp"
#include <locale>
#include <iostream>
#include <vector>
#include <string>
#include "../util/MultiLineWStringBuilder.hpp"
#include "../util/colorUtil.hpp"
#ifdef _WIN32 
#include "../util/windowsConsole.hpp"
#endif


/// Black text on white background.
static const std::wstring BLACK_FG_WHITE_BG = ColorUtil::wrgbBoth({ 0,0,0 }, { 245,247,250 });
/// Red text on white background.
static const std::wstring RED_FG_WHITE_BG = ColorUtil::wrgbBoth({ 255,0,0 }, { 255,255,255 });
/// White text on green background.
static const std::wstring WHITE_FG_GREEN_BG = ColorUtil::wrgbBoth({ 245,247,250 }, { 52,162,73 });
/// Black text on green background.
static const std::wstring BLACK_FG_GREEN_BG = ColorUtil::wrgbBoth({ 0,0,0 }, { 52,162,73 });
/// White text on white background.
static const std::wstring WHITE_FG_WHITE_BG = ColorUtil::wrgbBoth({ 245,247,250 }, { 245,247,250 });
/// White text on dark green background.
static const std::wstring WHITE_FG_DARK_GREEN_BG = ColorUtil::wrgbBoth({ 245,247,250 }, { 31, 97, 44 });
/// Red text on dark green background.
static const std::wstring RED_FG_DARK_GREEN_BG = ColorUtil::wrgbBoth({ 255,0,0 }, { 31, 97, 44 });
/// Black text on dark green background.
static const std::wstring BLACK_FG_DARK_GREEN_BG = ColorUtil::wrgbBoth({ 0,0,0 }, { 31, 97, 44 });
/// White text on lighter dark green background.
static const std::wstring WHITE_FG_LIGHTER_DARK_GREEN_BG = ColorUtil::wrgbBoth({ 255,255,255 }, { 42, 130, 58 });


ConsoleUi::ConsoleUi(Game& game) : game(game) {}

/// @brief Converts a rank enum to its string representation.
/// @param rank The card rank.
/// @return A string representing the rank (e.g., "A", "2", ..., "K").
static std::wstring rankToString(Rank rank) {
    switch (rank) {
    case Rank::Ace:   return L"A";
    case Rank::Two:   return L"2";
    case Rank::Three: return L"3";
    case Rank::Four:  return L"4";
    case Rank::Five:  return L"5";
    case Rank::Six:   return L"6";
    case Rank::Seven: return L"7";
    case Rank::Eight: return L"8";
    case Rank::Nine:  return L"9";
    case Rank::Ten:   return L"10";
    case Rank::Jack:  return L"J";
    case Rank::Queen: return L"Q";
    case Rank::King:  return L"K";
    }
    return L"?";
}

/// @brief Converts a suit enum to its Unicode symbol.
/// @param suit The card suit.
/// @return A wide string containing the suit symbol.
static std::wstring suitToString(Suit suit) {
    switch (suit) {
    case Suit::Hearts:   return L"\u2665";  // ♥
    case Suit::Diamonds: return L"\u2666";  // ♦
    case Suit::Clubs:    return L"\u2663";  // ♣
    case Suit::Spades:   return L"\u2660";  // ♠
    }
    return L"?";
}

/**
 * @brief Generates an ASCII representation of a card as a vector of strings.
 * @param card The card to be drawn.
 * @return Vector of wide strings representing the ASCII box.
 */
static std::vector<std::wstring> cardToAsciiBox(const Card& card) {
    std::vector<std::wstring> lines(5);

    if (!card.isFacingUp()) {
        lines[0] = BLACK_FG_WHITE_BG + L"┌───────┐";
        lines[1] = BLACK_FG_WHITE_BG + L"│░░░░░░░│";
        lines[2] = BLACK_FG_WHITE_BG + L"│░░░░░░░│";
        lines[3] = BLACK_FG_WHITE_BG + L"│░░░░░░░│";    
        lines[4] = BLACK_FG_WHITE_BG + L"└───────┘";
        return lines;
    }

    std::wstring rank = rankToString(card.getRank());
    std::wstring suit = suitToString(card.getSuit());

    lines[0] = BLACK_FG_WHITE_BG + L"┌───────┐";
    std::wstring color = card.isRed() ? RED_FG_WHITE_BG : BLACK_FG_WHITE_BG;
    lines[1] = BLACK_FG_WHITE_BG + L"│" + color + rank + BLACK_FG_WHITE_BG + std::wstring(7 - rank.size(), ' ') + L"│";


    lines[2] = BLACK_FG_WHITE_BG + L"│" + std::wstring(3, ' ') + color + suit + BLACK_FG_WHITE_BG + std::wstring(3, ' ') + L"│";
    lines[3] = BLACK_FG_WHITE_BG + L"│" + std::wstring(7 - rank.size(), ' ') + color + rank + BLACK_FG_WHITE_BG + L"│";
    lines[4] = BLACK_FG_WHITE_BG + L"└───────┘";

    return lines;
}

void ConsoleUi::draw() {
    int pileYOffset = 1;
    MultiLineWStringBuilder builder(BLACK_FG_GREEN_BG);
    builder.set(2, pileYOffset++, BLACK_FG_WHITE_BG + L"╔═══════╗");
    builder.set(2, pileYOffset++, BLACK_FG_WHITE_BG + L"║ / / / ║");
    builder.set(2, pileYOffset++, BLACK_FG_WHITE_BG + L"║/ / / /║");
    builder.set(2, pileYOffset++, BLACK_FG_WHITE_BG + L"║ / / / ║");
    builder.set(2, pileYOffset++, BLACK_FG_WHITE_BG + L"╚═══════╝");
    pileYOffset++;

    std::vector<Card> pile = game.getPile();

    for (int i = 0; i < pile.size(); i++) {
        std::vector<std::wstring> cardLines = cardToAsciiBox(pile[i]);
        if (i < pile.size() - 1) {
            cardLines.pop_back();
            cardLines.pop_back();
        }
        for (int j = 0; j < cardLines.size(); j++) {
            builder.set(2, pileYOffset++, cardLines[j]);
        }
    }

    int xOffset = 20;
    for (int i = 0; i < game.columnsSize; i++) {
        int yOffset = 1;
        // column number
        builder.set(xOffset + 4, 0, WHITE_FG_GREEN_BG + std::to_wstring(i + 1));

        std::vector<Card>& column = game.getColumn(i);
        for (int j = 0; j < column.size(); j++) {
            std::vector<std::wstring> cardLines = cardToAsciiBox(column[j]);

            if (j < column.size() - 1) {
                cardLines.pop_back();
                cardLines.pop_back();
            }

            for (int k = 0; k < cardLines.size(); k++) {
                builder.set(xOffset, yOffset + k, cardLines[k]);
            }
            if (j < column.size() - 1) {
                yOffset += 3;
            }
            else {
                yOffset += 5;
            }

        }
        xOffset += 10;
    }
    xOffset += 9;

    int reserveYOffset = 1;

    // top lighter outline
    builder.set(xOffset - 1, reserveYOffset, WHITE_FG_LIGHTER_DARK_GREEN_BG + L"             ");
    reserveYOffset++;
    for (int i = 0; i < game.reserveSlotSize; i++) {

        std::wstring suitColor = i < 2 ? RED_FG_WHITE_BG : BLACK_FG_WHITE_BG;


        //top
        builder.set(xOffset, reserveYOffset, WHITE_FG_DARK_GREEN_BG + L"           ");

        //bottom
        builder.set(xOffset, reserveYOffset + 6, WHITE_FG_DARK_GREEN_BG + L"           ");


        // left  top corner lighter outline
        builder.set(xOffset - 1, reserveYOffset, WHITE_FG_LIGHTER_DARK_GREEN_BG + L" ");
        
         //right top corner lighter outline
        builder.set(xOffset + 11, reserveYOffset, WHITE_FG_LIGHTER_DARK_GREEN_BG + L" ");

        // left bottom corner lighter outline
        builder.set(xOffset - 1, reserveYOffset + 6, WHITE_FG_LIGHTER_DARK_GREEN_BG + L" ");

        //right bottom corner lighter outline
        builder.set(xOffset + 11, reserveYOffset + 6, WHITE_FG_LIGHTER_DARK_GREEN_BG + L" ");

        // slot number
        builder.set(xOffset + 13, reserveYOffset + 3, WHITE_FG_GREEN_BG + std::to_wstring(i + 1));

        for (int j = 0; j < 5; j++) {

            // left lighter outline
            builder.set(xOffset - 1, reserveYOffset + j + 1, WHITE_FG_LIGHTER_DARK_GREEN_BG + L" ");

            //right lighter outline
            builder.set(xOffset + 11, reserveYOffset + j + 1, WHITE_FG_LIGHTER_DARK_GREEN_BG + L" ");
            
            // left darker outline
            builder.set(xOffset, reserveYOffset + j + 1, WHITE_FG_DARK_GREEN_BG + L" ");
           
            // right darker outline
            builder.set(xOffset + 10, reserveYOffset + j + 1, WHITE_FG_DARK_GREEN_BG + L" ");
        }



        if (!game.getReserveSlot(i).isValid()) {
            builder.set(xOffset + 1, reserveYOffset + 1, BLACK_FG_WHITE_BG + L"┌───────┐");
            builder.set(xOffset + 1, reserveYOffset + 2, BLACK_FG_WHITE_BG + L"│   " + suitColor + suitToString(static_cast<Suit>(i)) + BLACK_FG_WHITE_BG + L"   │");
            builder.set(xOffset + 1, reserveYOffset + 3, BLACK_FG_WHITE_BG + L"│       │");
            builder.set(xOffset + 1, reserveYOffset + 4, BLACK_FG_WHITE_BG + L"│       │");
            builder.set(xOffset + 1, reserveYOffset + 5, BLACK_FG_WHITE_BG + L"└───────┘");
        }
        else {
            std::vector<std::wstring> lines = cardToAsciiBox(game.getReserveSlot(i));
            for (int j = 0; j < lines.size(); j++) {
                builder.set(xOffset + 1, reserveYOffset + 1 + j, BLACK_FG_WHITE_BG + lines[j]);
            }
        }
        reserveYOffset += 6;
    }
    builder.set(xOffset - 1, ++reserveYOffset, WHITE_FG_LIGHTER_DARK_GREEN_BG + L"             ");  

#ifdef _WIN32
    WindowsConsole::WriteWStringToConsole(builder.str());
#else   
    std::wcout << builder.str();
#endif
    
}



/**
 * @brief Splits a string by a specified separator character.
 *
 * This function takes a string and splits it into substrings wherever the specified separator occurs.
 *
 * @param s The input string to split.
 * @param seperator The character to use as a delimiter.
 * @return A vector of substrings obtained by splitting the input string.
 */
std::vector<std::string> Split(const std::string& s, char seperator)
{
    std::vector<std::string> output;

    std::string::size_type prev_pos = 0, pos = 0;

    while ((pos = s.find(seperator, pos)) != std::string::npos)
    {
        std::string substring(s.substr(prev_pos, pos - prev_pos));

        output.push_back(substring);

        prev_pos = ++pos;
    }

    output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word

    return output;
}

std::string ConsoleUi::handleCommand(std::string command) {
    std::vector<std::string> splitted = Split(command, ' ');

    std::string invalidColumn = "Niepoprawna kolumna";

    switch (hash(splitted[0])) {
        case hash("d"):
        case hash("dobierz"): {
            bool success = game.drawCard();
            return success ? "Dobrano karte" : "Nie można dobrać karty przetasuj używając komendy \"przetasuj\"";
            break;
        }
        case hash("p"):
        case hash("przenies"): {
            std::string invalidArguments = "Niepoprawne argumenty, oczekiwano przenies [od nr kolumny] [do nr kolumny] [ilosc kart]";
            if (splitted.size() != 4) return invalidArguments;
            int from, to, count;
            try {
                from = std::stoi(splitted[1]);
                to = std::stoi(splitted[2]);
                count = std::stoi(splitted[3]);
            } catch (...) {
                return invalidArguments;
            }

            if (from > 7 || to > 7 || from < 1 || to < 1) {
                return invalidColumn;
            }
            from--;
            to--;

            bool success = game.moveCard(from, to, count);
            if (success) {
                return count == 1
                    ? "Pomyslnie przeniesiono karte"
                    : "Pomyslnie przeniesiono karty";
            }
            else {
                return count == 1
                    ? "Nie mozna przeniesc karty"
                    : "Nie mozna przeniesc kart";
            }
        }
        case hash("pk"):
        case hash("z_puli_do_kolumny"): {
            std::string invalidArguments = "Niepoprawne argumenty, oczekiwano z_puli_do_kolumny [nr_kolumny]";
            if (splitted.size() != 2) return invalidArguments;
            int to;
            try {
                to = std::stoi(splitted[1]);
            } catch (...) {
                return invalidArguments;
            }
            if (to > 7 || to < 1) return "Niepoprawna kolumna";
            to--;

            bool success = game.moveFromPileToColumn(to);
            return success ? "Pomyslnie przeniesiono karte" : "Nie mozna przeniesc karty";
        }
        case hash("pr"):
        case hash("z_puli_do_rezerwy"): {
            std::string invalidArguments = "Niepoprawne argumenty, oczekiwano z_puli_do_rezerwy [nr_rezerwy]";
            if (splitted.size() != 2) return invalidArguments;
            int to;
            try {
                to = std::stoi(splitted[1]);
            }
            catch (...) {
                return invalidArguments;
            }
            if (to > 4 || to < 1) return "niepoprawny numer kolumny ";
            to--;

            bool success = game.moveFromPileToReserve(to);
            return success ? "Pomyslnie przeniesiono karte" : "Nie mozna przeniesc karty";
        }
        case hash("kr"):
        case hash("z_kolumny_do_rezerwy"): {
            std::string invalidArguments = "Niepoprawne argumenty, oczekiwano z_kolumny_do_rezerwy [nr_kolumny] [nr_rezerwy]";
            if (splitted.size() != 3) return invalidArguments;

            int from, to;
            try {
                from = std::stoi(splitted[1]);
                to = std::stoi(splitted[2]);
              
            }
            catch (...) {
                return invalidArguments;
            }
            if (to > 4 || to < 1) return "Niepoprawna kolumna";
            if (from > 7 || to < 1) return "Niepoprawna kolumna";
            to--;
            from--;

            bool success = game.moveFromColumnToReserve(from, to);
            return success ? "Pomyslnie przeniesiono karte" : "Nie mozna przeniesc karty";
        }
        case hash("rk"):
        case hash("z_rezerwy_do_kolumny"): {
            std::string invalidArguments = "Niepoprawne argumenty, oczekiwano z_rezerwy_do_kolumny [nr_rezerwy] [nr_kolumny]";
            if (splitted.size() != 3) return invalidArguments;

            int from, to;
            try {
                from = std::stoi(splitted[1]);
                to = std::stoi(splitted[2]);

            }
            catch (...) {
                return invalidArguments;
            }
            if (from > 4 || from < 1) return "Niepoprawna rezerwa";
            if (to > 7 || to < 1) return "Niepoprawna kolumna";
            to--;
            from--;

            bool success = game.moveFromReserveToColumn(from, to);
            return success ? "Pomyslnie przeniesiono karte" : "Nie mozna przeniesc karty";
        }
        case hash("przetasuj"): {
            if (!game.getDeck().isEmpty()) return "Nie można przetasować, na stosie są karty użyj \"dobierz\" aby dobrać karte";
            game.getDeck().reShuffle(game.getPile());
            return "Przetasowano";
        }
        case hash("reset"): {
            game.reset();
            return "Zresetowano gre";
        }
        case hash("wyjdz"): {
            running = false;
            return "Wychodzenie..";
        }
        case hash("menu"): {
            drawMenu();
            return "";
        }
        case hash("zapisz"): {
            std::string invalidArguments = "Niepoprawne argumenty, oczekiwano zapisz [nazwa zapisu]";
            if (splitted.size() != 2) return invalidArguments;
            if (!BufferedIO::isValidFilename(splitted[1])) return "Niedozwolone znaki w nazwie zapisu uzyj alfanumerycznych znakow";
            bool success = game.saveFileGame(splitted[1]);
            return success ? "Zapisano plik" : "Wystapil blad w zapisywaniu pliku";
        }
        case hash("pomoc"): {
            return "Dostepne komendy\n"
                "wyjdz - wychodzi z gry\n"
                "reset - resetuje gre\n"
                "przetasuj - przetasowuje karty gdy wszystkie karty zostały dobrane\n"
                "dobierz,d - dobiera karte\n"
                "przenies,p [od nr kolumny] [do nr kolumny] [ilosc kart] - przenosi podana ilosc kart z kolumny do kolumny\n"
                "z_puli_do_kolumny,pk [nr_kolumny] - przenosi dobrana karte do kolumny \n"
                "z_puli_do_rezerwy,pr [nr_rezerwy] - przenosi dobrana karte do rezerwy\n"
                "z_kolumny_do_rezerwy,kr [nr_kolumny] [nr_rezerwy] - przenosi karte z kolumny dorezerwy\n"
                "z_rezerwy_do_kolumny,rk [nr_rezerwy] [nr_kolumny] - przenosi karte z rezerwy do kolumny\n"
                "menu - wychodzi do glownego menu\n"
                "zapisz [nazwa zapisu] - zapisuje gre\n"
                "pomoc - wyswietla wszystkie komendy";
        }
    }
    return "Nie znaleziono komendy";
}
#ifdef _WIN32
CRITICAL_SECTION draw_cs;

DWORD WINAPI ResizeWatcher(LPVOID lpParam) {
    ConsoleUi* stdUi = reinterpret_cast<ConsoleUi*>(lpParam);

    while (stdUi->running) {
        if (stdUi->inMainMenu) {
            Sleep(100);
            continue;
        }
        EnterCriticalSection(&draw_cs);
        if (WindowsConsole::hasResized()) {
            WindowsConsole::clear();
            stdUi->draw();
            std::cout << "wpisz komende aby zagrać jeżeli nie znasz komend wpisz \"pomoc\"\n";
            std::cout << "komenda: ";
            std::cout << stdUi->inputBuffer;
        }
        LeaveCriticalSection(&draw_cs);
        Sleep(100);
    }

    return 0;
}
#endif



void ConsoleUi::drawMenu() {
    inMainMenu = true;
    int baseTick = 0;
    WindowsConsole::WriteWStringToConsole(ColorUtil::RESET);
    WindowsConsole::clear();
    std::vector<std::wstring> menuText = {
        L" __        __                  __  ",
        L"|__)  /\\  /__`    |  /\\  |\\ | /__` ",
        L"|    /~~\\ .__/ \\__/ /~~\\ | \\| .__/ ",
        L"",
        L"Autor: Arkadiusz Winiarski",
        L"Wybierz opcje: ",
        L"1.Nowa gra",
        L"2.Załaduj gre"
    };
    bool latestFound = BufferedIO::fileExists("latest.sot");
    if (latestFound) {
        menuText.push_back(L"3.Załaduj ostatnią gre");
    }
    menuText.push_back(L"Napisz wyjdz aby wyjsc z programu");
    float saturation = 0.9f;
    bool pulseUp = false;
    std::string inputBuffer;
    std::wstring resultMessage = L"";

    int selectedNum = -1;
    bool shouldRun = true;
    while (shouldRun) {
        if (selectedNum != -1 && selectedNum != 2) break;
        if (saturation > 0.9) pulseUp = false;
        if (saturation < 0.5) pulseUp = true;
        if (pulseUp) saturation += 0.01;
        else saturation -= 0.01;
        
        if (baseTick > 100) baseTick = 0; // avoid overflowing issues
        MultiLineWStringBuilder builder;

        for (int row = 0; row < menuText.size(); ++row) {
            std::wstring line;
            const std::wstring& logoLine = menuText[row];
            for (int col = 0; col < logoLine.size(); ++col) {
                int rgbTick = baseTick + col * 2;
                ColorUtil::RGB rgbColor = ColorUtil::getRainbowColor(rgbTick, 50,saturation);
                std::wstring colorCode = ColorUtil::wrgbToForeground(rgbColor);
                line += colorCode + logoLine[col];
            }
            builder.set(0, row, line);
        }

        WindowsConsole::setCursorPosition(0, 0);
        WindowsConsole::WriteWStringToConsole(builder.str());
        WindowsConsole::WriteWStringToConsole(resultMessage.c_str());
        if (selectedNum == 2) {
            std::cout << "podaj nazwe zapisu lub wpisz \"wyjdz\" aby wybrać inną opcję : ";
        }
        std::cout << inputBuffer;

		bool selectionMade = WindowsConsole::processConsoleInput(inputBuffer, false);
		if (selectedNum == 2 && selectionMade) {
            std::string buffer = inputBuffer;
            inputBuffer = "";
            WindowsConsole::clear();

			if (buffer == "wyjdz") {
				selectedNum = -1;
                resultMessage = L"";
			}
			else {
				if (!BufferedIO::isValidFilename(buffer + ".sot")) {
					resultMessage = L"Nazwa pliku zawiera niedozwolone znaki\n";
				}
				else {
					if (!BufferedIO::fileExists(buffer + ".sot")) {
						resultMessage = L"Plik nie istnieje\n";
					}
					else {
						bool readed = game.readFileGame(buffer);
						if (!readed) {
                            resultMessage = L"Wystąpił błąd w czytaniu pliku\n";
						}
						else {
							shouldRun = false;
						}
					}
				}
			}

        }
        else if (selectionMade && inputBuffer == "wyjdz") {
            inputBuffer = "";
            WindowsConsole::clear();
            running = false;
            break;
        } else if (selectionMade) {

            std::string buffer = inputBuffer;
            inputBuffer = "";
            WindowsConsole::clear();
            int num;
            try {
                num = std::stoi(buffer);
            }
            catch (...) {
                resultMessage = L"Nie została wpisana liczba\n";
                continue;
            }
            if (num < 1 || num > (latestFound ? 3 : 2)) {
                resultMessage = L"Została wpisana nieprawidłowa liczba\n";
                continue;
            } 
            selectedNum = num;
            if (selectedNum == 3) {
                game.readFileGame("latest");
            }
            else if (selectedNum == 1) {
                game.reset();
            }
        }
        
        baseTick += 1;
        Sleep(50);
    }
    WindowsConsole::WriteWStringToConsole(BLACK_FG_GREEN_BG);
    inMainMenu = false;
}
void ConsoleUi::start() {
  //  game.test();
#ifdef _WIN32
    WindowsConsole::enable24BitColors();
#endif
    try {
        std::locale::global(std::locale("en_US.UTF-8"));
    }
    catch (const std::runtime_error&) {
        std::locale::global(std::locale(""));
    }
    std::cout.imbue(std::locale());

#ifdef _WIN32
    drawMenu();
#else
    if (BufferedIO::fileExists("latest.sot")) {
        std::cout << "zapis ostatniej gry zostal znaleziony czy chcesz go zaladowac? Tak/Nie: ";
        std::string response = "";
        std::cin >> response;

        std::transform(response.begin(), response.end(), response.begin(),
            [](unsigned char c) { return std::tolower(c); });
        if (response == "tak") {
            bool code = game.readFileGame("latest");
            std::cout << "load code " << code;
            system("pause");
        }
    }
#endif

#ifdef _WIN32
    InitializeCriticalSection(&draw_cs);
    HANDLE resizeThread = CreateThread(nullptr, 0, ResizeWatcher, this, 0, nullptr);
#endif
   

    std::string commandResult = "wpisz komende aby zagrać jeżeli nie znasz komend wpisz \"pomoc\"";

    draw(); // draws whole background after second use of draw
    while(running) {
        draw:
#ifdef _WIN32
        EnterCriticalSection(&draw_cs);
        WindowsConsole::clear();
#elif
        system("clear");
#endif

        draw();


#ifdef _WIN32 
        LeaveCriticalSection(&draw_cs);
#endif
        if (game.isGameWon()) {
            std::cout << "Gra została wygrana czy chcesz rozpoczac nowa? Tak/Nie: ";
            std::string response = "";
            std::cin >> response;

            std::transform(response.begin(), response.end(), response.begin(),
                [](unsigned char c) { return std::tolower(c); });

            if (response == "tak") {
                game.reset();
                goto draw;
            } else break;

        }

        std::cout << commandResult << "\n";
        std::cout << "komenda : ";
        std::string input;
#ifdef _WIN32
        input = WindowsConsole::getLine(true,&inputBuffer);
#else
        std::getline(std::cin, input);
#endif
        commandResult = handleCommand(input);
        game.saveFileGame("latest");
    }

    running = false;
    std::wcout << ColorUtil::RESET;
#ifdef _WIN32
    WaitForSingleObject(resizeThread, INFINITE);
    DeleteCriticalSection(&draw_cs);
    CloseHandle(resizeThread);
    WindowsConsole::restoreConsole();
    WindowsConsole::clear();
#else 
    system("clear");
#endif
}
