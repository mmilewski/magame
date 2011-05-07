#include "Editor.h"
#include "EditorCommand.h"

#include "../TileGridHelper.h"

bool PlatformEditorCommand::IsReady() const {
    if (m_beg.X() < 0 || m_beg.Y() < 0 || m_end.X() < 0 || m_end.Y() < 0) {
        return false;
    }
    TileGridHelper tgh(m_beg, m_end);
    tgh.SnapToGrid().SortCoordsOfBox();
    unsigned tiles_hor = tgh.TilesHorizontally();
    unsigned tiles_ver = tgh.TilesVertically();
    if (tiles_hor == 1) return false;   // szerokość == 1
    if (tiles_hor < 1 || tiles_ver < 1) return false;    // mniej niż 1x1
    if (tiles_ver == 1 && tiles_hor == 1) return false;  // dokładnie 1x1
    return true;
}

void PlatformEditorCommand::Execute(Editor* editor) {
    if (IsReady() == false) {
        throw std::logic_error("PlatformEditorCommand: Nie można uruchomić "
                "polecenia, które nie jest gotowe.");
    }
    TileGridHelper tgh(m_beg, m_end);
    tgh.SnapToGrid().SortCoordsOfBox();
    unsigned tiles_hor = tgh.TilesHorizontally();
    unsigned tiles_ver = tgh.TilesVertically();

    // Zapisz kafelki, które aktualnie występują na planszy
    for (unsigned ver = 0; ver < tiles_ver; ver++) { // wypełnienie
        for (unsigned hor = 0; hor < tiles_hor; hor++) {
            m_saved_fields.push_back(
                    editor->GetFieldAt(tgh.Beg() + Position(hor, ver)));
        }
    }

    // Prostokąt
    for (unsigned ver = 0; ver < tiles_ver; ver++) { // wypełnienie
        for (unsigned hor = 0; hor < tiles_hor; hor++) {
            editor->SetFieldAt(tgh.Beg() + Position(hor, ver), FT::PlatformMid);
        }
    }
    for (unsigned hor = 0; hor < tiles_hor; hor++) { // górna krawędź
        editor->SetFieldAt(tgh.Beg() + Position(hor, tiles_ver - 1), FT::PlatformTop);
    }
    for (unsigned ver = 0; ver < tiles_ver; ver++) { // boki
        editor->SetFieldAt(tgh.Beg() + Position(0, ver), FT::PlatformLeft);
        editor->SetFieldAt(tgh.Beg() + Position(tiles_hor - 1, ver), FT::PlatformRight);
    }
    // narożniki (górny lewy i górny prawy)
    editor->SetFieldAt(tgh.Beg() + Position(0, tiles_ver - 1), FT::PlatformTopLeft);
    editor->SetFieldAt(tgh.Beg() + Position(tiles_hor - 1, tiles_ver - 1), FT::PlatformTopRight);
}

void PlatformEditorCommand::Undo(Editor* editor) {
    if (IsNotReady()) {
        throw std::logic_error("PlatformEditorCommand: Nie można wycofać "
                "polecenia, które nie jest gotowe.");
    }
    if (m_saved_fields.empty()) {
        return;
    }
    TileGridHelper tgh(m_beg, m_end);
    tgh.SnapToGrid().SortCoordsOfBox();
    unsigned tiles_hor = tgh.TilesHorizontally();
    unsigned tiles_ver = tgh.TilesVertically();
    assert(tiles_hor * tiles_ver == m_saved_fields.size() && "Zła liczba pól");
    std::vector<FT::FieldType>::const_iterator it = m_saved_fields.begin();
    for (unsigned ver = 0; ver < tiles_ver; ver++) { // wypełnienie
        for (unsigned hor = 0; hor < tiles_hor; hor++) {
            editor->SetFieldAt(tgh.Beg() + Position(hor, ver), *it);
            ++it;
        }
    }
}
