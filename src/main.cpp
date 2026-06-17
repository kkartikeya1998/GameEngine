#include "entities/pokemon/Pokemon.h"
#include "entities/pokemon/WildPokemon.h"
#include "entities/pokemon/CapturedPokemon.h"

#include "entities/pokemon/PokemonFactory.h"
#include "entities/pokemon/PokemonSpeciesRepository.h"
#include "entities/pokemon/ISpeciesRepository.h"

#include <iostream>
#include <memory>
#include <stdexcept>

int main() {
    try {
        ISpeciesRepository& repository = PokemonSpeciesRepository::instance();

        WildPokemonFactory wild_factory(repository);
        CapturedPokemonFactory captured_factory(repository);

        // Create a wild Pokémon (Bulbasaur, level 5)
        std::cout << "Creating wild Bulbasaur...\n";
        std::unique_ptr<Pokemon> pokemon = std::unique_ptr<Pokemon>(
            wild_factory.create(3, 5)
        );

        // Try to capture it
        WildPokemon* wild = dynamic_cast<WildPokemon*>(pokemon.get());
        if (!wild) {
            std::cerr << "Failed: pokemon is not a WildPokemon.\n";
            return 1;
        }

        std::cout << "Attempting to capture " << repository.findById(wild->species_id())->name << "...\n";
        if (!wild->capture()) {
            std::cerr << "Capture failed!\n";
            return 1;
        }

        std::cout << "Capture successful! Creating CapturedPokemon...\n";

        auto captured = std::unique_ptr<CapturedPokemon>(
            new CapturedPokemon(
                wild->species_id(),
                wild->level(),
                "Bulby"
            )
        );

        pokemon = std::unique_ptr<Pokemon>(captured.release());

        CapturedPokemon* captured_pkmn = dynamic_cast<CapturedPokemon*>(pokemon.get());
        if (!captured_pkmn) {
            std::cerr << "Failed: pokemon is not a CapturedPokemon after capture.\n";
            return 1;
        }

        std::cout << "Successfully captured " << repository.findById(captured_pkmn->species_id())->name
                  << " (ID: " << captured_pkmn->species_id()
                  << ", Level: " << captured_pkmn->level()
                  << ", Nickname: " << captured_pkmn->nickname()
                  << ").\n";

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}

// int main() {
//     const std::string world_file = "assets/maps";

//     World world(world_file);

//     std::cout << "Loading maps: pallet_town_small, route1_small, viridian_forest_small\n";
//     world.load_map(0);

//     std::cout << "Rendering world...\n";
//     world.render();
//     world.load_map(1);

//     std::cout << "Rendering world...\n";
//     world.render();
//     world.load_map(2);

//     std::cout << "Rendering world...\n";
//     world.render();

//     return 0;
// }