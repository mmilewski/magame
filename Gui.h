#ifndef __GUI_H_INCLUDED__
#define __GUI_H_INCLUDED__

#include "Aabb.h"
#include "Sprite.h"

class Gui;
typedef boost::shared_ptr<Gui> GuiPtr;

class Gui : public boost::enable_shared_from_this<Gui> {
public:
    virtual ~Gui() {}

    virtual void Start() = 0;
    virtual void Init() = 0;
    virtual void Draw() = 0;
    virtual void Update(double dt) = 0;

    virtual bool OnKeyDown(const SDLKey& /* key */) { return false; }
    virtual bool OnKeyUp(const SDLKey& /* key */)   { return false; }
    virtual bool OnMouseMove(double /* x */, double /* y */) { return false; }
    virtual bool OnMouseDown(Uint8 /* button */, double /* x */, double /* y */) { return false; }
};

#include "BasicMathTypes.h"


class GuiWidget;
typedef boost::shared_ptr<GuiWidget> GuiWidgetPtr;

class GuiWidget {
public:
    explicit GuiWidget(Position position, Size size, bool is_visible)
        : m_position(position),
          m_size(size),
          m_is_visible(is_visible) {
    }
    virtual ~GuiWidget() {}

    virtual void Draw() const = 0;
    virtual void Update(double dt) = 0;

    Position GetPosition()      const { return m_position; }
    Position GetSizedPosition() const { return Position(GetPosition().X() + GetSize().X(),
                                                        GetPosition().Y() + GetSize().Y()); }
    Size     GetSize()          const { return m_size; }
    bool     IsVisible()        const { return m_is_visible; }
    GuiWidget* MoveBy(const Vector2& vector)       { m_position += vector; return this; }
    GuiWidget* SetSize(const Vector2& vector)      { m_size = vector;      return this; }
    GuiWidget* SetPosition(const Vector2& vector)  { m_position = vector;  return this; }
    GuiWidget* SetVisible(bool is_visible)         { m_is_visible = is_visible; return this; }

    Aabb GetAabb() const {
        return Aabb(GetPosition().X(), GetPosition().Y(),
                    GetSizedPosition().X(), GetSizedPosition().Y());
    }
private:
    Position m_position;             // położenia (lewego dolnego narożnika) na ekranie (przestrzeń okna)
    Size m_size;                     // rozmiar (szerokość i wysokość) (przestrzeń okna)
    bool m_is_visible;               // czy kontrolka jest widoczna
};


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


class SpriteButton;
typedef boost::shared_ptr<SpriteButton> SpriteButtonPtr;

class SpriteButton : public GuiWidget {
public:
    explicit SpriteButton(SpritePtr sprite, Position position, Size size, BrushPtr brush)
        : GuiWidget(position, size, true),
          m_sprite(sprite),
          m_brush(brush)  {
        // nop
    }
    virtual void Draw() const      { m_sprite->DrawCurrentFrame(GetPosition(), GetSize()); }
    virtual void Update(double dt) { m_sprite->Update(dt); }

    SpritePtr GetSprite() const    { return m_sprite; }
    BrushPtr  GetBrush()  const    { return m_brush; }
private:
    SpritePtr m_sprite;
    BrushPtr m_brush;
};


class EditorGui;
typedef boost::shared_ptr<EditorGui> EditorGuiPtr;

class EditorGui : public Gui, public boost::enable_shared_from_this<EditorGui> {
public:
    explicit EditorGui();
    void Start();
    void Init();
    void Draw();
    void Update(double dt);

    bool OnMouseMove(double x, double y);
    bool OnMouseDown(Uint8 button, double x, double y);

    BrushPtr GetActiveBrush() const {
        return m_active_brush;
    }

private:
    typedef std::vector<SpriteButtonPtr> ButtonContrainer;
    ButtonContrainer m_widgets;
    SpriteButtonPtr m_hovered_button;
    BrushPtr m_active_brush;
};

#endif
