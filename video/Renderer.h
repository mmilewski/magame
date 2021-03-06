#ifndef MAGAME_RENDERER_H_INCLUDED
#define MAGAME_RENDERER_H_INCLUDED
#include "StdAfx.h"

#include "math/BasicMathTypes.h"
#include "math/Aabb.h"
#include "Types.h"
#include "Rgba.h"

class Renderer {
public:
    explicit Renderer() 
        : m_texture(0xCAFE),
          m_tile_width(0.05),
          m_tile_height(0.05) {    
    }

    double GetTileWidth() const { return m_tile_width; }
    double GetTileHeight() const { return m_tile_height; }
    Size GetTileSize() const { return Size(GetTileWidth(), GetTileHeight()); }

    size_t GetHorizontalTilesOnScreenCount() const { return static_cast<size_t>(1.0 / m_tile_width  + 0.5); }
    size_t GetVerticalTilesOnScreenCount()   const { return static_cast<size_t>(1.0 / m_tile_height + 0.5); }

    void SetTileSize(double width, double height);

    void LoadTexture(const std::string & filename);
    void DrawSprite(double tex_x, double tex_y, double tex_w, double tex_h,
                    double pos_x, double pos_y, double width, double height,
                    DL::DisplayLayer layer, double brightness = 1.0);

    void DrawAabb(const Aabb& box, Rgba color=Rgba(1,1,0,.7)) const;
    void DrawQuad(double min_x, double min_y,
                  double max_x, double max_y,
                  Rgba color) const;
    void DrawQuad(Position min_position, Position max_position, Rgba color) const;
    void DrawQuadSized(Position min_position, Size size, Rgba color) const;

    void SetProjection(size_t width, size_t height);
    void ResetProjection();

private:
    GLuint m_texture;
    double m_tile_width;
    double m_tile_height;
};

typedef boost::shared_ptr<Renderer> RendererPtr;

#endif /* MAGAME_RENDERER_H_INCLUDED */
