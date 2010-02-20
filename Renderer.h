#ifndef RENDERER_H_
#define RENDERER_H_

#include <SDL/SDL_opengl.h>
#include <boost/shared_ptr.hpp>
#include <string>
#include "Aabb.h"
#include "Types.h"

class Renderer {
public:
    Renderer() 
        : m_tile_width(0.05),
          m_tile_height(0.05) {    
    }

    double GetTileWidth() const { return m_tile_width; }
    double GetTileHeight() const { return m_tile_height; }
    size_t GetHorizontalTilesOnScreenCount() const { return 1.0 / m_tile_width  + 0.5; }
    size_t GetVerticalTilesOnScreenCount()   const { return 1.0 / m_tile_height + 0.5; }

    void SetTileSize(double width, double height);

    void LoadTexture(const std::string & filename);
    void DrawSprite(double tex_x, double tex_y, double tex_w, double tex_h, double pos_x, double pos_y,
                    double width, double height, DL::DisplayLayer layer);

    void DrawAabb(const Aabb& box, double r=1, double g=1, double b=0, double a=.7) const;
    void DrawQuad(double min_x, double min_y,
                  double max_x, double max_y,
                  double r, double g, double b, double a) const;
    void SetProjection(size_t width, size_t height);
    void ResetProjection();

private:
    GLuint m_texture;
    double m_tile_width;
    double m_tile_height;
};

typedef boost::shared_ptr<Renderer> RendererPtr;

#endif /* RENDERER_H_ */
