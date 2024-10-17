/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023-2024 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  multi_status_bar.h
 * @brief Multi Status Bar class definitions.
 *-----------------------------------------------------------------------------
 */
#ifndef MULTI_STATUS_BAR_H
#define MULTI_STATUS_BAR_H

#include <QString>
#include "moniq_label.h"
#include "gui_msg.h"

// Multi Status Bar class
class MultiStatusBar: public MoniqLabel
{
    Q_OBJECT
public:
    // Constructors
    explicit MultiStatusBar(QWidget *parent = nullptr);

    // Overriden functions
    void setGeometry(int x, int y, int w, int h);
    void show();
    void hide();

    // Public functions
    void set_l1_sound_name(const QString &name);
    void set_l2_sound_name(const QString &name);
    void select_layer(Layer layer);
    void refresh_colour();

private:
    // Private data
    MoniqLabel *_l1_sound_name;
    MoniqLabel *_l2_sound_name;
    Layer _current_layer;
};

#endif  // MULTI_STATUS_BAR_H
