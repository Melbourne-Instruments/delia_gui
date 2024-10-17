/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023-2024 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  msg_box.cpp
 * @brief Message Box widget class implementation.
 *-----------------------------------------------------------------------------
 */
#include <QMovie>
#include "msg_box.h"
#include "gui_common.h"
#include "utils.h"

// Constants
constexpr uint LINE_1_FONT_SIZE          = 30;
constexpr uint LINE_2_FONT_SIZE          = 30;
constexpr uint LINE_3_FONT_SIZE          = 30;
constexpr uint HOURGLASS_WIDTH           = 60;
constexpr uint HOURGLASS_HEIGHT          = 60;
constexpr uint BORDER_MARGIN             = 40;
constexpr uint LINE_MARGIN               = 15;
constexpr uint SPACING                   = 20;
constexpr uint HOURGLASS_REFRESH_RATE_MS = 40;

//----------------------------------------------------------------------------
// MsgBox
//----------------------------------------------------------------------------
MsgBox::MsgBox(QWidget *parent) : MoniqLabel(parent)
{
    // Create the border and line objects
    _border = new MoniqLabel(this);
    _line_1 = new MoniqLabel(this);
	_line_1->setFont(QFont(STANDARD_FONT_NAME, LINE_1_FONT_SIZE));
    _line_1->setAlignment(Qt::AlignCenter);
    _line_2 = new MoniqLabel(this);
	_line_2->setFont(QFont(STANDARD_FONT_NAME, LINE_2_FONT_SIZE));
    _line_2->setAlignment(Qt::AlignCenter);
    _line_3 = new MoniqLabel(this);
	_line_3->setFont(QFont(STANDARD_FONT_NAME, LINE_2_FONT_SIZE));
    _line_3->setAlignment(Qt::AlignCenter);

    // Create the hourglass object
    _hourglass = new MoniqLabel(this);
    _hourglass->setAlignment(Qt::AlignCenter);
    _hourglass->setGeometry (0, 0, HOURGLASS_WIDTH, HOURGLASS_HEIGHT);

    // Create the hourglass timer
    _hourglass_timer = new QTimer(this);
    connect(_hourglass_timer, &QTimer::timeout, this, &MsgBox::_update_hourglass);

    // Set the initial colour
    setStyleSheet("QLabel { background-color: black; }");
    refresh_colour();
    hide();
}

//----------------------------------------------------------------------------
// setGeometry
//----------------------------------------------------------------------------
void MsgBox::setGeometry(int x, int y, int w, int h)
{
    // Set the message box geometry
    MoniqLabel::setGeometry(x, y, w, h);

    // Set the border geometry
    _border->setGeometry (BORDER_MARGIN, BORDER_MARGIN, (w - (BORDER_MARGIN * 2)), (h - (BORDER_MARGIN * 2)));
}

//----------------------------------------------------------------------------
// hide
//----------------------------------------------------------------------------
void MsgBox::hide()
{
    // Hide the message box
    _hourglass_timer->stop();
    _hourglass->hide();
    MoniqLabel::hide();
}

//----------------------------------------------------------------------------
// show_msg
//----------------------------------------------------------------------------
void MsgBox::show_msg(const QString& line_1, const QString& line_2, const QString& line_3, bool show_hourglass)
{
    uint x_offset;
    uint y_offset;
    bool show_line_2 = std::strlen(line_2.toStdString().c_str());
    bool show_line_3 = std::strlen(line_3.toStdString().c_str());
    int line_1_total_height = _line_1->height() + SPACING;
    int line_2_total_height = show_line_2 ? _line_2->height() + SPACING : 0;
    int line_3_total_height = show_line_3 ? _line_3->height() + SPACING : 0;
    int hourglass_height = show_hourglass ? _hourglass->height() : 0;

    // Show the message box
    _line_1->set_text(line_1, (_border->width() - (LINE_MARGIN * 2)));
    show_line_2 ?
        _line_2->set_text(line_2, (_border->width() - (LINE_MARGIN * 2))) :
        _line_2->hide();
    show_line_3 ?
        _line_3->set_text(line_3, (_border->width() - (LINE_MARGIN * 2))) :
        _line_3->hide();

    // Calculate the line 1 x offset
    x_offset = (LCD_WIDTH - _line_1->width()) / 2;
    if (x_offset < LINE_MARGIN) {
        x_offset = LINE_MARGIN;
    }

    // Centre the line 1 in the message box - depends on whether the hourglass is shown or not
    y_offset = (height() - line_1_total_height - line_2_total_height - line_3_total_height - hourglass_height) / 2;
    _line_1->setGeometry (x_offset, y_offset, _line_1->width(), _line_1->height());
    y_offset += line_1_total_height;

    // If we are showing line 2
    if (show_line_2) {
        // Calculate the line 2 x offset
        x_offset = (LCD_WIDTH - _line_2->width()) / 2;
        if (x_offset < LINE_MARGIN) {
            x_offset = LINE_MARGIN;
        }   
        _line_2->setGeometry (x_offset, y_offset, _line_2->width(), _line_2->height());
        _line_2->show();
        y_offset += line_2_total_height;
    }

    // If we are showing line 3
    if (show_line_3) {
        // Calculate the line 3 x offset
        x_offset = (LCD_WIDTH - _line_3->width()) / 2;
        if (x_offset < LINE_MARGIN) {
            x_offset = LINE_MARGIN;
        }   
        _line_3->setGeometry (x_offset, y_offset, _line_3->width(), _line_3->height());
        _line_3->show();
        y_offset += line_3_total_height;
    }

    // Should we show the hourglass?
    if (show_hourglass) {
        // Set the hourglass geometry
        x_offset = (LCD_WIDTH - _hourglass->width()) / 2;
        _hourglass->setGeometry (x_offset, y_offset, _hourglass->width(), _hourglass->height());
        _hourglass_pixmap_index = 0;
        _hourglass_timer->start(HOURGLASS_REFRESH_RATE_MS);
        _hourglass->show();
    }
    else {
        // Stop and hide the hourglass
        _hourglass_timer->stop();
        _hourglass->hide();
    }
    show();
}

//----------------------------------------------------------------------------
// refresh_colour
//----------------------------------------------------------------------------
void MsgBox::refresh_colour()
{
    // Refresh the message dialog object colours
    _border->setStyleSheet("QLabel { background-color: black; border: 10px solid " + utils::get_css_system_colour_str() + "; }");
	_line_1->setStyleSheet("QLabel { background-color: black; color : " + utils::get_css_system_colour_str() + "; }"); 
	_line_2->setStyleSheet("QLabel { background-color: black; color : " + utils::get_css_system_colour_str() + "; }"); 
	_line_3->setStyleSheet("QLabel { background-color: black; color : " + utils::get_css_system_colour_str() + "; }"); 

    // Create the warning hourglass - firstly we need to read each pixmap
    // from the hourglass GIF
    auto *movie = new QMovie(HOURGLASS_GIF_RES);
    movie->setScaledSize(QSize(60, 60));
    _hourglass_pixmaps.clear();
    for (int i=0; i<movie->frameCount(); i++) {
        movie->jumpToFrame(i);
        _hourglass_pixmaps.push_back(utils::set_pixmap_to_system_colour(movie->currentPixmap()));
    }	 
}

//----------------------------------------------------------------------------
// _update_hourglass
//----------------------------------------------------------------------------
void MsgBox::_update_hourglass()
{
    // Show the next hourglass image
    _hourglass->setPixmap(_hourglass_pixmaps.at(_hourglass_pixmap_index++));
    if (_hourglass_pixmap_index >= _hourglass_pixmaps.size()) {
        _hourglass_pixmap_index = 0;
    }
}
