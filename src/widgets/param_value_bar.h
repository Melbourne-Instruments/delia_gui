/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2024 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  param_value_bar.h
 * @brief Param Value Bar class definitions.
 *-----------------------------------------------------------------------------
 */
#ifndef PARAM_VALUE_BAR_H
#define PARAM_VALUE_BAR_H

#include <QString>
#include "moniq_label.h"
#include "gui_msg.h"

// Param Value Bar class
class ParamValueBar: public MoniqLabel
{
    Q_OBJECT
public:
    // Constructors
    explicit ParamValueBar(QWidget *parent = nullptr);

    // Overriden functions
    void setGeometry(int x, int y, int w, int h);
    void show();
    void hide();

    // Public functions
    void set_param_name(const QString &name);
    void set_param_value(const QString& value);
    void refresh_colour();

private:
    // Private data
    MoniqLabel *_param_name;
    MoniqLabel *_param_value;
};

#endif  // PARAM_VALUE_BAR_H
