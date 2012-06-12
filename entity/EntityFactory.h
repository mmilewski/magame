#ifndef MAGAME_ENTITY_FACTORY_H_INCLUDED
#define MAGAME_ENTITY_FACTORY_H_INCLUDED
#include "StdAfx.h"

class Entity;
class LevelEntityData;
typedef boost::shared_ptr<Entity> EntityPtr;

class EntityFactory {
public:
    explicit EntityFactory();
    EntityPtr CreateEntity(ET::EntityType type, double x, double y, Vector2 direction = Vector2::ZERO);
    EntityPtr CreateEntity(const LevelEntityData& entity_data);
};

typedef boost::shared_ptr<EntityFactory> EntityFactoryPtr;

#endif /* MAGAME_ENTITY_FACTORY_H_INCLUDED */
