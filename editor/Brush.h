#ifndef __BRUSH_H_INCLUDED__
#define __BRUSH_H_INCLUDED__

class Brush;
typedef boost::shared_ptr<Brush> BrushPtr;

class Brush {
public:
    struct ST {
        enum SpecialType { UNKNOWN, Player, Eraser };
    };
    
    explicit Brush(SpritePtr sprite, FT::FieldType ft)
        : m_sprite(sprite),
          m_is_field(true),
          m_is_entity(false),
          m_is_special(false),
          m_field_type(ft)
        {
    }
    explicit Brush(SpritePtr sprite, ET::EntityType et)
        : m_sprite(sprite),
          m_is_field(false),
          m_is_entity(true),
          m_is_special(false),
          m_entity_type(et)
        {
    }
    explicit Brush(SpritePtr sprite, ST::SpecialType st)
        : m_sprite(sprite),
          m_is_field(false),
          m_is_entity(false),
          m_is_special(true),
          m_special_type(st)
        {
    }

    virtual ~Brush() {}

    static BrushPtr New(SpritePtr sprite, FT::FieldType ft) {
        return BrushPtr(new Brush(sprite, ft));
    }
    static BrushPtr New(SpritePtr sprite, ET::EntityType et) {
        return BrushPtr(new Brush(sprite, et));
    }
    static BrushPtr New(SpritePtr sprite, ST::SpecialType st) {
        return BrushPtr(new Brush(sprite, st));
    }

    bool IsField()   const { return m_is_field; }
    bool IsEntity()  const { return m_is_entity; }
    bool IsSpecial() const { return m_is_special; }

    // Zwraca typ pola/encji/specjalny.
    // Wartość jest poprawna, gdy IsField/IsEntity/IsSpecial()==true
    FT::FieldType GetFieldType() const { return m_field_type; }
    ET::EntityType GetEntityType() const { return m_entity_type; }
    ST::SpecialType GetSpecialType() const { return m_special_type; }

    SpritePtr GetSprite() const { return m_sprite; }
private:
    SpritePtr m_sprite;

    bool m_is_field;
    bool m_is_entity;
    bool m_is_special;

    FT::FieldType m_field_type;
    ET::EntityType m_entity_type;
    ST::SpecialType m_special_type;
};


class MultiBrush;
typedef boost::shared_ptr<MultiBrush> MultiBrushPtr;

class MultiBrush {
public:
    explicit MultiBrush(SpritePtr sprite, Position start_point)
        : m_sprite(sprite)
        {
    }

    static MultiBrushPtr New(SpritePtr sprite, Position start_point) {
        return MultiBrushPtr(new MultiBrush(sprite, start_point));
    }

    virtual void Draw(Position position, Size size) {
        std::cout << "MultiBrush::Draw()" << std::endl;
    }

protected:
    SpritePtr GetSprite() const { return m_sprite; }
private:
    SpritePtr m_sprite;
};

#endif
