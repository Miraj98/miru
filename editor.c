#include "editor.h"

struct Editor {
    enum EditorMode mode;
};

Editor editor = { E_NORMAL_MODE };

void updateEditorMode(Editor *e, enum EditorMode m) {
    e->mode = m;
}

enum EditorMode getEditorMode(Editor *e) {
    return e->mode;
}
