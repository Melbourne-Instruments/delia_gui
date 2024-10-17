/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  moniq_label.h
 * @brief Moniq Label class definitions.
 *-----------------------------------------------------------------------------
 */
#ifndef MONIQ_LABEL_H
#define MONIQ_LABEL_H

#include <QLabel>

// Moniq Label class
class MoniqLabel: public QLabel
{
    Q_OBJECT
public:
    // Constructors
    explicit MoniqLabel(QWidget *parent = nullptr);

    // Public functions
    virtual void set_text(const QString& text, int max_width);
    virtual void refresh_colour();
};

#endif
