/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023-2024 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  main_list.h
 * @brief Main List widget class definitions.
 *-----------------------------------------------------------------------------
 */
#ifndef MAIN_LIST_H
#define MAIN_LIST_H

#include <QListWidget>
#include <QString>
#include "moniq_label.h"

// Main List class
class MainList: public QListWidget
{
    Q_OBJECT
public:
    // Constructors
    explicit MainList(uint num_rows, QWidget *parent = nullptr);

    // Public functions
    void add_item(const QString& text, bool selected, bool enabled, bool separator);
    void select_item(uint index);
    void refresh_colour();

private:
    // Private data
    uint _num_rows;
    std::vector<std::string> _list_items;
    std::vector<bool> _list_items_enabled;
    std::vector<bool> _list_items_separator;
    MoniqLabel *_dummy_label;

    // Private functions
    void _list_label_set_text(QLabel *label, const QString& text, bool selected, bool enabled, bool separator);
};

#endif
