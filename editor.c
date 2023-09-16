#include "editor.h"

struct Editor {
    int rows;
    int cols;
    enum EditorMode mode;
};

Editor editor = { .mode = E_NORMAL_MODE, .rows = 0, .cols = 0 };

void updateEditorMode(Editor *e, enum EditorMode m) {
    e->mode = m;
}

enum EditorMode getEditorMode(Editor *e) {
    return e->mode;
}

void setEditorSize(Editor *e, int rows, int cols) {
    e->rows = rows;
    e->cols = cols;
}

int getEditorRows(Editor *e) {
    return e->rows;
}

int getEditorCols(Editor *e) {
    return e->cols;
}
