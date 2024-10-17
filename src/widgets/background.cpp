/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  background.cpp
 * @brief Background class implementation.
 *-----------------------------------------------------------------------------
 */
#include "background.h"
#include "gui_common.h"
#include "utils.h"

//----------------------------------------------------------------------------
// Background
//----------------------------------------------------------------------------
Background::Background(QWidget *parent) : QLabel(parent)
{
    // Setup the label parameters
    setStyleSheet("QLabel { background-color : black; }");
    setScaledContents(true);
    resize(LCD_WIDTH, LCD_HEIGHT);
}

//----------------------------------------------------------------------------
// set_image
//----------------------------------------------------------------------------
void Background::set_image(const std::string &path)
{
    // Show the image
    setPixmap(utils::set_pixmap_to_system_colour(QPixmap(path.c_str())));
    setAlignment(Qt::AlignCenter);
}

//----------------------------------------------------------------------------
// clear_image
//----------------------------------------------------------------------------
void Background::clear_image()
{
    // Clear the image
    setPixmap(QPixmap());
}
