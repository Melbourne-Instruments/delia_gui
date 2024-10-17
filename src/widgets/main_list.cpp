/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023-2024 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  main_list.cpp
 * @brief Main List class implementation.
 *-----------------------------------------------------------------------------
 */
#include "main_list.h"
#include "gui_common.h"
#include "utils.h"

// Constants
constexpr uint LIST_FONT_SIZE         = 30;
constexpr uint LIST_ITEM_MARGIN_LEFT  = 20;
constexpr uint LIST_ITEM_MARGIN_RIGHT = 20;

//----------------------------------------------------------------------------
// StatusBar
//----------------------------------------------------------------------------
MainList::MainList(uint num_rows, QWidget *parent) : QListWidget(parent)
{
    // Create the dummy label - not visible
    // This is used to calculate max string lengths
    _dummy_label = new MoniqLabel(this);
	_dummy_label->setFont(QFont(STANDARD_FONT_NAME, LIST_FONT_SIZE)); 
    _dummy_label->setVisible(false);

    // Setup the list object
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFont(QFont(STANDARD_FONT_NAME, LIST_FONT_SIZE));

    // Set the initial list colour
	refresh_colour();
    hide();
    _num_rows = num_rows;
}

//----------------------------------------------------------------------------
// add_item
//----------------------------------------------------------------------------
void MainList::add_item(const QString& text, bool selected, bool enabled, bool separator)
{
    // Firstly set the text we can display, truncated if necessary
    // We do this with the dummy QLabel object  
    _dummy_label->set_text(text, (width() - LIST_ITEM_MARGIN_LEFT - LIST_ITEM_MARGIN_RIGHT));

    // Add this text item to the list, and resize it
    auto label = new QLabel(this);
    auto item = new QListWidgetItem(this);
    label->setFont(QFont(STANDARD_FONT_NAME, LIST_FONT_SIZE));
    _list_label_set_text(label, _dummy_label->text(), selected, enabled, separator);
    item->setSizeHint(QSize(width(), (height() / _num_rows)));
    addItem(item);
    setItemWidget(item, label);

    // Save this list item
    _list_items.push_back(_dummy_label->text().toStdString());
    _list_items_enabled.push_back(enabled);
    _list_items_separator.push_back(separator);
}

//----------------------------------------------------------------------------
// select_item
//----------------------------------------------------------------------------
void MainList::select_item(uint index)
{
    bool selected_item_enabled;

    // Check the selected item is within range
    if (index < (uint)count()) {
        // Its within range, so select that row
        setCurrentRow(index);
        
        // Trick to make QT scroll LOGICALLY and CONSISTENTLY (duh!)
        // Note this assumes that there are 6 items shown in the list
        uint i = index;
        if (i < 4)
            i = 0;
        if (i == 4)
            i = 3;
        scrollToItem(item(i), QAbstractItemView::PositionAtCenter);

        // Update each list item
        for (uint i=0; i<(uint)count(); i++) {
            auto obj = reinterpret_cast<QLabel *>(itemWidget(item(i)));
            _list_label_set_text(obj, _list_items[i].c_str(), (i == index), _list_items_enabled[i], _list_items_separator[i]);
        }
    }
}

//----------------------------------------------------------------------------
// _list_label_set_text
//----------------------------------------------------------------------------
void MainList::_list_label_set_text(QLabel *label, const QString& text, bool selected, bool enabled, bool separator)
{
    QString fontColour = utils::get_css_system_colour_str() + ";";
    QString bgColour = "transparent;";
    QString paddingTop = "0px;";
    QString borderStyle = "solid;";
    QString borderWidth = "0.5px;";
    QString borderColour = utils::get_css_system_colour_str() + ";";

    // Set the param list label text depending on the state of this list item
    if (selected) {
        if (!enabled) {
            // Show the text dimmed
            fontColour = utils::get_dimmed_css_system_colour_str();
        }
        else {
            fontColour = "black;";
        }
        bgColour = utils::get_css_system_colour_str() + ";";
        if (separator) {
            borderColour = "black;";
        }            
    }
    else {
        if (!enabled) {
            // Show the text dimmed
            fontColour = utils::get_dimmed_css_system_colour_str();
        }               
    }
    if (separator) {
        paddingTop = "2.0px;";
        borderStyle = "dotted;";
        borderWidth = "3.0px;";
    }        
    label->setText(text);
    label->setStyleSheet(
        "QLabel { color:" + fontColour + "; background-color:" + bgColour + 
        "padding-left: 20px; padding-right: 10px; padding-top:" + paddingTop +
        "border-style:" + borderStyle + "border-bottom-width:" + borderWidth + "border-color:" + borderColour + "; }");
}

//----------------------------------------------------------------------------
// refresh_colour
//----------------------------------------------------------------------------
void MainList::refresh_colour()
{
    // Refresh the object colours
	setStyleSheet(
        "QListWidget { background-color: black; color : " + utils::get_css_system_colour_str() + "; }"
        "QListWidget::item::selected { color: black; background: " + utils::get_css_system_colour_str() + "; }");
}
