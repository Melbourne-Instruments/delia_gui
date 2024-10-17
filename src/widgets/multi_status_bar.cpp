/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023-2024 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  multi_status_bar.cpp
 * @brief Multi Status Bar class implementation.
 *-----------------------------------------------------------------------------
 */
#include "multi_status_bar.h"
#include "gui_common.h"
#include "utils.h"

// Constants
constexpr uint SOUND_NAME_FONT_SIZE = 30;
constexpr uint PADDING              = 10;
constexpr uint BOTTOM_BORDER_SIZE   = 1;

//----------------------------------------------------------------------------
// MultiStatusBar
//----------------------------------------------------------------------------
MultiStatusBar::MultiStatusBar(QWidget *parent) : MoniqLabel(parent)
{
    // Initialise class data
    _current_layer = Layer::LAYER_1;

    // Create the L1 sound name
    _l1_sound_name = new MoniqLabel(parent);
	_l1_sound_name->setFont(QFont(STANDARD_FONT_NAME, SOUND_NAME_FONT_SIZE));
    _l1_sound_name->setAlignment(Qt::AlignCenter);
    _l1_sound_name->setText("----");
    _l1_sound_name->adjustSize();
    _l1_sound_name->show();

    // Create the L2 sound name
    _l2_sound_name = new MoniqLabel(parent);
	_l2_sound_name->setFont(QFont(STANDARD_FONT_NAME, SOUND_NAME_FONT_SIZE));
    _l2_sound_name->setAlignment(Qt::AlignCenter);
    _l2_sound_name->setText("----");
    _l2_sound_name->adjustSize();
    _l2_sound_name->show();

    // Refresh the colours and hide the bar
	refresh_colour();
    hide();
}

//----------------------------------------------------------------------------
// setGeometry
//----------------------------------------------------------------------------
void MultiStatusBar::setGeometry(int x, int y, int w, int h)
{
    // Set the multi status bar geometry
    MoniqLabel::setGeometry(x, y, w, h);

    // Set the L1 sound name geometry
    _l1_sound_name->setGeometry (x, y, (w / 2), h);

    // Set the L2 sound name geometry
    _l2_sound_name->setGeometry ((x + (w / 2)), y, (w / 2), h);
}

//----------------------------------------------------------------------------
// show
//----------------------------------------------------------------------------
void MultiStatusBar::show()
{
    // Show the object
    _l1_sound_name->show();
    _l2_sound_name->show();
    MoniqLabel::show();
}

//----------------------------------------------------------------------------
// hide
//----------------------------------------------------------------------------
void MultiStatusBar::hide()
{
    // Hide the object
    _l1_sound_name->hide();
    _l2_sound_name->hide();
    MoniqLabel::hide();
}

//----------------------------------------------------------------------------
// set_l1_sound_name
//----------------------------------------------------------------------------
void MultiStatusBar::set_l1_sound_name(const QString &name)
{
    auto max_width = ((width() / 2) - (PADDING * 2));

    // Update the L1 sound name
    _l1_sound_name->set_text(name, max_width);
    _l1_sound_name->setGeometry (x(), y(), (width() / 2), (height() - BOTTOM_BORDER_SIZE));
}

//----------------------------------------------------------------------------
// set_l2_sound_name
//----------------------------------------------------------------------------
void MultiStatusBar::set_l2_sound_name(const QString &name)
{
    auto max_width = ((width() / 2) - (PADDING * 2));

    // Update the L2 sound name
    _l2_sound_name->set_text(name, max_width);
    _l2_sound_name->setGeometry ((x() + (width() / 2)), y(), (width() / 2), (height() - BOTTOM_BORDER_SIZE));
}

//----------------------------------------------------------------------------
// select_layer
//----------------------------------------------------------------------------
void MultiStatusBar::select_layer(Layer layer)
{
    // Save the current layer and refresh the object
    _current_layer = layer;
    refresh_colour();
}

//----------------------------------------------------------------------------
// refresh_colour
//----------------------------------------------------------------------------
void MultiStatusBar::refresh_colour()
{
    // Refresh the layer sound bar object colours
    if (_current_layer == Layer::LAYER_1) {
        _l1_sound_name->setStyleSheet("QLabel { background-color: " + utils::get_css_system_colour_str() + "; }");
        _l2_sound_name->setStyleSheet("QLabel { background-color: black; color : " + utils::get_css_system_colour_str()  + "; }");
    }
    else {
        _l1_sound_name->setStyleSheet("QLabel { background-color: black; color : " + utils::get_css_system_colour_str()  + "; }");
        _l2_sound_name->setStyleSheet("QLabel { background-color: " + utils::get_css_system_colour_str() + "; }");   
    }
	setStyleSheet("QLabel { background-color: " + utils::get_css_system_colour_str() + "; }"); 
}
