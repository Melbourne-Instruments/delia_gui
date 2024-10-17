/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023-2024 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  status_bar.h
 * @brief Status Bar class definitions.
 *-----------------------------------------------------------------------------
 */
#ifndef STATUS_BAR_H
#define STATUS_BAR_H

#include <QString>
#include "moniq_label.h"
#include "gui_msg.h"

// Status Bar class
class StatusBar: public MoniqLabel
{
    Q_OBJECT
public:
    // Constructors
    explicit StatusBar(QWidget *parent = nullptr);

    // Overriden functions
    void setGeometry(int x, int y, int w, int h);

    // Public functions
    void set_left_status(const QString &status, bool modified=false);
    void set_layer_status(Layer current_layer, bool twelve_note_mode, uint l1_voices, uint l2_voices);
    void set_tempo(const QString& tempo);    
    void refresh_colour();

private:
    // Private data
    MoniqLabel *_left_status;
    MoniqLabel *_preset_status;
    MoniqLabel *_12v_mode_status;
    MoniqLabel *_l1_voice_status;
    MoniqLabel *_l2_voice_status;
    MoniqLabel *_tempo_status;
    QString _left_status_text;
    bool _preset_modified;
    bool _12v_mode;

    // Private functions
    QString _get_layer_status_pixmap_path(bool current_layer, uint num_voices);
};

#endif
