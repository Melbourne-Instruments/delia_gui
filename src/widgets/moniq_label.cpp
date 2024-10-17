/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  moniq_label.cpp
 * @brief Moniq Label class implementation.
 *-----------------------------------------------------------------------------
 */
#include "moniq_label.h"
#include "gui_common.h"
#include "utils.h"

//----------------------------------------------------------------------------
// MoniqLabel
//----------------------------------------------------------------------------
MoniqLabel::MoniqLabel(QWidget *parent) 
    : QLabel(parent)
{
    // Nothing specific to do
}

//----------------------------------------------------------------------------
// set_text
//----------------------------------------------------------------------------
void MoniqLabel::set_text(const QString& text, int max_width)
{
    QString str = text;

    // Update the object text
    setText(str);
    adjustSize();
    if (max_width != -1) {
        uint w = width();
        while (w > (uint)max_width) {
            str = str.mid(0, str.size() - 1);
            QString trunc_str = str + "~";
            setText(trunc_str);
            adjustSize();
            w = width();
        }
    }
}

//----------------------------------------------------------------------------
// refresh_colour
//----------------------------------------------------------------------------
void MoniqLabel::refresh_colour()
{
    // Each class should implement this
}
