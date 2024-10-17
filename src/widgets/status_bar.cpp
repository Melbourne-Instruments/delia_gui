/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023-2024 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  status_bar.cpp
 * @brief Status Bar class implementation.
 *-----------------------------------------------------------------------------
 */
#include "status_bar.h"
#include "gui_common.h"
#include "utils.h"

// Constants
constexpr uint LEFT_STATUS_FONT_SIZE         = 35;
constexpr uint TEMPO_STATUS_FONT_SIZE        = 30;
constexpr uint SETUP_STATUS_FONT_SIZE        = 35;
constexpr char MODIFIED_INDICATOR[]          = "*";
constexpr uint MARGIN_LEFT                   = 20;
constexpr uint MARGIN_RIGHT                  = 20;
constexpr uint TWELEVE_VOICE_MODE_MARGIN_TOP = 3;
constexpr uint LAYER_STATUS_MARGIN_TOP       = 0;
constexpr uint TEMPO_STATUS_MARGIN_TOP       = 4;
constexpr uint SPACING                       = 10;

//----------------------------------------------------------------------------
// StatusBar
//----------------------------------------------------------------------------
StatusBar::StatusBar(QWidget *parent) : MoniqLabel(parent)
{
    // 12 voice mode off by default
    _left_status_text = "";
    _preset_modified = false;
    _12v_mode = false;

    // Create the left status object
    _left_status = new MoniqLabel(parent);
	_left_status->setFont(QFont(STANDARD_FONT_NAME, LEFT_STATUS_FONT_SIZE));
    _left_status->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    _left_status->setText("----");
    _left_status->adjustSize();
    _left_status->show();

    // Create the preset status object
    _preset_status = new MoniqLabel(parent);
	_preset_status->setFont(QFont(STANDARD_FONT_NAME, SETUP_STATUS_FONT_SIZE));
    _preset_status->setAlignment(Qt::AlignCenter);
    _preset_status->setText(MODIFIED_INDICATOR);
    _preset_status->adjustSize();
    _preset_status->hide();

    // Create the Layer voice status objects
    _12v_mode_status = new MoniqLabel(parent);
    auto image = QPixmap(TWELVE_VOICE_STATUS_PNG_RES);
    image = image.scaled(50,60);
    _12v_mode_status->setPixmap(image);
    _12v_mode_status->adjustSize();
    _12v_mode_status->hide();  
    _l1_voice_status = new MoniqLabel(parent);
    image = QPixmap(SIX_VOICE_ACTIVE_LAYER_PNG_RES);
    image = image.scaled(50,60);
    _l1_voice_status->setPixmap(image);
    _l1_voice_status->adjustSize();
    _l1_voice_status->hide();
    _l2_voice_status = new MoniqLabel(parent);
    image = QPixmap(ZERO_VOICE_INACTIVE_LAYER_PNG_RES);
    image = image.scaled(50,60);
    _l2_voice_status->setPixmap(image);
    _l2_voice_status->adjustSize();
    _l2_voice_status->hide();

    // Create the Tempo status object
    _tempo_status = new MoniqLabel(this);
	_tempo_status->setFont(QFont(PARAM_VALUE_FONT_NAME, TEMPO_STATUS_FONT_SIZE));
    _tempo_status->setAlignment(Qt::AlignCenter);
    _tempo_status->setText("000.0");
    _tempo_status->adjustSize();
    _tempo_status->show();    

    // Setup the status bar parameters
	refresh_colour();
    show();
}

//----------------------------------------------------------------------------
// setGeometry
//----------------------------------------------------------------------------
void StatusBar::setGeometry(int x, int y, int w, int h)
{
    // Set the status bar geometry
    MoniqLabel::setGeometry(x, y, w, h);

    // Set the left status geometry
    _left_status->setGeometry ((x + MARGIN_LEFT), y, _left_status->width(), h);

    // Set the tempo status geometry
    _tempo_status->setGeometry ((w - _tempo_status->width() - MARGIN_RIGHT), 
                                (y + TEMPO_STATUS_MARGIN_TOP), 
                                _tempo_status->width(), 
                                _tempo_status->height());

    // Set the Layer voice status status geometry
    _l2_voice_status->setGeometry ((w - _tempo_status->width() - SPACING - _l2_voice_status->width() - MARGIN_RIGHT), 
                                   (y + LAYER_STATUS_MARGIN_TOP), 
                                   _l2_voice_status->width(),
                                   _l2_voice_status->height());
    _l1_voice_status->setGeometry ((w - _tempo_status->width() - SPACING - _l2_voice_status->width() - _l1_voice_status->width() - MARGIN_RIGHT), 
                                   (y + LAYER_STATUS_MARGIN_TOP), 
                                   _l1_voice_status->width(),
                                   _l1_voice_status->height());
    _12v_mode_status->setGeometry ((w - _tempo_status->width() - SPACING - _l2_voice_status->width() - _l1_voice_status->width() - _12v_mode_status->width() - MARGIN_RIGHT), 
                                   (y + TWELEVE_VOICE_MODE_MARGIN_TOP), 
                                   _12v_mode_status->width(),
                                   _12v_mode_status->height());                                                              
}

//----------------------------------------------------------------------------
// set_left_status
//----------------------------------------------------------------------------
void StatusBar::set_left_status(const QString &status, bool modified)
{
    int twelve_note_mode_width = _12v_mode ? _12v_mode_status->width() : 0;
    auto max_width = (width() - MARGIN_LEFT - SPACING - twelve_note_mode_width - _l2_voice_status->width() - _l1_voice_status->width() - SPACING - _tempo_status->width() - MARGIN_RIGHT);

    // Has the setup been modified?
    if (modified) {
        // Show the setup status indicator
        _preset_status->show();
        max_width -= _preset_status->width();
    }
    else {
        // Hide the setup status indicator
        _preset_status->hide();
    }

    // Show the left status
    _left_status->set_text(status, max_width);
    _left_status->setGeometry (MARGIN_LEFT, 
                               y(), 
                               _left_status->width(), 
                               height());
    
    // If the setup was modified, set the setup geometry
    if (modified) {
        _preset_status->setGeometry (MARGIN_LEFT + _left_status->width(), 
                                     y(), 
                                     _preset_status->width(), 
                                     height());
    }

    // Save the status text and preset modified state in case we need to update the left status
    _left_status_text = status;
    _preset_modified = modified;
}

//----------------------------------------------------------------------------
// set_layer_status
//----------------------------------------------------------------------------
void StatusBar::set_layer_status(Layer current_layer, bool twelve_note_mode, uint l1_voices, uint l2_voices)
{
    QString path;
    bool twelve_note_mode_changed = _12v_mode != twelve_note_mode;

    // Set the 12 voice mode
    _12v_mode = twelve_note_mode;

    // Was the 12 voice mode changed?
    if (twelve_note_mode_changed) {
        set_left_status(_left_status_text, _preset_modified);
    }

    _12v_mode ?
        _12v_mode_status->show() :
        _12v_mode_status->hide();

    // Set the L1 status based on the number of voices
    auto image = QPixmap(_get_layer_status_pixmap_path((current_layer == Layer::LAYER_1), l1_voices));
    image = image.scaled(50,60);
    _l1_voice_status->setPixmap(image);
    _l1_voice_status->show();

    // Set the L2 status based on the number of voices
    image = QPixmap(_get_layer_status_pixmap_path((current_layer == Layer::LAYER_2), l2_voices));
    image = image.scaled(50,60);
    _l2_voice_status->setPixmap(image);
    _l2_voice_status->show();
}

//----------------------------------------------------------------------------
// set_tempo
//----------------------------------------------------------------------------
void StatusBar::set_tempo(const QString& tempo)
{
    // Set the tempo
    _tempo_status->setText(tempo);
}

//----------------------------------------------------------------------------
// refresh_colour
//----------------------------------------------------------------------------
void StatusBar::refresh_colour()
{
    // Refresh the status bar object colours
	_left_status->setStyleSheet("QLabel { background-color: " + utils::get_css_system_colour_str() + "; color : black; }");
    _preset_status->setStyleSheet("QLabel { background-color: " + utils::get_css_system_colour_str() + "; color : black; }");
    _tempo_status->setStyleSheet("QLabel { background-color: " + utils::get_css_system_colour_str() + "; color : black; }"); 
	setStyleSheet("QLabel { background-color: " + utils::get_css_system_colour_str() + "; color : black; }"); 
}

//----------------------------------------------------------------------------
// _get_layer_status_pixmap_path
//----------------------------------------------------------------------------
QString StatusBar::_get_layer_status_pixmap_path(bool current_layer, uint num_voices)
{
    QString path;

    // Get the pixmap path based on the number of voices
    switch (num_voices) {
        case 0:
            path = current_layer ? ZERO_VOICE_ACTIVE_LAYER_PNG_RES : ZERO_VOICE_INACTIVE_LAYER_PNG_RES;
            break;
        
        case 1:
            path = current_layer ? ONE_VOICE_ACTIVE_LAYER_PNG_RES : ONE_VOICE_INACTIVE_LAYER_PNG_RES;
            break;

        case 2:
            path = current_layer ? TWO_VOICE_ACTIVE_LAYER_PNG_RES : TWO_VOICE_INACTIVE_LAYER_PNG_RES;
            break;

        case 3:
            path = current_layer ? THREE_VOICE_ACTIVE_LAYER_PNG_RES : THREE_VOICE_INACTIVE_LAYER_PNG_RES;
            break;

        case 4:
            path = current_layer ? FOUR_VOICE_ACTIVE_LAYER_PNG_RES : FOUR_VOICE_INACTIVE_LAYER_PNG_RES;
            break;

        case 5:
            path = current_layer ? FIVE_VOICE_ACTIVE_LAYER_PNG_RES : FIVE_VOICE_INACTIVE_LAYER_PNG_RES;
            break;

        case 6:
            path = current_layer ? SIX_VOICE_ACTIVE_LAYER_PNG_RES : SIX_VOICE_INACTIVE_LAYER_PNG_RES;
            break;
    }
    return path;
}