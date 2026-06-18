#pragma once

#include "Terrain.h"

class Tile {
private:
    Terrain::Type terrain_type;

public:
    explicit Tile(Terrain::Type type = Terrain::Type::Grass)
        : terrain_type(type) {}

    Terrain::Type terrain() const {
        return terrain_type;
    }

    char render_char() const {
        return Terrain::toString(terrain_type)[0];
    }
};