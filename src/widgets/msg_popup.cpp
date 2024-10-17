/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  msg_popup.cpp
 * @brief Message Popup widget class implementation.
 *-----------------------------------------------------------------------------
 */
#include "msg_popup.h"
#include "gui_common.h"
#include "utils.h"

// Constants
constexpr uint LINE_1_FONT_SIZE = 30;
constexpr uint LINE_2_FONT_SIZE = 40;
constexpr uint LINE_MARGIN      = 40;
constexpr uint SPACING          = 20;
constexpr uint POPUP_TIMEOUT_MS = 1000;

//----------------------------------------------------------------------------
// MsgPopup
//----------------------------------------------------------------------------
MsgPopup::MsgPopup(QWidget *parent) : MoniqLabel(parent)
{
    // Create the line objects
    _line_1 = new MoniqLabel(this);
	_line_1->setFont(QFont(STANDARD_FONT_NAME, LINE_1_FONT_SIZE));
    _line_1->setAlignment(Qt::AlignCenter);
	_line_1->setStyleSheet("QLabel { background-color: transparent; color : black; border: none; }");  
    _line_2 = new MoniqLabel(this);
	_line_2->setFont(QFont(STANDARD_FONT_NAME, LINE_2_FONT_SIZE));
    _line_2->setAlignment(Qt::AlignCenter);
	_line_2->setStyleSheet("QLabel { background-color: transparent; color : black; border: none; }"); 

    // Create the popup timer
    _timer.setSingleShot(true);
    connect(&_timer, &QTimer::timeout, this, &MsgPopup::_timeout);

    // Set the initial colour
    refresh_colour();
    hide();    
}

//----------------------------------------------------------------------------
// show_msg
//----------------------------------------------------------------------------
void MsgPopup::show_msg(const QString& line_1, const QString& line_2)
{
    uint x_offset;
    uint y_offset;

    // Stop the timer
    _timer.stop();

    // Show the message box
    _line_1->set_text(line_1, (width() - (LINE_MARGIN * 2)));
    _line_2->set_text(line_2, (width() - (LINE_MARGIN * 2)));

    // Calculate the line 1 x offset
    x_offset = (width() - _line_1->width()) / 2;
    if (x_offset < LINE_MARGIN) {
        x_offset = LINE_MARGIN;
    }

    // Centre the line 1 in the message popup
    y_offset = (height() - _line_1->height() - SPACING - _line_2->height()) / 2;
    _line_1->setGeometry (x_offset, y_offset, _line_1->width(), _line_1->height());

    // Calculate the line 1 x offset
    x_offset = (width() - _line_2->width()) / 2;
    if (x_offset < LINE_MARGIN) {
        x_offset = LINE_MARGIN;
    }   
    y_offset += _line_1->height() + SPACING;
    _line_2->setGeometry (x_offset, y_offset, _line_2->width(), _line_2->height());

    // Show the popup
    refresh_colour();
    show();

    // Start the popup timer
    _timer.start(POPUP_TIMEOUT_MS);
}

//----------------------------------------------------------------------------
// refresh_colour
//----------------------------------------------------------------------------
void MsgPopup::refresh_colour()
{
    // Refresh the message dialog object colours
    setStyleSheet("QLabel { background-color: " + utils::get_css_system_colour_str() + "; color : black; border: 40px solid black; }");  	 
}

//----------------------------------------------------------------------------
// _timeout
//----------------------------------------------------------------------------
void MsgPopup::_timeout()
{
    // Hide the popup on timeout
    hide();
}
