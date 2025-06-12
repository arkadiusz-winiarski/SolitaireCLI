#include "game/Game.hpp"
#include "game/util/assert.hpp"
#include "game/util/allocator.hpp"
#include "game/ui/ConsoleUi.hpp"
#include <stdio.h>
#include <windows.h>

int main() {
#if defined(_DEBUG) && defined(_WIN32)
	Allocator::initialize();
#endif

	Game game;
	game.start();

	ConsoleUi consoleUi(game);
	consoleUi.start();

	return 0;
}
 