#include "StdAfx.h"

#include "TileGridHelper.h"
#include "Brush.h"

void MultiBrush::DrawSketch(Position /* scr_position */, Size /* scr_size */) const {
    if (!IsActive()) {
        return;
    }

    TileGridHelper tgh(GetStart(), GetEnd());
    tgh.SnapToGrid();
    tgh.SortCoordsOfBox();

    const unsigned tiles_hor = tgh.TilesHorizontally(),
                   tiles_ver = tgh.TilesVertically();

    if (tiles_hor > 50 || tiles_ver > 50) {
        std::cerr << "[MultiBrush::DrawSketch] Uzyskano niepokojąco duże wartości:"
                << "\n\ttiles_hor: " << tiles_hor
                << "\n\ttiles_ver: " << tiles_ver
                << "\n";
    }

    const Size tile_size = Engine::Get().GetRenderer()->GetTileSize();
    Position begWorld = tgh.Beg().Scale(tile_size),
             endWorld = tgh.End().Scale(tile_size);
    for (unsigned col = 0; col < tiles_ver; col++) {
        for (unsigned row = 0; row < tiles_hor; row++) {
            Sprite::GetByName("PlatformMid")->DrawCurrentFrame(begWorld + Position(row * tile_size[0], col * tile_size[1]),
                                                               tile_size);
        }
    }

    Engine::Get().GetRenderer()->DrawQuad(begWorld, endWorld, Rgba(1,1,0, .7));
}
