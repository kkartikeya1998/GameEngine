Now that we have the main domain elements and their minimal interactions required set up. This can act as the core game mechanics for further development. This system (world, entities, game controller) will have to be updated as the whole system requirements evolve.

#### Currently achieved:
	1. World representation and decomposition.
	2. Player class with basic move operations, unrelated to the map.
	3. Controller with instances of the above two.

### What we need:
```
THE GAME LOOP:
  1. Take input
  2. Process input, update state
  3. Render output
```
SFML library provides for a good option in input handling and rendering system. Let's break this down into more details.
### Input System
- for now a keyboard input
- which keys map to commands?
- how to feed input to next step?
### Core mechanics
- controller manages the states of world and player
- based on the input provided from last step, the controller updates the states as required
- game mechanics
- the world and entity classes should not be responsible for render logic, but only expose Data to render system
NOTE on the ownership of CapturedPokemon: should be given to the PokemonRepository, specifically PCStorage to mimic the behaviour from real game. The player cannot transfer/release pokemon from anywhere but the PCStorage.
### Renderer System
- decoupled from core classes
- each drawable entity should have render functionality