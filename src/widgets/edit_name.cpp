/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  edit_name.cpp
 * @brief Edit Name widget class implementation.
 *-----------------------------------------------------------------------------
 */
#include "edit_name.h"
#include "gui_common.h"
#include "utils.h"

// Constants
constexpr uint EDIT_NAME_FONT_SIZE      = 32;
constexpr uint EDIT_NAME_SEL_CHAR_WIDTH = 32;
constexpr uint EDIT_NAME_CHAR_WIDTH     = 24;
constexpr uint EDIT_NAME_CHAR_HEIGHT    = 44;

//----------------------------------------------------------------------------
// EditName
//----------------------------------------------------------------------------
EditName::EditName(QWidget *parent) : MoniqLabel(parent)
{
    // Create the Edit Name object (array of labels)
    for (uint i=0; i<EDIT_NAME_STR_LEN; i++) {
        // Create the character object - note the edit name is shown in the middle of the
        // ASCII picker
        _edit_name[i] = new QLabel(parent);
        _edit_name[i]->setFont(QFont(STANDARD_FONT_NAME, EDIT_NAME_FONT_SIZE));
        _edit_name[i]->setAlignment(Qt::AlignCenter);
    }

    // Create the ASCII picker object (array of labels)
    for (uint i=0; i<ASCII_PICKER_SIZE; i++) {
        // Create the character object
        _ascii_picker[i] = new QLabel(parent);
        _ascii_picker[i]->setFont(QFont(STANDARD_FONT_NAME, EDIT_NAME_FONT_SIZE));
        _ascii_picker[i]->setAlignment(Qt::AlignCenter);
    }

    // Create an array of ASCII characters we are allowed to display when editing a name
    // Add 0-9 and A-Z only
    uint j=0;
    _ascii_chars[j++] = ' ';
    for (uint i=0; i<26; i++) {
        _ascii_chars[j++] = 'A' + i; 
    }
    for (uint i=0; i<10; i++) {
        _ascii_chars[j++] = '0' + i; 
    }
    _ascii_chars[j] = '-';

    // Initialise other variables
    _selected_char = -1;

    // Set the initial colour
    setStyleSheet("QLabel { background-color: black; }");
    refresh_colour();
    hide();
}

//----------------------------------------------------------------------------
// setGeometry
//----------------------------------------------------------------------------
void EditName::setGeometry(int x, int y, int w, int h)
{
    uint left_margin = x + (w - ((EDIT_NAME_STR_LEN - 1) * EDIT_NAME_CHAR_WIDTH) - EDIT_NAME_SEL_CHAR_WIDTH) / 2;
    uint top_margin = y + (h - (ASCII_PICKER_SIZE * EDIT_NAME_CHAR_HEIGHT)) / 2;

    // Set the overall edit name geometry
    MoniqLabel::setGeometry(x, y, w, h);

    // Set the geometry for each edit name character
    uint x_offset = left_margin;
    uint y_offset = top_margin;
    uint width = EDIT_NAME_SEL_CHAR_WIDTH;
    for (uint i=0; i<EDIT_NAME_STR_LEN; i++)  {
        // Set the edit name characters geometry
        uint width = (i == 0 ? EDIT_NAME_SEL_CHAR_WIDTH : EDIT_NAME_CHAR_WIDTH);
        _edit_name[i]->setGeometry(x_offset, 
                                   (y_offset + (EDIT_NAME_CHAR_HEIGHT * 3)),
                                   width,
                                   EDIT_NAME_CHAR_HEIGHT);
        x_offset += width;   
    }

    // Set the geometry for each ASCII picker character
    x_offset = left_margin;
    y_offset = top_margin;
    for (uint i=0; i<ASCII_PICKER_SIZE; i++) {
        _ascii_picker[i]->setGeometry(x_offset, 
                                      y_offset,
                                      EDIT_NAME_SEL_CHAR_WIDTH,
                                      EDIT_NAME_CHAR_HEIGHT);
        y_offset += EDIT_NAME_CHAR_HEIGHT;
    }
}

//----------------------------------------------------------------------------
// hide
//----------------------------------------------------------------------------
void EditName::hide()
{
    // Hide the Edit Name and ASCII Picker
    for (uint i=0; i<EDIT_NAME_STR_LEN; i++)
        _edit_name[i]->hide();
    for (uint i=0; i<ASCII_PICKER_SIZE; i++)
        _ascii_picker[i]->hide();
    MoniqLabel::hide();
    _selected_char = -1;
}

//----------------------------------------------------------------------------
// show_edit_name
//----------------------------------------------------------------------------
void EditName::show_edit_name(const QString& name)
{
    // Has a selected character been specified?
    if (_selected_char == -1) {
        int x_offset = _edit_name[0]->x();
        int width;

        // Always select one following the last character
        _selected_char = (name.size() < EDIT_NAME_STR_LEN) ? name.size() : (EDIT_NAME_STR_LEN-1);

        // Set the style for each character in the name
        for (uint i=0; i<EDIT_NAME_STR_LEN; i++) {
            // Highlight the selected character
            if (i == (uint)_selected_char) {
                _edit_name[i]->setStyleSheet("QLabel { background-color:  " + utils::get_css_system_colour_str() + "; color : black; }");
                width = EDIT_NAME_SEL_CHAR_WIDTH;
            }
            else {
                _edit_name[i]->setStyleSheet("QLabel { color : " + utils::get_css_system_colour_str() + "; }");
                width = EDIT_NAME_CHAR_WIDTH;
            }

            // Set the character geometry
            _edit_name[i]->setGeometry (x_offset, (_ascii_picker[0]->y() + (EDIT_NAME_CHAR_HEIGHT * 3)), width, EDIT_NAME_CHAR_HEIGHT);
            x_offset += width;            
        }
    }

    // Set the name text
    uint i = 0;
    for (; i<name.size(); i++) {
        // Show each character in the name
        _edit_name[i]->setText(name.at(i));
        _edit_name[i]->show();
    }
    _edit_name[i]->setText(" ");
    _edit_name[i]->show();

    // Set the ASCII picker geometry
    _set_ascii_picker_geometry();   
}

//----------------------------------------------------------------------------
// select_edit_name_char
//----------------------------------------------------------------------------
void EditName::select_edit_name_char(uint index)
{
    int x_offset = _edit_name[0]->x();
    int width;

    // Process each character in the edit name
    _selected_char = index;
    for (uint i=0; i<EDIT_NAME_STR_LEN; i++)  {
        // Is this the selected character?
        if (i == _selected_char) {
            // Highlight it and set the selected character width
            _edit_name[i]->setStyleSheet("QLabel { background-color:  " + utils::get_css_system_colour_str() + "; color : black; }");
            width = EDIT_NAME_SEL_CHAR_WIDTH;
        }
        else if (i < _selected_char) {
            // Show the character normally
            _edit_name[i]->setStyleSheet("QLabel { color : " + utils::get_css_system_colour_str() + "; }");
            width = EDIT_NAME_CHAR_WIDTH;
        }
        else {
            // Show a dimmed character
            _edit_name[i]->setStyleSheet("QLabel { color : " + utils::get_dimmed_css_system_colour_str() + "; }");
            width = EDIT_NAME_CHAR_WIDTH;            
        }

        // Set the character geometry
        _edit_name[i]->setGeometry (x_offset, (_ascii_picker[0]->y() + (EDIT_NAME_CHAR_HEIGHT * 3)), width, EDIT_NAME_CHAR_HEIGHT);
        x_offset += width;
    }
    
    // Set the ASCII picker geometry
    _set_ascii_picker_geometry();
}

//----------------------------------------------------------------------------
// change_edit_name_char
//----------------------------------------------------------------------------
void EditName::change_edit_name_char(uint char_index)
{
    // Get the ASCII list index to show and range check it
    // If out of range select the default character
    if (char_index > (NUM_ASCII_CHARS - 1))
        char_index = 1;
    
    // Hide the selected character in the name
    _edit_name[_selected_char]->setVisible(false);

    // Set the ASCII chars to show in the list
    int index = char_index - 3;
    for (uint i=0; i<ASCII_PICKER_SIZE; i++)
    {
        // If the index can be used to show a character
        if ((index >=0) && (index < (int)NUM_ASCII_CHARS)) {
            // Set the list item character and show it
            std::string str;
            str = _ascii_chars[index];
            _ascii_picker[i]->setText(str.c_str());
            _ascii_picker[i]->show();
        }
        else {
            // The index is out of range, so don't show this picker item
            _ascii_picker[i]->hide();
        }
        index++;
    }
}

//----------------------------------------------------------------------------
// refresh_colour
//----------------------------------------------------------------------------
void EditName::refresh_colour()
{
    // Refresh the edit name object colours
    for (uint i=0; i<EDIT_NAME_STR_LEN; i++) {
        // If this is the first character
        if (i == 0) {
            // Highlight the character and move to the next char
            _edit_name[i]->setStyleSheet("QLabel { background-color: " + utils::get_css_system_colour_str() + "; color : black; }");
        }          
        else {
            // Don't Highlight the character and move to the next char
            _edit_name[i]->setStyleSheet("QLabel { color: " + utils::get_css_system_colour_str() + "; }");
        }
    }

    // Refresh the ASCII picker object colours
    for (uint i=0; i<ASCII_PICKER_SIZE; i++) {
        // If this is the middle character
        if (i == 3) {
            // Highlight the character and move to the next char
            _ascii_picker[i]->setStyleSheet("QLabel { background-color: " + utils::get_css_system_colour_str() + "; color : black; }");
        }          
        else {
            // Don't Highlight the character and move to the next char
            _ascii_picker[i]->setStyleSheet("QLabel { color: " + utils::get_css_system_colour_str() + "; }");
        }
    }    
}

//----------------------------------------------------------------------------
// _set_ascii_picker_geometry
//----------------------------------------------------------------------------
void EditName::_set_ascii_picker_geometry()
{
    auto x_offset = _edit_name[0]->x();

    // Set the geometry for each ASCII picker character
    for (uint i=0; i<ASCII_PICKER_SIZE; i++) {
        _ascii_picker[i]->setGeometry((x_offset + (_selected_char*EDIT_NAME_CHAR_WIDTH)),
                                       _ascii_picker[i]->y(),
                                       EDIT_NAME_SEL_CHAR_WIDTH,
                                       EDIT_NAME_CHAR_HEIGHT);
        _ascii_picker[i]->hide();
    }
}
