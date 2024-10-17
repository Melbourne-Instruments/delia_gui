/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023-2024 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  bottom_bar.h
 * @brief Bottom Bar class definitions.
 *-----------------------------------------------------------------------------
 */
#ifndef BOTTOM_BAR_H
#define BOTTOM_BAR_H

#include <QString>
#include "gui_common.h"
#include "gui_msg.h"
#include "moniq_label.h"

// Bottom Bar class
class BottomBar: public MoniqLabel
{
    Q_OBJECT
public:
    // Constructors
    explicit BottomBar(QWidget *parent = nullptr);

    // Overriden functions
    void setGeometry(int x, int y, int w, int h);

    // Public functions
    void set_soft_button_text(SoftButtonId id, const QString& text);
    void set_soft_button_state(SoftButtonId id, int state);
    void refresh_colour();

private:
    // Private data
    MoniqLabel *_soft_button1;
    MoniqLabel *_soft_button2;
    bool _soft_button1_state;
    bool _soft_button2_state;
};

#endif
