/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2024 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  param_value_bar.cpp
 * @brief Param Value Bar class implementation.
 *-----------------------------------------------------------------------------
 */
#include "param_value_bar.h"
#include "gui_common.h"
#include "utils.h"

// Constants
constexpr uint PARAM_NAME_FONT_SIZE  = 32;
constexpr uint PARAM_VALUE_FONT_SIZE = 32;
constexpr uint PADDING               = 10;
constexpr uint BOTTOM_BORDER_SIZE    = 1;

//----------------------------------------------------------------------------
// ParamValueBar
//----------------------------------------------------------------------------
ParamValueBar::ParamValueBar(QWidget *parent) : MoniqLabel(parent)
{
    // Create the param name
    _param_name = new MoniqLabel(parent);
	_param_name->setFont(QFont(STANDARD_FONT_NAME, PARAM_NAME_FONT_SIZE));
    _param_name->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    _param_name->setText("TUNE");
    _param_name->adjustSize();
    _param_name->show();

    // Create the param value
    _param_value = new MoniqLabel(parent);
	_param_value->setFont(QFont(STANDARD_FONT_NAME, PARAM_VALUE_FONT_SIZE));
    _param_value->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    _param_value->setText("33");
    _param_value->adjustSize();
    _param_value->show();

    // Refresh the colours and hide the bar
	refresh_colour();
    //hide();
}

//----------------------------------------------------------------------------
// setGeometry
//----------------------------------------------------------------------------
void ParamValueBar::setGeometry(int x, int y, int w, int h)
{
    auto max_width = ((w / 2) - (PADDING * 2));
    
    // Set the multi status bar geometry
    MoniqLabel::setGeometry(x, y, w, h);

    // Set the param name geometry
    _param_name->setGeometry (x, y, (w / 2), (h - BOTTOM_BORDER_SIZE));

    // Set the param value geometry
    _param_value->setGeometry ((x + (w / 2)), y, (w / 2), (h - BOTTOM_BORDER_SIZE));
}

//----------------------------------------------------------------------------
// show
//----------------------------------------------------------------------------
void ParamValueBar::show()
{
    // Show the object
    _param_name->show();
    _param_value->show();
    MoniqLabel::show();
}

//----------------------------------------------------------------------------
// hide
//----------------------------------------------------------------------------
void ParamValueBar::hide()
{
    // Hide the object
    _param_name->hide();
    _param_value->hide();
    MoniqLabel::hide();
}

//----------------------------------------------------------------------------
// set_param_name
//----------------------------------------------------------------------------
void ParamValueBar::set_param_name(const QString &name)
{
    auto max_width = ((width() / 2) - (PADDING * 2));

    // Update the param name
    _param_name->set_text(name, max_width);
    _param_name->setGeometry (x(), y(), (width() / 2), (height() - BOTTOM_BORDER_SIZE));    
}

//----------------------------------------------------------------------------
// set_param_value
//----------------------------------------------------------------------------
void ParamValueBar::set_param_value(const QString& value)
{
    auto max_width = ((width() / 2) - (PADDING * 2));

    // Update the param value
    _param_value->set_text(value, max_width);
    _param_value->setGeometry ((x() + (width() / 2)), y(), (width() / 2), (height() - BOTTOM_BORDER_SIZE));    
}

//----------------------------------------------------------------------------
// refresh_colour
//----------------------------------------------------------------------------
void ParamValueBar::refresh_colour()
{
    _param_name->setStyleSheet("QLabel { background-color: black; color : " + utils::get_css_system_colour_str()  + "; padding-left: 10px; }");
    _param_value->setStyleSheet("QLabel { background-color: black; color : " + utils::get_css_system_colour_str()  + "; padding-left: 10px; border-left-width: 0.5px; border-left-style: solid; border-left-color: " + utils::get_css_system_colour_str() + ";}");
	setStyleSheet("QLabel { background-color: " + utils::get_css_system_colour_str() + "; }"); 
}
