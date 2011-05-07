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

    virtual void Draw(Position scr_position, Size scr_size) const {
        DrawSketch(scr_position, scr_size);
        DrawIcon(scr_position, scr_size);
    }

    virtual EditorCommandPtr GetCommand() const { return EditorCommandPtr(); }

protected:
    // Metoda do przesłonięcia w klasach pochodnych.
    // Rysowanie szkicu/podglądu.
    virtual void DrawSketch(Position /* scr_position */, Size /* scr_size */) const {
    }

    // Rysuje mały kafelek obok kursora.
    void DrawIcon(Position scr_position, Size scr_size) const {
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
    explicit MultiBrush(SpritePtr sprite)
        : Brush(sprite, Brush::ST::Multi),
          m_start(-1, -1),
          m_end(-1, -1),
          m_is_active(false) {
        // nop
    }

    static MultiBrushPtr New(SpritePtr sprite) { return MultiBrushPtr(new MultiBrush(sprite)); }

    virtual void DrawSketch(Position scr_position, Size scr_size) const;

    void StartAt(const Position& pos) { m_start = m_end = pos; m_is_active = true; }
    void StartAt(double x, double y)  { StartAt(Position(x, y)); }

    void MoveTo(const Position& pos) {
        if (IsActive()) {
            m_end = pos;
        }
    }
    void MoveTo(double x, double y)  { MoveTo(Position(x, y)); }

    void FinishAt(const Position& pos) {
        if (IsActive()) {
            m_end = pos;
            m_is_active = false;
        }
    }
    void FinishAt(double x, double y)  { FinishAt(Position(x, y)); }

    virtual EditorCommandPtr GetCommand() const {
        if (IsActive()) {
            std::cerr << "UWAGA: Pobranie akcji z aktywnego pędzla multibrush. "
                      << "Akcja jest gotowa dopiero, gdy pędzel jest nieaktywny";
        }
        return PlatformEditorCommandPtr(new PlatformEditorCommand(GetStart(), GetEnd()));
    }

private:
    Position GetStart() const { return m_start; }
    Position GetEnd() const { return m_end; }
    bool IsActive() const { return m_is_active; }

    Position m_start, m_end;
    bool m_is_active;
};

#endif
