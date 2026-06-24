#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Tile.h"
#include "MapObject.h"
// ---------------------------------------------------------------------------
// Map — pure in-memory representation of a loaded map.
//
// Holds tiles and placed objects. Knows nothing about laoding from
// file. Building a Map from a file is MapLoader's job.
//
// Construction:
//   Map is built empty via the constructor (just dimensions), then filled
//   in by MapLoader via set_tile() and addMapObject(). This makes Map
//   trivially testable without ever touching a file.
// ---------------------------------------------------------------------------
class Map {
private:
    std::vector<Tile> tiles;
    std::vector<std::unique_ptr<MapObject>> map_objects;

    int width{0};
    int height{0};

    std::size_t index(int x, int y) const;

public:
    Map() = delete;
    Map(int width, int height);

    int getHeight() const { return height; }
    int getWidth() const { return width; }

    Tile& tile_at(int x, int y);
    const Tile& tile_at(int x, int y) const;
    void set_tile(int x, int y, Terrain::Type terrain, std::string typeName);

    // Takes ownership of a fully-constructed MapObject (built by MapLoader).
    void addMapObject(std::unique_ptr<MapObject> obj);

    const std::vector<std::unique_ptr<MapObject>>& getMapObjects() const {
        return map_objects;
    }

    // void render() const;
};