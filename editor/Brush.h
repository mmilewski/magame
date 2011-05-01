#ifndef __BRUSH_H_INCLUDED__
#define __BRUSH_H_INCLUDED__

#include "../StdAfx.h"
#include "EditorCommand.h"
#include "../Sprite.h"
#include "../Engine.h"


class Brush;
typedef boost::shared_ptr<Brush> BrushPtr;

class Brush {
public:
    struct ST {
        enum SpecialType { UNKNOWN, Player, Eraser, Multi };
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
    FT::FieldType   GetFieldType() const   { return IsField() ? m_field_type : FT::None; }
    ET::EntityType  GetEntityType() const  { return IsEntity() ? m_entity_type : ET::UNKNOWN; }
    ST::SpecialType GetSpecialType() const { return IsSpecial() ? m_special_type : ST::UNKNOWN; }

    SpritePtr GetSprite() const { return m_sprite; }

    virtual void Draw(Position scr_position, Size scr_size) {
        DrawSketch(scr_position, scr_size);
        DrawIcon(scr_position, scr_size);
    }

    virtual EditorCommandPtr GetCommand() { return EditorCommandPtr(); }

protected:
    // Metoda do przesłonięcia w klasach pochodnych.
    // Rysowanie szkicu/podglądu.
    virtual void DrawSketch(Position scr_position, Size scr_size) {
    }

    // Rysuje małego kafelka obok kursora.
    void DrawIcon(Position scr_position, Size scr_size) {
        GetSprite()->DrawCurrentFrame(scr_position, scr_size);
    }

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

class MultiBrush : public Brush {
public:
    explicit MultiBrush(SpritePtr sprite) : Brush(sprite, Brush::ST::Multi), m_is_active(false) {
        // nop
    }

    static MultiBrushPtr New(SpritePtr sprite) {
        return MultiBrushPtr(new MultiBrush(sprite));
    }

    virtual void DrawSketch(Position scr_position, Size scr_size);

    void StartAt(double x, double y) {
        m_startx = m_endx = x;
        m_starty = m_endy = y;
        m_is_active = true;
    }

    void MoveTo(double x, double y) {
        if (IsActive()) {
            m_endx = x;
            m_endy = y;
        }
    }

    void FinishAt(double x, double y) {
        if (IsActive()) {
            m_endx = x;
            m_endy = y;
            m_is_active = false;
        }
    }

    virtual EditorCommandPtr GetCommand() { return PlatformEditorCommandPtr(new PlatformEditorCommand); }

private:
    Position GetStart() { return Position(m_startx, m_starty); }
    Position GetEnd() { return Position(m_endx, m_endy); }
    bool IsActive() { return m_is_active; }

    double m_startx, m_starty;
    double m_endx, m_endy;
    bool m_is_active;
};

#endif
