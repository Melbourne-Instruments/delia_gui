/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023-2024 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  bottom_bar.cpp
 * @brief Bottom Bar class implementation.
 *-----------------------------------------------------------------------------
 */
#include "bottom_bar.h"
#include "utils.h"

// Constants
constexpr uint SOFT_BUTTON_FONT_SIZE = 32;
constexpr uint SOFT_BUTTON_WIDTH     = 281;

//----------------------------------------------------------------------------
// BottomBar
//----------------------------------------------------------------------------
BottomBar::BottomBar(QWidget *parent) : MoniqLabel(parent)
{
    // Create the soft button objects
    _soft_button1 = new MoniqLabel(parent);
    _soft_button1->setFont(QFont(STANDARD_FONT_NAME, SOFT_BUTTON_FONT_SIZE));
    _soft_button1->setAlignment(Qt::AlignTop|Qt::AlignHCenter);
    _soft_button1->setText("----");
    _soft_button2 = new MoniqLabel(parent);
    _soft_button2->setFont(QFont(STANDARD_FONT_NAME, SOFT_BUTTON_FONT_SIZE));
    _soft_button2->setAlignment(Qt::AlignTop|Qt::AlignHCenter);
    _soft_button2->setText("----");
    _soft_button1_state = false;
    _soft_button2_state = false;

    // Setup the status bar parameters
	refresh_colour();
    show();
}

//----------------------------------------------------------------------------
// setGeometry
//----------------------------------------------------------------------------
void BottomBar::setGeometry(int x, int y, int w, int h)
{
    int button_width = w / 2;

    // Set the status bar geometry
    MoniqLabel::setGeometry(x, y, w, h);

    // Set the soft button geometry
    _soft_button1->setGeometry (x, y, button_width, h);
    _soft_button2->setGeometry ((x + w - button_width), y, button_width, h);
}

//----------------------------------------------------------------------------
// set_soft_button_text
//----------------------------------------------------------------------------
void BottomBar::set_soft_button_text(SoftButtonId id, const QString& text)
{
    MoniqLabel *soft_button = id == SoftButtonId::BUTTON_1 ? _soft_button1 : _soft_button2;

    // Set the soft button text
    if (text.size() > 0)
        soft_button->setText(text);
    else
        soft_button->setText("----");
}

//----------------------------------------------------------------------------
// set_soft_button_state
//----------------------------------------------------------------------------
void BottomBar::set_soft_button_state(SoftButtonId id, int state)
{
    // If we should update this button state
    if (state != -1) {
        // Update the soft button state depending if pressed or not
        if (state) {
            id == SoftButtonId::BUTTON_1 ? _soft_button1_state = true : _soft_button2_state = true;
            id == SoftButtonId::BUTTON_1 ?
                _soft_button1->setStyleSheet("QLabel { color: black; background-color: " + utils::get_css_system_colour_str() + "; border-top: 2px solid " + utils::get_css_system_colour_str() + "; border-right: 1px solid " + utils::get_css_system_colour_str() + ";}") :
                _soft_button2->setStyleSheet("QLabel { color: black; background-color: " + utils::get_css_system_colour_str() + "; border-top: 2px solid " + utils::get_css_system_colour_str() + "; border-left: 1px solid " + utils::get_css_system_colour_str() + ";}");
        }
        else {
            id == SoftButtonId::BUTTON_1 ? _soft_button1_state = false : _soft_button2_state = false;
            id == SoftButtonId::BUTTON_1 ?
                _soft_button1->setStyleSheet("QLabel { color: " + utils::get_css_system_colour_str() + "; background-color: black; border-top: 2px solid " + utils::get_css_system_colour_str() + "; border-right: 1px solid " + utils::get_css_system_colour_str() + ";}") :
                _soft_button2->setStyleSheet("QLabel { color: " + utils::get_css_system_colour_str() + "; background-color: black; border-top: 2px solid " + utils::get_css_system_colour_str() + "; border-left: 1px solid " + utils::get_css_system_colour_str() + ";}");                          
        }
    }
}

//----------------------------------------------------------------------------
// refresh_colour
//----------------------------------------------------------------------------
void BottomBar::refresh_colour()
{
    // Refresh the bottom bar object colours
    setStyleSheet("QLabel { color: " + utils::get_css_system_colour_str() + "; background-color: black; border-top: 2px solid " + utils::get_css_system_colour_str() + ";}");
    set_soft_button_state(SoftButtonId::BUTTON_1, _soft_button1_state);
    set_soft_button_state(SoftButtonId::BUTTON_2, _soft_button2_state);
}
