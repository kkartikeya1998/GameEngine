## Basic idea
```
A player can walk around on a map. A map can have items like buildings, trees, forests which can block the way or move player to another map, like inside a building or a cave. There also exist Pokemon creatures in this Universe. A Pokemon usually has a Wild encounter, which can be random spawns or interaction with a Pokemon entity on the map. If the Pokemon is captured, it can be saved in the team or in the PC. A player can have maximum 6 Pokemon in their team.
```
#### Functional Requirements:
- There is a Pokemon **World**
- This world has many **Map**s
- Each **Map** is made up of fixed number of **Tile**s which represent the basic unit of movement
- A **Tile** can be occupied by **NonBlockingObjects** such as grass, stairs, bridges, door. 
    - **Tile** can also be occupied by a **BlockingObject** such as tree, building or waterfall.   
    - **Tile** can also be a **TeleportPoint** which teleports the player to another **TeleportPoint**, on the same **Map** or another.
- The **Player** can walk around on a **Map** on **Walkable** tiles, and can interact with **Interactable** objects.
- The **Pokemon** object can be **Encounter**ed as a random event, or as a result of an interaction.
    - A **WildPokemon** is generated is random mostly, but can also be triggered by special events. They can be captured.
    - A **CapturedPokemon** is owned by a **Trainer** and can be actively used in their **Party** (max 6 Pokemon) or can be stored in the **PCStorage** (Lets say max 1000).
- Each **Pokemon** can be of a **PokemonSpecies**. 
- A **CapturedPokemon** is owned (i really dont like this wording but for the sake of good design) by a **Player**.
- A **Player** has a **Party** of **Pokemon**, an **Inventory** of **Item**s, and a state for tracking story progress.

#### Non-functional requirements:
- Each **Map** can have a terrain type such as grassy, ocean, cave, volcano... 
- For bigger **BlockingObject**s such as trees of 2 or 3 tiles or longer height, not all **Tile**s are blocked. The tiles apart from the base of the tree can be walkable.
- Some **Pokemon** can evolve into a certain **Pokemon** when certain conditions are met. 
- Each **Species** has some fixed base stats such as attack, sp. attack, defence, sp. defence, hp and speed at level 1. They can level upto 100, based on the amount of experience gained. The main stats of the Pokemon are derived from the base stats scaled linearly with level.

![[FirstDesign.svg]]  
- add Pokemon Stats. [done]
- add some Pokemon data (assets/pokemon/poke_species_gen1.h) [done]
- add PokemonFactory