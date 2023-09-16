#ifndef EDITOR_H
#define EDITOR_H

enum EditorMode {
    E_NORMAL_MODE,
    E_INSERT_MODE
};

typedef struct Editor Editor;
extern Editor editor;

enum EditorMode getEditorMode(Editor *e);
void updateEditorMode(Editor *e, enum EditorMode m);
void setEditorSize(Editor *e, int rows, int cols);
int getEditorRows(Editor *e);
int getEditorCols(Editor *e);

#endif // EDITOR_H
