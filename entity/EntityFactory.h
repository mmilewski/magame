#ifndef __ENTITY_FACTORY_H__
#define __ENTITY_FACTORY_H__
#include "StdAfx.h"

class Entity;
class LevelEntityData;
typedef boost::shared_ptr<Entity> EntityPtr;

class EntityFactory {
public:
    explicit EntityFactory();
    EntityPtr CreateEntity(ET::EntityType type, double x, double y);
    EntityPtr CreateEntity(const std::string& name, double x, double y);
    EntityPtr CreateEntity(const LevelEntityData& entity_data);
};

typedef boost::shared_ptr<EntityFactory> EntityFactoryPtr;

#endif
