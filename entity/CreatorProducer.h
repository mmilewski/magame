#ifndef MAGAME_CREATOR_PRODUCER_H_INCLUDED
#define MAGAME_CREATOR_PRODUCER_H_INCLUDED
#include "StdAfx.h"

#include "Creator.h"

// Klasa bazowa dla klas, które będą dodawać kreatory
class CreatorProducer {
public:
    virtual ~CreatorProducer() {}

    void AddCreator(CreatorPtr creator) {
        m_creators.push_back(creator);
    }

    void DropAllCreators() {
        m_creators.clear();
    }

    std::list<CreatorPtr> GetCreators() const {
        return m_creators;
    }

    void MoveCreatorsTo(std::list<CreatorPtr>& target) {
        target.splice(target.end(), m_creators);
    }

private:
    std::list<CreatorPtr> m_creators;
};

#endif
