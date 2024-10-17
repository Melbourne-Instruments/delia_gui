/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  utils.h
 * @brief Utility functions.
 *-----------------------------------------------------------------------------
 */
#ifndef _UTILS_H
#define _UTILS_H

#include <QBrush>
#include <QColor>

namespace utils
{
    // System utilities
    void init();
    QColor get_system_colour();
    QString get_system_colour_str();
    QString get_css_system_colour_str();
    QBrush get_system_colour_brush();
    QString get_dimmed_css_system_colour_str(uint intensity=5);
    QBrush get_dimmed_system_colour_brush(uint intensity=5);
    void set_system_colour(const char *colour_str);
    QPixmap set_pixmap_to_system_colour(const QPixmap& pixmap);
}

#endif  // _UTILS_H
