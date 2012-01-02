#ifndef __CREATOR_PRODUCER_H_INCLUDED__
#define __CREATOR_PRODUCER_H_INCLUDED__
#include "StdAfx.h"

#include "Creator.h"

// Klasa bazowa dla klas, które będą dodawać kreatory
class CreatorProducer {
public:
    void AddCreator(CreatorPtr creator) {
        m_creators.push_back(creator);
    }

    void DropAllCreators() {
        m_creators.clear();
    }

    std::list<CreatorPtr> GetCreators() const {
        return m_creators;
    }

private:
    std::list<CreatorPtr> m_creators;
};

#endif
