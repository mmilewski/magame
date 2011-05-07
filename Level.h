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

    bool operator== (const LevelEntityData& other) const {
       return name == other.name
           && abs(x-other.x) < 0.001
           && abs(y-other.y) < 0.001;
    }

    std::string name;  // nazwa jednostki
    double x;          // położenie na osi odciętych
    double y;          // położenie na osi rzędnych
};


class Level;
typedef boost::shared_ptr<Level> LevelPtr;

class Level {
public:
    explicit Level();
    explicit Level(LevelPtr level, const std::list<LevelEntityData>& entities_data, const LevelEntityData& player_data);

    void SaveFieldsToFile(const std::string& filename);
    void SaveEntitiesToFile(const std::string& filename);

    void LoadFromFile(const std::string& filename);
    void LoadEntitiesFromFile(const std::string& filename);

    std::list<LevelEntityData> GetAllEntitiesToCreate() const  { return m_entities_to_create; }
    LevelEntityData            GetPlayerData()          const  { return m_player_data; }

    FT::FieldType Field(size_t x, size_t y) const;
    void SetField(size_t x, size_t y, FT::FieldType ft);

    // Zwiększa (nigdy nie zmniejsza) szerokość poziomu do zadanej.
    void EnsureWidth(size_t width);

    // Szerokość, do której zostanie przycięty poziom.
    // width=0 oznacza "za ostatnim klockiem". Nie zwiększa szerokości poziomu.
    void ShrinkWidth(size_t width = 0);

    size_t GetWidth() const   { return m_width; }
    size_t GetHeight() const  { return m_height; }
    Aabb GetFieldAabb(size_t x, size_t y) const;
    bool IsFieldCollidable(int x, int y) const;
    bool IsFieldNotCollidable(int x, int y) const;

    bool        GetLoaded() const { return m_loaded; }
    std::string GetName()   const { return m_name; }

private:
    std::string m_name;
    size_t m_width;
    size_t m_height;
    std::vector<std::vector<FT::FieldType> > m_data;
    std::list<LevelEntityData> m_entities_to_create;
    LevelEntityData m_player_data;
    bool m_loaded;
};

#endif
