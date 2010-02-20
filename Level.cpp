#include <fstream>
#include <iostream> 
#include <sstream>   // istringstream
#include <algorithm>  // sort
#include "Engine.h"
#include "Level.h"


Level::Level()
    : m_width(0),
      m_height(0),
      m_loaded(false) {
}

void Level::LoadFromFile(const std::string& filename) {
    std::ifstream lvl(filename.c_str());
    if (!lvl) {
        std::cerr << "Nie udało się załadować pliku " << filename << "\n";
        m_loaded = false;
        return;
    }

    lvl >> m_width >> m_height;
    m_data.resize(m_height);
    for (size_t y = 0; y < m_height; ++y) {
        m_data.at(y).resize(m_width);
    }

    int tmp;
    for (size_t y = 0; y < m_height; ++y) {
        for (size_t x = 0; x < m_width; ++x) {
            lvl >> tmp;
            m_data.at(y).at(x) = FT::FieldType(tmp);
        }
    }

    m_loaded = true;
}


void Level::LoadEntitiesFromFile(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if (!file) {
        std::cerr << "Nie udało się załadować pliku " << filename << "\n";
        return;
    }

    // wczytaj linia po linii
    const int buffer_size = 1024;
    char buffer[buffer_size];
    while (file) {
        file.getline(buffer, buffer_size);
        std::string line(buffer);
        if (line.length() < 5 || line.at(0) == '#')
            continue;
        std::istringstream iss(line);
        LevelEntityData data;
        iss >> data.name;
        iss >> data.x;
        iss >> data.y;
        if (data.name == "player") {
            m_player_data = data;
        } else {
            m_entities_to_create.push_back(data);
        }

        std::cout << "[LoadEntityFromFile] " << data.name << ", " << data.x << ", " << data.y << std::endl;
    }

    // posortuj wczytane rekordy
    m_entities_to_create.sort(LevelEntityData());
}


FT::FieldType Level::Field(size_t x, size_t y) const {
    if (x >= m_width || y >= m_height) {
        return FT::None;
    }
    return m_data.at(y).at(x);
}


Aabb Level::GetFieldAabb(size_t x, size_t y) const {
//    const double tile_width = Engine::Get().Renderer()->GetTileWidth();
//    const double tile_height = Engine::Get().Renderer()->GetTileHeight();
    RendererPtr renderer = Engine::Get().Renderer();
    const size_t v_tiles_count = renderer->GetVerticalTilesOnScreenCount();
    y = v_tiles_count - y;  // odbij y w pionie (y=0 będzie na dole)

    Aabb box = Aabb(x, y-1, x + 1, y);
    return box;
}
