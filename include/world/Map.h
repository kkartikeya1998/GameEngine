#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Tile.h"
#include "MapObject.h"

class Map {
private:
    std::vector<Tile> tiles;
    std::vector<std::unique_ptr<MapObject>> map_objects;

    int width{0};
    int height{0};

    std::size_t index(int x, int y) const;

public:
    Map() = delete;
    explicit Map(const std::string& path);

    void load_from_file(const std::string& path);
    void render() const;

    int getHeight() const { return height; }
    int getWidth() const { return width; }

    Tile& tile_at(int x, int y);
    const Tile& tile_at(int x, int y) const;
    void set_tile(int x, int y, Terrain::Type terrain);
};