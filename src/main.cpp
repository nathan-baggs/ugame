#include <iostream>
#include <print>

#include "game/game.h"
#include "utils/error.h"
#include "utils/exception.h"
#include "utils/log.h"

auto main(int argc, char **argv) -> int
{
    game::log::info("starting game");

    try
    {
        game::ensure(argc == 2, "./game.exe <root_path>");

        auto g = game::Game{};
        g.run(argv[1]);
    }
    catch (const game::Exception &err)
    {
        std::println(std::cerr, "{}", err);
    }
    catch (...)
    {
        std::println(std::cerr, "unknown exception");
    }

    return 0;
}
