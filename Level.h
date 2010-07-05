#ifndef __LEVEL_H__
#define __LEVEL_H__
#include "StdAfx.h"

#include "Aabb.h"
#include "Types.h"

// Dane dotyczące jednostki wczytane np. z pliku. Na ich podstawie
// zostanie w grze stworzona odpowiednia jednostka
struct LevelEntityData {
    LevelEntityData()
        :  name("[unknown]"), x(-1), y(-1) {   }

    LevelEntityData(const std::string& name, double x, double y)
        :  name(name), x(x), y(y) {   }

    bool operator() (const LevelEntityData& a, const LevelEntityData& b) {
        return a.x < b.x;
    }

    std::string name;  // nazwa jednostki
    double x;          // położenie na osi odciętych
    double y;          // położenie na osi rzędnych
};


class Level {
public:
    explicit Level();

    void LoadFromFile(const std::string& filename);
    void LoadEntitiesFromFile(const std::string& filename);
    std::list<LevelEntityData> GetAllEntitiesToCreate() const  { return m_entities_to_create; }
    LevelEntityData GetPlayerData() const { return m_player_data; }

    FT::FieldType Field(size_t x, size_t y) const;
    size_t GetWidth() const   { return m_width; }
    size_t GetHeight() const  { return m_height; }
    Aabb GetFieldAabb(size_t x, size_t y) const;
    bool IsFieldCollidable(int x, int y) const;
    bool IsFieldNotCollidable(int x, int y) const;
    
    bool GetLoaded() const {
        return m_loaded;
    }

    
private:
    size_t m_width;
    size_t m_height;
    std::vector<std::vector<FT::FieldType> > m_data;
    std::list<LevelEntityData> m_entities_to_create;
    LevelEntityData m_player_data;
    bool m_loaded;

};
typedef boost::shared_ptr<Level> LevelPtr;

#endif
