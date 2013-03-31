#ifndef MAGAME_BRUSH_H_INCLUDED
#define MAGAME_BRUSH_H_INCLUDED

#include "StdAfx.h"
#include "video/Sprite.h"
#include "EditorCommand.h"


class Brush;
typedef boost::shared_ptr<Brush> BrushPtr;

class Brush {
public:
    Brush(SpritePtr sprite)
        : m_sprite(sprite),
          m_start(-1, -1),
          m_end(-1, -1),
          m_is_active(false) {}


    struct ST {
        enum SpecialType { UNKNOWN, Player, Eraser, Multi };
    };

    explicit Brush(SpritePtr sprite, ST::SpecialType st)
        : m_sprite(sprite),
          m_is_special(true),
          m_special_type(st),

          m_start(-1, -1),
          m_end(-1, -1),
          m_is_active(false)
        {
    }

    virtual ~Brush() {}

    bool IsSpecial() const { return m_is_special; }
    ST::SpecialType GetSpecialType() const { return IsSpecial() ? m_special_type : ST::UNKNOWN; }

    SpritePtr GetSprite() const { return m_sprite; }

    virtual void Draw(Position scr_position, Size scr_size) const {
        DrawSketch(scr_position, scr_size);
        DrawIcon(scr_position, scr_size);
    }

    virtual EditorCommandPtr GetCommand() const { return EditorCommandPtr(); }

    virtual bool SnapsToGrid() { return false; }

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

    bool m_is_special;
    ST::SpecialType m_special_type;

public:
    virtual void StartAt(const Position& pos) { m_start = m_end = pos; m_is_active = true; }
    void StartAt(double x, double y)  { StartAt(Position(x, y)); }

    virtual void MoveTo(const Position& pos) {
        if (IsActive()) {
            m_end = pos;
        }
    }
    void MoveTo(double x, double y)  { MoveTo(Position(x, y)); }

    virtual void FinishAt(const Position& pos) {
        if (IsActive()) {
            m_end = pos;
            m_is_active = false;
        }
    }
    void FinishAt(double x, double y)  { FinishAt(Position(x, y)); }

protected:
    Position GetStart() const { return m_start; }
    Position GetEnd() const { return m_end; }
    bool IsActive() const { return m_is_active; }
    Position m_start, m_end;
    bool m_is_active;

};

class AddEntityBrush : public Brush {
public:
    explicit AddEntityBrush(SpritePtr sprite, ET::EntityType type)
        : Brush(sprite),
          m_type(type) {
    }

    virtual EditorCommandPtr GetCommand() const {
        return EditorCommandPtr(new AddEntityCommand(GetEnd(), m_type));
    }

private:
    ET::EntityType m_type;
};

class SetFieldBrush : public Brush {
public:
    explicit SetFieldBrush(SpritePtr sprite, FT::FieldType type)
        : Brush(sprite),
          m_type(type) {
    }

    virtual EditorCommandPtr GetCommand() const {
        return EditorCommandPtr(new SetFieldCommand(GetEnd().CutToInt(), m_type));
    }

    virtual bool SnapsToGrid() { return true; }

private:
    FT::FieldType m_type;
};


class MultiBrush;
typedef boost::shared_ptr<MultiBrush> MultiBrushPtr;

class MultiBrush : public Brush {
public:
    explicit MultiBrush(SpritePtr sprite)
        : Brush(sprite, Brush::ST::Multi) {
        // nop
    }

    static MultiBrushPtr New(SpritePtr sprite) { return MultiBrushPtr(new MultiBrush(sprite)); }

    virtual void DrawSketch(Position scr_position, Size scr_size) const;

    virtual EditorCommandPtr GetCommand() const {
        if (IsActive()) {
            std::cerr << "UWAGA: Pobranie akcji z aktywnego pędzla multibrush. "
                      << "Akcja jest gotowa dopiero, gdy pędzel jest nieaktywny";
        }
        return PlatformEditorCommandPtr(new PlatformEditorCommand(GetStart(), GetEnd()));
    }

    virtual bool SnapsToGrid() { return true; }
};

#endif
