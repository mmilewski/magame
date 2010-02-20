#ifndef __ENTITY_FACTORY_H__
#define __ENTITY_FACTORY_H__

#include <boost/shared_ptr.hpp>

class Entity;
typedef boost::shared_ptr<Entity> EntityPtr;

class EntityFactory {
public:
    explicit EntityFactory();
    EntityPtr CreateEntity(ET::EntityType type, double x, double y);
    EntityPtr CreateEntity(const std::string& name, double x, double y);
};

typedef boost::shared_ptr<EntityFactory> EntityFactoryPtr;

#endif
