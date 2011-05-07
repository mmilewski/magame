#include "../TileGridHelper.h"
#include "Brush.h"

void MultiBrush::DrawSketch(Position /* scr_position */, Size /* scr_size */) const {
    if (!IsActive()) {
        return;
    }

    TileGridHelper tgh(GetStart(), GetEnd());
    tgh.SnapToGrid();
    tgh.SortCoordsOfBox();

    unsigned tiles_hor = tgh.TilesHorizontally(), tiles_ver = tgh.TilesVertically();

    if (tiles_hor > 50 || tiles_ver > 50) {
        std::cerr << "[MultiBrush::DrawSketch] Uzyskano niepokojąco duże wartości:"
                << "\n\ttiles_hor: " << tiles_hor
                << "\n\ttiles_ver: " << tiles_ver
                << "\n";
    }

    const double tile_width = Engine::Get().GetRenderer()->GetTileWidth();
    const double tile_height = Engine::Get().GetRenderer()->GetTileHeight();
    Position begWorld = tgh.Beg().scale(tile_width, tile_height),
             endWorld = tgh.End().scale(tile_width, tile_height);
    for (unsigned i = 0; i < tiles_ver; i++) {
        for (unsigned j = 0; j < tiles_hor; j++) {
            Sprite::GetByName("PlatformMid")->DrawCurrentFrame(
                begWorld + Position(j*tile_width, i * tile_height),
                Size(tile_width, tile_height));
        }
    }

    Engine::Get().GetRenderer()->DrawQuad(begWorld, endWorld, 1,1,0, .7);
}
