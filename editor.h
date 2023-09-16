#ifndef EDITOR_H
#define EDITOR_H

enum EditorMode {
    E_NORMAL_MODE,
    E_INSERT_MODE
};

typedef struct Editor Editor;
extern Editor editor;

void updateEditorMode(Editor *e, enum EditorMode m);
enum EditorMode getEditorMode(Editor *e);

#endif // EDITOR_H
