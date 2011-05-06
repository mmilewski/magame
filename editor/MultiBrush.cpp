#include "Brush.h"

Position MapPosWorldToWindow(const Position& pos) {
    const double tile_width  = Engine::Get().GetRenderer()->GetTileWidth();
    const double tile_height = Engine::Get().GetRenderer()->GetTileHeight();
    return Position(static_cast<int>(pos.X())*tile_width, static_cast<int>(pos.Y())*tile_height);
}

std::ostream& operator<<(std::ostream& os, const Vector2& v) {
    os << "[" << v.X() << ", " << v.Y() << "]";
    return os;
}

void MultiBrush::DrawSketch(Position /* scr_position */, Size /* scr_size */) const {
    if (!IsActive()) {
        return;
    }
    const double tile_width  = Engine::Get().GetRenderer()->GetTileWidth();
    const double tile_height = Engine::Get().GetRenderer()->GetTileHeight();

    const Position orgBeg = MapPosWorldToWindow(GetStart());
    const Position orgEnd = MapPosWorldToWindow(GetEnd());
    Position beg = orgBeg;
    Position end = orgEnd;

    beg[0] = std::min(orgBeg.X(), orgEnd.X());
    end[0] = std::max(orgBeg.X(), orgEnd.X());
    beg[1] = std::min(orgBeg.Y(), orgEnd.Y());
    end[1] = std::max(orgBeg.Y(), orgEnd.Y());

    const double epsilon = 0.00001;
    unsigned tiles_hor = epsilon + (end[0] - beg[0])/tile_width;
    unsigned tiles_ver = epsilon + (end[1] - beg[1])/tile_height;

    for (unsigned i=0; i<tiles_ver; i++) {
        for (unsigned j=0; j<tiles_hor; j++) {
            DrawIcon(Position(j*tile_width, i*tile_height)+beg, Size(tile_width, tile_height));
        }
    }

    Engine::Get().GetRenderer()->DrawQuad(beg, end, 1,1,0, .8);
}
