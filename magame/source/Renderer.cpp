#include "StdAfx.h"

#include "Renderer.h"

void Renderer::SetTileSize(double width, double height) {
    m_tile_width = width;
    m_tile_height = height;
}

void Renderer::LoadTexture(const std::string & filename) {
    std::cout << "Ładowanie obrazka z pliku " + filename + "\n";

    // załaduj z pliku
    SDL_Surface* surface = SDL_LoadBMP(filename.c_str());
    if (!surface) {
        std::cerr << "Ładowanie pliku " + filename + " FAILED: " + SDL_GetError() + "\n";
        exit(1);
    }

    // sprawdź wymiary - czy są potęgą 2
    const int width = surface->w;
    const int height = surface->h;
    if (((width & (width - 1)) != 0) || ((height & (height - 1)) != 0)) {
        std::cerr << "Obrazek " + filename + " ma nieprawidłowe wymiary (powinny być potęgą 2): " << width << "x"
                << height << "\n";
        exit(1);
    }

    GLenum format;
    switch (surface->format->BytesPerPixel) {
    case 3:
        format = GL_BGR;
        break;
    case 4:
        format = GL_BGRA;
        break;
    default:
        std::cerr << "Nieznany format pliku " + filename + "\n";
        exit(1);
    }

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, surface->format->BytesPerPixel, width, height,
                 0, format, GL_UNSIGNED_BYTE, surface->pixels);
    if (surface) {
        SDL_FreeSurface(surface);
    }

}


void Renderer::DrawSprite(double tex_x, double tex_y, double tex_w, double tex_h,
                          double pos_x, double pos_y, double width, double height, 
                          DL::DisplayLayer layer) {
    const double texture_w = 1024.0;
    const double texture_h = 1024.0;

    const double left = tex_x / texture_w;
    const double right = left + tex_w / texture_w;
    const double bottom = tex_y / texture_h;
    const double top = bottom - tex_h / texture_h;
    /*
     * Obrazek ładowany jest do góry nogami, więc punkt (0,0) jest w lewym górnym rogu.
     * Stąd wynika, że w powyższym wzorze top jest poniżej bottom
     */

    glColor3f(1, 1, 1);
    glPushMatrix();
    {
        glTranslated(0, 0, -static_cast<double> (layer));
        glBegin(GL_QUADS);
        {
            glTexCoord2d(right, top);    glVertex2d(pos_x + width, pos_y + height);
            glTexCoord2d(left, top);     glVertex2d(pos_x, pos_y + height);
            glTexCoord2d(left, bottom);  glVertex2d(pos_x, pos_y);
            glTexCoord2d(right, bottom); glVertex2d(pos_x + width, pos_y);
        }
        glEnd();
    }
    glPopMatrix();
}

void Renderer::DrawAabb(const Aabb& box, double r, double g, double b, double a) const {
    const double tw = m_tile_width;
    const double th = m_tile_height;

    DrawQuad(box.GetMinX()*tw, box.GetMinY()*tw,
             box.GetMaxX()*th, box.GetMaxY()*th,
             r, g, b, a);
}

void Renderer::DrawQuad(double min_x, double min_y,
                        double max_x, double max_y,
                        double r, double g, double b, double a) const {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4d(r, g, b, a);
    glBegin(GL_QUADS);
    {
        glVertex2d(min_x, min_y);
        glVertex2d(max_x, min_y);
        glVertex2d(max_x, max_y);
        glVertex2d(min_x, max_y);
    }
    glEnd();

    glPopAttrib();
}

void Renderer::SetProjection(size_t width, size_t height) {
    glViewport(0, 0, static_cast<GLsizei> (width), static_cast<GLsizei> (height));
    ResetProjection();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Renderer::ResetProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1, 0, 1, -1, 10);
}
