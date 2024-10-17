/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  edit_name.h
 * @brief Edit Name widget class definitions.
 *-----------------------------------------------------------------------------
 */
#ifndef EDIT_NAME_H
#define EDIT_NAME_H

#include <QObject>
#include <QString>
#include "gui_msg.h"
#include "moniq_label.h"
#include "gui_common.h"

// Constants
constexpr uint NUM_ASCII_CHARS   = (1 + 26 + 10 + 1);
constexpr uint ASCII_PICKER_SIZE = 7;

// Edit Name class
class EditName: public MoniqLabel
{
    Q_OBJECT    
public:
    // Constructors
    explicit EditName(QWidget *parent = nullptr);

    // Overriden functions
    void setGeometry(int x, int y, int w, int h);
    void hide();

    // Public functions
    void show_edit_name(const QString& name);
    void select_edit_name_char(uint index);
    void change_edit_name_char(uint ascii_index);   
    void refresh_colour();

private:
    // Private data
    QLabel *_edit_name[EDIT_NAME_STR_LEN];
    QLabel *_ascii_picker[ASCII_PICKER_SIZE];
    char _ascii_chars[NUM_ASCII_CHARS];
    int _selected_char;

    // Private functions
    void _set_ascii_picker_geometry();
};

#endif
