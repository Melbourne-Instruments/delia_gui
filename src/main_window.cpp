/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023-2024 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  main_window.cpp
 * @brief Main Window class implementation.
 *-----------------------------------------------------------------------------
 */
#include <QCoreApplication>
#include <QFontDatabase>
#include <QPainter>
#include <unistd.h>
#include <filesystem>
#include "main_window.h"
#include "gui_common.h"
#include "utils.h"
#include "version.h"

// Constants
constexpr uint VISIBLE_LCD_MARGIN_LEFT      = 9;
constexpr uint VISIBLE_LCD_MARGIN_RIGHT     = 1;
constexpr uint VISIBLE_LCD_WIDTH            = (LCD_WIDTH - VISIBLE_LCD_MARGIN_LEFT - VISIBLE_LCD_MARGIN_RIGHT);
constexpr uint VISIBLE_LCD_MARGIN_TOP       = 5;
constexpr uint VISIBLE_LCD_MARGIN_BOTTOM    = 5;
constexpr uint VISIBLE_LCD_HEIGHT           = (LCD_HEIGHT - VISIBLE_LCD_MARGIN_TOP - VISIBLE_LCD_MARGIN_BOTTOM);
constexpr uint STATUS_BAR_MARGIN_LEFT       = VISIBLE_LCD_MARGIN_LEFT;
constexpr uint STATUS_BAR_MARGIN_TOP        = VISIBLE_LCD_MARGIN_TOP;
constexpr uint STATUS_BAR_WIDTH             = VISIBLE_LCD_WIDTH;
constexpr uint STATUS_BAR_HEIGHT            = 60;
constexpr uint MULTI_STATUS_BAR_MARGIN_LEFT = VISIBLE_LCD_MARGIN_LEFT;
constexpr uint MULTI_STATUS_BAR_MARGIN_TOP  = (STATUS_BAR_MARGIN_TOP + STATUS_BAR_HEIGHT + 3);
constexpr uint MULTI_STATUS_BAR_WIDTH       = VISIBLE_LCD_WIDTH;
constexpr uint MULTI_STATUS_BAR_HEIGHT      = 46;
constexpr uint MAIN_AREA_MARGIN_LEFT        = VISIBLE_LCD_MARGIN_LEFT;
constexpr uint MAIN_AREA_MARGIN_TOP         = (STATUS_BAR_MARGIN_TOP + STATUS_BAR_HEIGHT);
constexpr uint MAIN_AREA_WIDTH              = VISIBLE_LCD_WIDTH;
constexpr uint MAIN_AREA_HEIGHT             = 338;
constexpr uint BOTTOM_BAR_MARGIN_LEFT       = VISIBLE_LCD_MARGIN_LEFT;
constexpr uint BOTTOM_BAR_MARGIN_TOP        = (MAIN_AREA_MARGIN_TOP + MAIN_AREA_HEIGHT);
constexpr uint BOTTOM_BAR_WIDTH             = VISIBLE_LCD_WIDTH;
constexpr uint BOTTOM_BAR_HEIGHT            = (VISIBLE_LCD_HEIGHT - BOTTOM_BAR_MARGIN_TOP);
constexpr uint LIST_MARGIN_LEFT             = VISIBLE_LCD_MARGIN_LEFT;
constexpr uint LIST_MARGIN_TOP              = (STATUS_BAR_MARGIN_TOP + STATUS_BAR_HEIGHT + 1);
constexpr uint LIST_WIDTH                   = VISIBLE_LCD_WIDTH;
constexpr uint LIST_ROW_HEIGHT              = 48;
constexpr uint NUM_LIST_ROWS                = 7;
constexpr uint LIST_HEIGHT                  = (7 * LIST_ROW_HEIGHT);
constexpr uint LIST_FONT_SIZE               = 30;
constexpr uint PARAM_LIST_MARGIN_TOP        = (STATUS_BAR_MARGIN_TOP + STATUS_BAR_HEIGHT + 3);
constexpr uint PARAM_LIST_WIDTH             = (VISIBLE_LCD_WIDTH/2);
constexpr uint PARAM_VALUE_MARGIN_LEFT      = 10;
constexpr uint PARAM_VALUE_MARGIN_LEFT_RLO  = (VISIBLE_LCD_MARGIN_LEFT + 10);
constexpr uint PARAM_VALUE_MARGIN_RIGHT     = PARAM_LIST_WIDTH + 10;
constexpr uint PARAM_VALUE_MARGIN_RIGHT_RLO = 10;
constexpr uint PARAM_VALUE_WIDTH            = (VISIBLE_LCD_WIDTH - PARAM_VALUE_MARGIN_RIGHT - PARAM_VALUE_MARGIN_LEFT);
constexpr uint PARAM_VALUE_NUM_FONT_SIZE    = 80;
constexpr uint PARAM_VALUE_TXT_FONT_SIZE    = 36;
constexpr uint PARAM_VALUE_TAG_FONT_SIZE    = 30;
constexpr uint WT_LIST_WIDTH                = (VISIBLE_LCD_WIDTH/2);
constexpr uint WT_CHART_MARGIN_LEFT         = (LIST_MARGIN_LEFT + WT_LIST_WIDTH + 10);
constexpr uint WT_CHART_MARGIN_TOP          = (LIST_MARGIN_TOP + 10);
constexpr uint WT_CHART_WIDTH               = (VISIBLE_LCD_WIDTH - WT_LIST_WIDTH - 20);
constexpr uint WT_CHART_HEIGHT              = LIST_HEIGHT - 20;
constexpr uint EG_STD_MARGIN_LEFT           = (VISIBLE_LCD_WIDTH - 600) / 2 ;
constexpr uint EG_STD_MARGIN_TOP            = MULTI_STATUS_BAR_MARGIN_TOP + MULTI_STATUS_BAR_HEIGHT + 10;
constexpr uint EG_STD_WIDTH                 = 600;
constexpr uint EG_STD_HEIGHT                = MAIN_AREA_HEIGHT - MULTI_STATUS_BAR_HEIGHT - 20;
constexpr uint EG_LST_MARGIN_LEFT           = (LIST_MARGIN_LEFT + PARAM_LIST_WIDTH + 10);
constexpr uint EG_LST_MARGIN_TOP            = LIST_MARGIN_TOP + 10;
constexpr uint EG_LST_WIDTH                 = (VISIBLE_LCD_WIDTH - PARAM_LIST_WIDTH - 20);
constexpr uint EG_LST_HEIGHT                = (LIST_HEIGHT / 2) - 20;
constexpr uint VCF_CUTOFF_MARGIN_LEFT       = (LIST_MARGIN_LEFT + PARAM_LIST_WIDTH + 10);
constexpr uint VCF_CUTOFF_MARGIN_TOP        = LIST_MARGIN_TOP + 10;
constexpr uint VCF_CUTOFF_WIDTH             = (VISIBLE_LCD_WIDTH - PARAM_LIST_WIDTH - 20);
constexpr uint VCF_CUTOFF_HEIGHT            = LIST_HEIGHT - 20;
constexpr uint SOUND_SCOPE_MARGIN_TOP       = (MAIN_AREA_MARGIN_TOP + 15);
constexpr uint SOUND_SCOPE_HEIGHT           = (MAIN_AREA_HEIGHT - 15);
constexpr uint OSC_SOUND_SCOPE_WIDTH        = VISIBLE_LCD_WIDTH;
constexpr uint XY_SOUND_SCOPE_WIDTH         = SOUND_SCOPE_HEIGHT;
constexpr uint OSC_SOUND_SCOPE_MARGIN_LEFT  = MAIN_AREA_MARGIN_LEFT;
constexpr uint XY_SOUND_SCOPE_MARGIN_LEFT   = MAIN_AREA_MARGIN_LEFT + ((VISIBLE_LCD_WIDTH - XY_SOUND_SCOPE_WIDTH) / 2);

//----------------------------------------------------------------------------
// MainWindow
//----------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // Register the meta types
    qRegisterMetaType<SetLeftStatusMsg>();
    qRegisterMetaType<SetLayerStatusMsg>();
    qRegisterMetaType<SetMidiStatusMsg>();
    qRegisterMetaType<SetTempoStatusMsg>();
    qRegisterMetaType<ShowHomeScreenMsg>();
    qRegisterMetaType<ShowListItemsMsg>();
    qRegisterMetaType<ListSelectItemMsg>();
    qRegisterMetaType<SelectLayerNameMsg>();
    qRegisterMetaType<SetSoftButtonsTextMsg>();
    qRegisterMetaType<SetSoftButtonsStateMsg>();
    qRegisterMetaType<ShowNormalParamMsg>();
    qRegisterMetaType<ShowNormalParamUpdateMsg>();
    qRegisterMetaType<ShowEnumParamUpdateMsg>();
    qRegisterMetaType<ShowEditNameMsg>();
    qRegisterMetaType<EditNameSelectCharMsg>();
    qRegisterMetaType<EditNameChangeCharMsg>();
    qRegisterMetaType<ShowMsgBoxMsg>();
    qRegisterMetaType<ShowMsgPopupMsg>();
    qRegisterMetaType<SetSystemColour>();
    qRegisterMetaType<ShowAdsrEnvParamMsg>();
    qRegisterMetaType<ShowAdsrEnvParamUpdateMsg>();
    qRegisterMetaType<ShowVcfCutoffParamMsg>();
    qRegisterMetaType<ShowVcfCutoffParamUpdateMsg>();

    // Add the Melbourne Instruments specific fonts
    QFontDatabase::addApplicationFont(OCR_B_FONT_RES);
    QFontDatabase::addApplicationFont(DSEG7_CLASSIC_FONT_RES);

    // Create the GUI objects
    _create_gui_objs();

    // Resize the display for the MONIQUE LCD screen
    resize(LCD_WIDTH, LCD_HEIGHT);    

    // Set the default  sound scope mode to OFF
    _sound_scope_mode = SoundScopeMode::SCOPE_MODE_OFF;

    // Create the thread to process incoming GUI messages from the MONIQUE UI App, and connect
    // to this thread
    _gui_msg_thread = new GuiMsgThread(this);
    connect(_gui_msg_thread, SIGNAL(left_status_msg(SetLeftStatusMsg)), this, SLOT(set_left_status(SetLeftStatusMsg)));
    connect(_gui_msg_thread, SIGNAL(layer_status_msg(SetLayerStatusMsg)), this, SLOT(set_layer_status(SetLayerStatusMsg)));
    connect(_gui_msg_thread, SIGNAL(midi_status_msg(SetMidiStatusMsg)), this, SLOT(set_midi_status(SetMidiStatusMsg)));
    connect(_gui_msg_thread, SIGNAL(tempo_status_msg(SetTempoStatusMsg)), this, SLOT(set_tempo_status(SetTempoStatusMsg)));
    connect(_gui_msg_thread, SIGNAL(home_screen_msg(ShowHomeScreenMsg)), this, SLOT(show_home_screen(ShowHomeScreenMsg)));
    connect(_gui_msg_thread, SIGNAL(list_items_msg(ShowListItemsMsg)), this, SLOT(show_list_items(ShowListItemsMsg)));
    connect(_gui_msg_thread, SIGNAL(list_select_item_msg(ListSelectItemMsg)), this, SLOT(list_select_item(ListSelectItemMsg)));
    connect(_gui_msg_thread, SIGNAL(select_layer_name_msg(SelectLayerNameMsg)), this, SLOT(select_layer_name(SelectLayerNameMsg)));
    connect(_gui_msg_thread, SIGNAL(soft_buttons_text_msg(SetSoftButtonsTextMsg)), this, SLOT(set_soft_buttons_text(SetSoftButtonsTextMsg)));
    connect(_gui_msg_thread, SIGNAL(soft_buttons_state_msg(SetSoftButtonsStateMsg)), this, SLOT(set_soft_buttons_state(SetSoftButtonsStateMsg)));
    connect(_gui_msg_thread, SIGNAL(param_update_msg(ShowNormalParamMsg)), this, SLOT(process_param_update(ShowNormalParamMsg)));
    connect(_gui_msg_thread, SIGNAL(param_value_update_msg(ShowNormalParamUpdateMsg)), this, SLOT(process_param_value_update(ShowNormalParamUpdateMsg)));
    connect(_gui_msg_thread, SIGNAL(enum_param_update_msg(ShowEnumParamUpdateMsg)), this, SLOT(process_enum_param_update(ShowEnumParamUpdateMsg)));
    connect(_gui_msg_thread, SIGNAL(enum_param_value_update_msg(ListSelectItemMsg)), this, SLOT(process_enum_param_value_update(ListSelectItemMsg)));
    connect(_gui_msg_thread, SIGNAL(edit_name_msg(ShowEditNameMsg)), this, SLOT(process_edit_name(ShowEditNameMsg)));
    connect(_gui_msg_thread, SIGNAL(edit_name_select_char_msg(EditNameSelectCharMsg)), this, SLOT(process_edit_name_select_char(EditNameSelectCharMsg)));
    connect(_gui_msg_thread, SIGNAL(edit_name_change_char_msg(EditNameChangeCharMsg)), this, SLOT(process_edit_name_change_char(EditNameChangeCharMsg)));
    connect(_gui_msg_thread, SIGNAL(msg_popup_msg(ShowMsgPopupMsg)), this, SLOT(show_msg_popup(ShowMsgPopupMsg)));
    connect(_gui_msg_thread, SIGNAL(msg_box_msg(ShowMsgBoxMsg)), this, SLOT(show_msg_box(ShowMsgBoxMsg)));
    connect(_gui_msg_thread, SIGNAL(clear_boot_warning_msg()), this, SLOT(clear_boot_warning()));
    connect(_gui_msg_thread, SIGNAL(set_system_colour_msg(SetSystemColour)), this, SLOT(set_system_colour(SetSystemColour)));
    connect(_gui_msg_thread, SIGNAL(show_adsr_envelope_msg(ShowAdsrEnvParamMsg)), this, SLOT(show_adsr_envelope(ShowAdsrEnvParamMsg)));
    connect(_gui_msg_thread, SIGNAL(update_adsr_envelope_msg(ShowAdsrEnvParamUpdateMsg)), this, SLOT(update_adsr_envelope(ShowAdsrEnvParamUpdateMsg)));
    connect(_gui_msg_thread, SIGNAL(show_vcf_cutoff_msg(ShowVcfCutoffParamMsg)), this, SLOT(show_vcf_cutoff(ShowVcfCutoffParamMsg)));
    connect(_gui_msg_thread, SIGNAL(update_vcf_cutoff_msg(ShowVcfCutoffParamUpdateMsg)), this, SLOT(update_vcf_cutoff(ShowVcfCutoffParamUpdateMsg)));
    connect(_gui_msg_thread, SIGNAL(main_screen_capture_msg()), this, SLOT(main_screen_capture()));
    _gui_msg_thread->start();

    // Start the sound scope messages thread
    _sound_scope_msg_thread = new SoundScopeMsgThread(_sound_scope, this);
    _sound_scope_msg_thread->start();
    _screen_capture_index = 1;
}

//----------------------------------------------------------------------------
// ~MainWindow
//----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    // Delete and stop the GUI and sound scope message threads
    delete _gui_msg_thread;
    delete _sound_scope_msg_thread;
    
    // Note QT handles the deletion of allocated objects
}

//----------------------------------------------------------------------------
// set_left_status
//----------------------------------------------------------------------------
void MainWindow::set_left_status(const SetLeftStatusMsg& msg)
{
    // Update the Left status object
    _status_bar->set_left_status(msg.status, msg.modified);
}

//----------------------------------------------------------------------------
// set_layer_status
//----------------------------------------------------------------------------
void MainWindow::set_layer_status(const SetLayerStatusMsg& msg)
{
    // Update the Layer status object
    _status_bar->set_layer_status(msg.current_layer, msg.twelve_note_mode, msg.l1_num_voices, msg.l2_num_voices);
}

//----------------------------------------------------------------------------
// set_midi_status
//----------------------------------------------------------------------------
void MainWindow::set_midi_status(const SetMidiStatusMsg& msg)
{
    // Update the MIDI status
    //(msg.midi_active) ?
    //    _status_bar->midi_status_active() :
    //    _status_bar->midi_status_inactive();
}

//----------------------------------------------------------------------------
// set_tempo_status
//----------------------------------------------------------------------------
void MainWindow::set_tempo_status(const SetTempoStatusMsg& msg)
{
    // Update the Tempo Status
    _status_bar->set_tempo(msg.tempo_value);
}

//----------------------------------------------------------------------------
// show_home_screen
//----------------------------------------------------------------------------
void MainWindow::show_home_screen(const ShowHomeScreenMsg& msg)
{
    // Save the sound scope mode
    _sound_scope_mode = msg.scope_mode;

    // Update the left status
    _status_bar->set_left_status(msg.preset_name, msg.preset_modified);
    _param_value_bar->hide();                            
    _show_default_background(true, true);
    _main_list->hide();
    _bottom_bar->hide();
    _show_param_obj(false);
    _show_enum_param_list_obj(false);
    _eg_chart->hide();
    _vcf_cutoff_chart->hide();
    _edit_name->hide();
    _msg_box->hide(); 
    _multi_status_bar->hide();
}

//----------------------------------------------------------------------------
// show_list_items
//----------------------------------------------------------------------------
void MainWindow::show_list_items(const ShowListItemsMsg& msg)
{
    // If any list items are specified
    if (msg.num_items > 0) {
        // Delete and re-create the main list - we need to do this due to *NUMEROUS* issues with
        // QT and re-assigning a list
        delete _main_list;
        _main_list = new MainList(NUM_LIST_ROWS, this);    
        _main_list->setGeometry (LIST_MARGIN_LEFT, LIST_MARGIN_TOP, LIST_WIDTH, LIST_HEIGHT);
        _main_list->stackUnder(_bottom_bar);

        // Add items to the list
        for (uint i=0; i<msg.num_items; i++) {
            // Special case!
            if (std::strcmp(msg.list_items[i], "GUI_VER") == 0) {
                char sw_ver[40];
                std::sprintf(sw_ver, "GUI: %d.%d.%d-%c%c%c%c%c%c%c", MONIQUE_GUI_MAJOR_VERSION, MONIQUE_GUI_MINOR_VERSION, MONIQUE_GUI_PATCH_VERSION, 
                                                                     MONIQUE_GUI_GIT_COMMIT_HASH[0], MONIQUE_GUI_GIT_COMMIT_HASH[1], MONIQUE_GUI_GIT_COMMIT_HASH[2],
                                                                     MONIQUE_GUI_GIT_COMMIT_HASH[3], MONIQUE_GUI_GIT_COMMIT_HASH[4], MONIQUE_GUI_GIT_COMMIT_HASH[5],
                                                                     MONIQUE_GUI_GIT_COMMIT_HASH[6]);                
                _main_list->add_item(sw_ver, false, true, false);
            }
            else {
                !msg.process_enabled_state ?
                    _main_list->add_item(msg.list_items[i], (i == msg.selected_item), true, msg.list_item_separator[i]) :
                    _main_list->add_item(msg.list_items[i], (i == msg.selected_item), msg.list_item_enabled[i], msg.list_item_separator[i]);
            }
        }
        if (msg.selected_item != -1) {
            _main_list->select_item(msg.selected_item);
        }

        // Show the list
        _main_list->show();
        _multi_status_bar->hide();
        _show_default_background(false, false);
        _show_param_obj(false);
        _show_enum_param_list_obj(false);
        _edit_name->hide();        
    }
}

//----------------------------------------------------------------------------
// list_select_item
//----------------------------------------------------------------------------
void MainWindow::list_select_item(const ListSelectItemMsg& msg)
{
    // Select the list item
    _main_list->select_item(msg.selected_item);
}

//----------------------------------------------------------------------------
// select_layer_name
//----------------------------------------------------------------------------
void MainWindow::select_layer_name(const SelectLayerNameMsg& msg)
{
    // Select the layer name in the multi status bar
    _multi_status_bar->select_layer(msg.selected_layer);
}

//----------------------------------------------------------------------------
// set_soft_buttons_text
//----------------------------------------------------------------------------
void MainWindow::set_soft_buttons_text(const SetSoftButtonsTextMsg& msg)
{
    // If the soft buttons text is undefined
    if ((std::strlen(msg.button1_text) == 0) && (std::strlen(msg.button2_text) == 0)) {
        // Hide the bottom bar
        _bottom_bar->hide();
    }
    else {
        // Update the soft button text
        _bottom_bar->set_soft_button_text(SoftButtonId::BUTTON_1, msg.button1_text);
        _bottom_bar->set_soft_button_text(SoftButtonId::BUTTON_2, msg.button2_text);
        _bottom_bar->show();     
    }
}

//----------------------------------------------------------------------------
// set_soft_buttons_state
//----------------------------------------------------------------------------
void MainWindow::set_soft_buttons_state(const SetSoftButtonsStateMsg& msg)
{
    // Update the soft buttons state
    _bottom_bar->set_soft_button_state(SoftButtonId::BUTTON_1, msg.button1_state);
    _bottom_bar->set_soft_button_state(SoftButtonId::BUTTON_2, msg.button2_state);
}

//----------------------------------------------------------------------------
// process_param_update
//----------------------------------------------------------------------------
void MainWindow::process_param_update(const ShowNormalParamMsg& msg)
{
    bool show_background = false;

    // Are we just showing a single param and not force show a list?
    if ((msg.num_items == 0) && !msg.force_show_list) {
        // Was a display string specified? If so - show this as the value
        QString value_str = "";
        if (std::strlen(msg.display_string) > 0) {
            // Show the param value as a text string
            value_str = msg.display_string;           
        }
        else {
            // Show the param value as a numerical value
            value_str = msg.value_string;
        }
        
        // If there is a value tag, add it to the string
        if (std::strlen(msg.value_tag) > 0) {
            value_str += " ";
            value_str += msg.value_tag;
        }
        
        // Show the param name and value
        _param_value_bar->set_param_name(msg.name);
        _param_value_bar->set_param_value(value_str);
        _param_value_bar->show();

        if (_enum_param_list->isVisible() || _wt_enum_param_list->isVisible()) {
            _show_enum_param_list_obj(false);
        }
        _show_param_obj(false);
        _params_list->clear();
        _list_items.clear();
        _list_items_enabled.clear();
        _list_items_separator.clear();
        _multi_status_bar->hide();
        show_background = true;
    }
    else {
        // Update the Left status object
        _status_bar->set_left_status(msg.name);

        // Show the multi status bar if the sound names have been specified
        uint list_margin_top = PARAM_LIST_MARGIN_TOP;
        uint list_height = LIST_HEIGHT;
        uint param_y_adj = 0;
        if ((std::strlen(msg.l1_patch_name) > 0) && (std::strlen(msg.l2_patch_name) > 0)) {
            // Show the multi status bar
            _multi_status_bar->set_l1_sound_name(msg.l1_patch_name);
            _multi_status_bar->set_l2_sound_name(msg.l2_patch_name);
            _multi_status_bar->show();

            // Adjust the param list and value for this bar
            list_margin_top += MULTI_STATUS_BAR_HEIGHT + 2;
            list_height -= MULTI_STATUS_BAR_HEIGHT + 2;
            param_y_adj = MULTI_STATUS_BAR_HEIGHT + 2;
        }
        else {
            // Hide the multi-status bar
            _multi_status_bar->hide();
        }

        // Was a display string specified? If so - show this as the value
        QString str = "";
        if (std::strlen(msg.display_string) > 0) {
            // Show the param value as a text string
            str = msg.display_string;
            _param_value->setFont(QFont(STANDARD_FONT_NAME, PARAM_VALUE_TXT_FONT_SIZE));
            _param_value->setText(str);              
        }
        else {
            // Show the param value as a numerical value
            str = msg.value_string;
            _param_value->setFont(QFont(PARAM_VALUE_FONT_NAME, PARAM_VALUE_NUM_FONT_SIZE));
            _param_value->setText(str);
        }
        _param_value->adjustSize();

        // Set the value geometry
        uint width = _param_value->width();
        while (width > PARAM_VALUE_WIDTH) {
            str = str.mid(0, str.size() - 1);
            QString trunc_str = str + "~";
            _param_value->setText(trunc_str);
            _param_value->adjustSize();
            width = _param_value->width();
        }
        int x = msg.screen_orientation == ScreenOrientation::LEFT_RIGHT ?
                    (VISIBLE_LCD_WIDTH - width - PARAM_VALUE_MARGIN_LEFT) :
                    PARAM_VALUE_MARGIN_LEFT_RLO;
        int y = list_margin_top + ((list_height - _param_value->height()) / 2);
        _param_value->setGeometry (x, y, width, _param_value->height());

        if (std::strlen(msg.value_tag) > 0) {
            str = msg.value_tag;
            _param_value_tag->setText(msg.value_tag);
            _param_value_tag->adjustSize();
            uint width = _param_value_tag->width();
            while (width > PARAM_VALUE_WIDTH) {
                str = str.mid(0, str.size() - 1);
                QString trunc_str = str + "~";
                _param_value_tag->setText(trunc_str);
                _param_value_tag->adjustSize();
                width = _param_value_tag->width();
            }
            x = msg.screen_orientation == ScreenOrientation::LEFT_RIGHT ?
                    (VISIBLE_LCD_WIDTH - _param_value_tag->width() - 20) :        
                    (_param_value_tag->width() < _param_value->width()) ?
                        (PARAM_VALUE_MARGIN_LEFT_RLO + _param_value->width() - _param_value_tag->width()) :
                        PARAM_VALUE_MARGIN_LEFT_RLO;
            y += _param_value->height() + 10;
            _param_value_tag->setGeometry (x, y, _param_value_tag->width(), _param_value_tag->height());
            _param_value_tag->setVisible(true);  
        }
        else {
            _param_value_tag->setText("");
            _param_value_tag->adjustSize();
            _param_value_tag->setVisible(false); 
        }
        if ((uint)_param_value_tag->width() > width)
            width = _param_value_tag->width();

        // Add the items to the list
        // Note: If the list has items, scroll to the start of the list to reset the scroll
        if (_params_list->count()) {
            _params_list->scrollToItem(_params_list->item(0), QAbstractItemView::PositionAtCenter);
        }
        _params_list->clear();
        _list_items.clear();
        _list_items_enabled.clear();
        _list_items_separator.clear();

        for (uint i=0; i<msg.num_items; i++) {
            _label_set_text(_dummy_label, msg.list_items[i], PARAM_LIST_WIDTH-30);
            auto label = new QLabel(this);
            auto item = new QListWidgetItem(_params_list);
            label->setFont(QFont(STANDARD_FONT_NAME, LIST_FONT_SIZE));
            _list_label_set_text(label, _dummy_label->text(), (i == msg.selected_item), msg.list_item_enabled[i], msg.list_item_separator[i]);
            item->setSizeHint(QSize(PARAM_LIST_WIDTH, LIST_ROW_HEIGHT));
            _params_list->addItem(item);
            _params_list->setItemWidget(item, label);
            _list_items.push_back(_dummy_label->text().toStdString());
            _list_items_enabled.push_back(msg.list_item_enabled[i]);
            _list_items_separator.push_back(msg.list_item_separator[i]);
        }

        if (msg.selected_item < msg.num_items)
            _params_list->setCurrentRow(msg.selected_item);
        else
            _params_list->setCurrentRow(0);
        x = msg.screen_orientation == ScreenOrientation::LEFT_RIGHT ?
                LIST_MARGIN_LEFT :
                (PARAM_VALUE_MARGIN_LEFT_RLO + PARAM_VALUE_WIDTH + PARAM_VALUE_MARGIN_RIGHT_RLO);
        _params_list->setGeometry(x, list_margin_top, PARAM_LIST_WIDTH, list_height);
        _params_list->setVisible(true);

        // Trick to make QT scroll LOGICALLY and CONSISTENTLY (duh)
        // Note this assumes that there are 6 items shown in the list
        uint index = msg.selected_item;
        if (index < 4)
            index = 0;
        if (index == 4)
            index = 3;
        _params_list->scrollToItem(_params_list->item(index), QAbstractItemView::PositionAtCenter);

        if (_enum_param_list->isVisible() || _wt_enum_param_list->isVisible()) {
            _show_enum_param_list_obj(false);
        }
        _show_param_obj(true);
        _param_value->setVisible(msg.num_items ? true : false);
        _param_value_bar->hide();    
    }
    if (_default_background->isVisible() || (_sound_scope->display_mode() == ScopeDisplayMode::FOREGROUND)) {
        _show_default_background(show_background, msg.show_scope);
    }
    _main_list->hide();
    _edit_name->hide();
    _eg_chart->hide();
    _vcf_cutoff_chart->hide();
}

//----------------------------------------------------------------------------
// process_param_value_update
//----------------------------------------------------------------------------
void MainWindow::process_param_value_update(const ShowNormalParamUpdateMsg& msg)
{
    // Are we showing this param as a list object?
    if (_params_list->count() == 0) {
        // Was a display string specified? If so - show this as the value
        QString value_str = "";
        if (std::strlen(msg.display_string) > 0) {
            // Show the param value as a text string
            value_str = msg.display_string;           
        }
        else {
            // Show the param value as a numerical value
            value_str = msg.value_string;
        }
        
        // If there is a value tag, add it to the string
        if (std::strlen(msg.value_tag) > 0) {
            value_str += " ";
            value_str += msg.value_tag;
        }
        
        // Show the param value
        _param_value_bar->set_param_name(msg.name);
        _param_value_bar->set_param_value(value_str);
        _param_value_bar->show();
        _show_param_obj(false);
        if (!_default_background->isVisible() && !_sound_scope->isVisible()) {
            _show_default_background(true, true);
        }   
    }
    else {
        // Was a display string specified? If so - show this as the value
        QString str = "";
        if (std::strlen(msg.display_string) > 0) 
        {
            // Show the param value as a text string
            str = msg.display_string;
            _param_value->setFont(QFont(STANDARD_FONT_NAME, PARAM_VALUE_TXT_FONT_SIZE));
            _param_value->setText(str);              
        }
        else 
        {
            // Show the param value as a numerical value
            str = msg.value_string;
            _param_value->setFont(QFont(PARAM_VALUE_FONT_NAME, PARAM_VALUE_NUM_FONT_SIZE));
            _param_value->setText(str);
        }
        _param_value->adjustSize();

        // Set the value geometry
        uint width = _param_value->width();
        while (width > PARAM_VALUE_WIDTH) {
            str = str.mid(0, str.size() - 1);
            QString trunc_str = str + "~";
            _param_value->setText(trunc_str);
            _param_value->adjustSize();
            width = _param_value->width();
        }
        int x = msg.screen_orientation == ScreenOrientation::LEFT_RIGHT ?
                    (VISIBLE_LCD_WIDTH - width - PARAM_VALUE_MARGIN_LEFT) :
                    PARAM_VALUE_MARGIN_LEFT_RLO;
        int y = _params_list->y() + ((_params_list->height() - _param_value->height()) / 2);
        _param_value->setGeometry(x, y, width, _param_value->height());

        if (std::strlen(msg.value_tag) > 0) {
            str = msg.value_tag;
            _param_value_tag->setText(msg.value_tag);
            _param_value_tag->adjustSize();
            uint width = _param_value_tag->width();
            while (width > PARAM_VALUE_WIDTH) {
                str = str.mid(0, str.size() - 1);
                QString trunc_str = str + "~";
                _param_value_tag->setText(trunc_str);
                _param_value_tag->adjustSize();
                width = _param_value_tag->width();
            }
            x = msg.screen_orientation == ScreenOrientation::LEFT_RIGHT ?
                    (VISIBLE_LCD_WIDTH - _param_value_tag->width() - 20) :        
                    (_param_value_tag->width() < _param_value->width()) ?
                        (PARAM_VALUE_MARGIN_LEFT_RLO + _param_value->width() - _param_value_tag->width()) :
                        PARAM_VALUE_MARGIN_LEFT_RLO;
            y += _param_value->height() + 10;
            _param_value_tag->setGeometry (x, y, _param_value_tag->width(), _param_value_tag->height());
            _param_value_tag->setVisible(true);  
        }
        else {
            _param_value_tag->setText("");
            _param_value_tag->adjustSize();
            _param_value_tag->setVisible(false); 
        }

        if ((uint)_param_value_tag->width() > width)
            width = _param_value_tag->width();

        // Check the selected item is within range
        if ((msg.selected_item != -1) && msg.selected_item < _params_list->count()) 
        {
            // Its within range, so select that row
            _params_list->setCurrentRow(msg.selected_item);
            
            // Trick to make QT scroll LOGICALLY and CONSISTENTLY (duh)
            // Note this assumes that there are 6 items shown in the list
            uint index = msg.selected_item;
            if (index < 4)
                index = 0;
            if (index == 4)
                index = 3;
            _params_list->scrollToItem(_params_list->item(index), QAbstractItemView::PositionAtCenter);

            for (int i=0; i<_params_list->count(); i++) {
                auto item = reinterpret_cast<QLabel *>(_params_list->itemWidget(_params_list->item(i)));
                _list_label_set_text(item, _list_items[i].c_str(), (i == msg.selected_item), _list_items_enabled[i], _list_items_separator[i]);
            }
        }
    }

    _eg_chart->hide();
    _vcf_cutoff_chart->hide();
}

//----------------------------------------------------------------------------
// process_enum_param_update
//----------------------------------------------------------------------------
void MainWindow::process_enum_param_update(const ShowEnumParamUpdateMsg& msg)
{
    // Set a pointer to the enum list object to update
    auto enum_param_list = msg.wt_list ? _wt_enum_param_list : _enum_param_list;
    auto list_width = msg.wt_list ? WT_LIST_WIDTH : LIST_WIDTH;
    
    // Update the Left status object
    _status_bar->set_left_status(msg.name);

    // Add the items to the list
    // Note: If the list has items, scroll to the start of the list to reset the scroll
    if (enum_param_list->count()) {
        enum_param_list->scrollToItem(enum_param_list->item(0), QAbstractItemView::PositionAtCenter);
    }
    enum_param_list->clear();
    _enum_list_items.clear();
    for (uint i=0; i<msg.num_items; i++) {
        msg.wt_list ?
            _wt_label_set_text(_dummy_label, msg.list_items[i]) :
            _label_set_text(_dummy_label, msg.list_items[i], (list_width - 30));
        auto label = new QLabel(this);
        auto item = new QListWidgetItem(enum_param_list);
        label->setFont(QFont(STANDARD_FONT_NAME, LIST_FONT_SIZE));
        if (i == msg.selected_item)
            label->setText(_dummy_label->text());
        else
            label->setText("<span style='color: " + utils::get_css_system_colour_str() + "'>" + _dummy_label->text() + "</span>");
        label->adjustSize();
        item->setSizeHint(QSize(list_width, LIST_ROW_HEIGHT));  
        enum_param_list->addItem(item);
        enum_param_list->setItemWidget(item, label);
        _enum_list_items.push_back(msg.list_items[i]);
    }

    if (msg.selected_item < msg.num_items)
        enum_param_list->setCurrentRow(msg.selected_item);
    else
        enum_param_list->setCurrentRow(0);
    enum_param_list->setVisible(true);

    // Trick to make QT scroll LOGICALLY and CONSISTENTLY (duh)
    // Note this assumes that there are 6 items shown in the list
    uint index = msg.selected_item;
    if (index < 4)
        index = 0;
    if (index == 4)
        index = 3;
    enum_param_list->scrollToItem(enum_param_list->item(index), QAbstractItemView::PositionAtCenter);
    _multi_status_bar->hide();
    _show_default_background(false, false);
    _main_list->hide();
    _params_list->setVisible(false);
    _param_value->setVisible(false);
    _param_value_tag->setVisible(false);
    _eg_chart->hide();
    _vcf_cutoff_chart->hide();
    _param_value_bar->hide();

    // Are we showing a WT list?
    if (msg.wt_list) {
        // Get the WT filename
        auto wt_filename = (msg.selected_item < msg.num_items) ? 
                                _enum_list_items[msg.selected_item] :
                                _enum_list_items[0];

        // Load the WT file
        _wt_scope->load_wt_file(wt_filename);
        _wt_scope->show();
        _sound_scope->hide();
    }
    else {
        // Make sure the WT enum param list and chart is not shown
        _wt_scope->unload_wt_file();
        _wt_enum_param_list->setVisible(false);
        QCoreApplication::processEvents();
        _wt_scope->hide();
    }
}

//----------------------------------------------------------------------------
// process_enum_param_value_update
//----------------------------------------------------------------------------
void MainWindow::process_enum_param_value_update(const ListSelectItemMsg& msg)
{
    // Set a pointer to the enum list object to update
    auto enum_param_list = msg.wt_list ? _wt_enum_param_list : _enum_param_list;
    
    // Check the selected item is within range
    if (msg.selected_item < (uint)enum_param_list->count()) 
    {
        // Its within range, so select that row
        enum_param_list->setCurrentRow(msg.selected_item);
        
        // Trick to make QT scroll LOGICALLY and CONSISTENTLY (duh)
        // Note this assumes that there are 6 items shown in the list
        uint index = msg.selected_item;
        if (index < 4)
            index = 0;
        if (index == 4)
            index = 3;
        enum_param_list->scrollToItem(enum_param_list->item(index), QAbstractItemView::PositionAtCenter);

        for (uint i=0; i<(uint)enum_param_list->count(); i++) {
            auto item = reinterpret_cast<QLabel *>(enum_param_list->itemWidget(enum_param_list->item(i)));
            if (i == msg.selected_item)
                item->setText(_enum_list_items[i].c_str());
            else
                item->setText("<span style='color: " + utils::get_css_system_colour_str() + "'>" + QString(_enum_list_items[i].c_str()) + "</span>");
        }

        // Are we showing a WT list?
        if (msg.wt_list) {
            // Load the WT file
            _wt_scope->load_wt_file(_enum_list_items[msg.selected_item]);
            _wt_scope->show();            
        }        
    }
}

//----------------------------------------------------------------------------
// process_edit_name
//----------------------------------------------------------------------------
void MainWindow::process_edit_name(const ShowEditNameMsg& msg)
{
    // Show the edit name
    _edit_name->show_edit_name(msg.name);

    // Hide the other controls
    _show_default_background(false, false);
    _show_param_obj(false);
    _show_enum_param_list_obj(false);
    _main_list->hide();
}

//----------------------------------------------------------------------------
// process_edit_name_select_char
//----------------------------------------------------------------------------
void MainWindow::process_edit_name_select_char(const EditNameSelectCharMsg& msg)
{
    _edit_name->select_edit_name_char(msg.selected_char);
}

//----------------------------------------------------------------------------
// process_edit_name_change_char
//----------------------------------------------------------------------------
void MainWindow::process_edit_name_change_char(const EditNameChangeCharMsg& msg)
{
    _edit_name->change_edit_name_char(msg.change_char);
}

//----------------------------------------------------------------------------
// show_msg_popup
//----------------------------------------------------------------------------
void MainWindow::show_msg_popup(const ShowMsgPopupMsg& msg)
{
    _msg_popup->show_msg(msg.line_1, msg.line_2);   
}

//----------------------------------------------------------------------------
// show_msg_box
//----------------------------------------------------------------------------
void MainWindow::show_msg_box(const ShowMsgBoxMsg& msg)
{
    msg.show ?
        _msg_box->show_msg(msg.line_1, msg.line_2, msg.line_3, msg.show_hourglass) :
        _msg_box->hide(); 
}

//----------------------------------------------------------------------------
// clear_boot_warning
//----------------------------------------------------------------------------
void MainWindow::clear_boot_warning()
{
    // Hide the boot warning background screen and start processing the sound scope
    _boot_warning_background->hide();
    _sound_scope->start();
}

//----------------------------------------------------------------------------
// set_system_colour
//----------------------------------------------------------------------------
void MainWindow::set_system_colour(const SetSystemColour& msg)
{
    // Set the system colour and update the GUI objects
    utils::set_system_colour(msg.colour);
    _set_gui_objs_system_colour();
}

//----------------------------------------------------------------------------
// show_adsr_envelope
//----------------------------------------------------------------------------
void MainWindow::show_adsr_envelope(const ShowAdsrEnvParamMsg& msg)
{
    // Are we showing this param as a list object?
    if (msg.num_items == 0) {
        // Was a display string specified? If so - show this as the value
        QString value_str = "";
        if (std::strlen(msg.display_string) > 0) {
            // Show the param value as a text string
            value_str = msg.display_string;           
        }
        else {
            // Show the param value as a numerical value
            value_str = msg.value_string;
        }
        
        // Show the param name and value
        _param_value_bar->set_param_name(msg.name);
        _param_value_bar->set_param_value(value_str);
        _param_value_bar->show();

        if (_enum_param_list->isVisible() || _wt_enum_param_list->isVisible()) {
            _show_enum_param_list_obj(false);
        }
        _show_param_obj(false);
        _params_list->clear();
        _list_items.clear();
        _list_items_enabled.clear();
        _list_items_separator.clear();

        _eg_chart->setGeometry (EG_STD_MARGIN_LEFT,
                                EG_STD_MARGIN_TOP,
                                EG_STD_WIDTH,
                                EG_STD_HEIGHT);
        _show_default_background(false, false);     
    }
    else {
        // Update the Left status object
        _status_bar->set_left_status(msg.name);

        // Add the items to the list
        // Note: If the list has items, scroll to the start of the list to reset the scroll
        if (_params_list->count()) {
            _params_list->scrollToItem(_params_list->item(0), QAbstractItemView::PositionAtCenter);
        }
        _params_list->clear();
        _list_items.clear();
        _list_items_enabled.clear();
        _list_items_separator.clear();

        for (uint i=0; i<msg.num_items; i++) {
            _label_set_text(_dummy_label, msg.list_items[i], PARAM_LIST_WIDTH-30);
            auto label = new QLabel(this);
            auto item = new QListWidgetItem(_params_list);
            label->setFont(QFont(STANDARD_FONT_NAME, LIST_FONT_SIZE));
            _list_label_set_text(label, _dummy_label->text(), (i == msg.selected_item), true, false); //msg.params.list_item_enabled[i], msg.list_item_separator[i]);
            item->setSizeHint(QSize(PARAM_LIST_WIDTH, LIST_ROW_HEIGHT));
            _params_list->addItem(item);
            _params_list->setItemWidget(item, label);
            _list_items.push_back(_dummy_label->text().toStdString());
            _list_items_enabled.push_back(true); //msg.list_item_enabled[i]);
            _list_items_separator.push_back(false); //(msg.list_item_separator[i]);
        }

        //if (msg.selected_item < msg.num_items)
            //_params_list->setCurrentRow(msg.selected_item);
        //else
            //_params_list->setCurrentRow(0);
        _params_list->setGeometry (LIST_MARGIN_LEFT,
                                PARAM_LIST_MARGIN_TOP, 
                                PARAM_LIST_WIDTH,
                                LIST_HEIGHT);            
        _params_list->setVisible(true);
        _param_value_bar->hide();

        // Show the param value as a numerical value
        QString str = msg.value_string;
        _param_value->setFont(QFont(PARAM_VALUE_FONT_NAME, PARAM_VALUE_NUM_FONT_SIZE));
        _param_value->setText(str);
        _param_value->adjustSize();
        _param_value->show();

        // Set the value geometry
        uint width = _param_value->width();
        while (width > PARAM_VALUE_WIDTH) {
            str = str.mid(0, str.size() - 1);
            QString trunc_str = str + "~";
            _param_value->setText(trunc_str);
            _param_value->adjustSize();
            width = _param_value->width();
        }
        int x = (VISIBLE_LCD_WIDTH - width - PARAM_VALUE_MARGIN_LEFT);
        //int y = STATUS_BAR_HEIGHT + ((VISIBLE_LCD_HEIGHT - STATUS_BAR_HEIGHT - BOTTOM_BAR_HEIGHT - _param_value->height()) / 2);
        int y = _params_list->y() + (_params_list->height() / 2) + (((_params_list->height() / 2) - _param_value->height()) / 2);
        _param_value->setGeometry(x, y, width, _param_value->height());
        _param_value_tag->hide();

        // Trick to make QT scroll LOGICALLY and CONSISTENTLY (duh)
        // Note this assumes that there are 6 items shown in the list
        uint index = msg.selected_item;
        if (index < 4)
            index = 0;
        if (index == 4)
            index = 3;
        _params_list->scrollToItem(_params_list->item(index), QAbstractItemView::PositionAtCenter);
        _eg_chart->setGeometry (EG_LST_MARGIN_LEFT,
                                EG_LST_MARGIN_TOP,
                                EG_LST_WIDTH,
                                EG_LST_HEIGHT);
        _show_default_background(false, true);       
    }

    // Update the EG  chart
    _eg_chart->update_chart(msg.attack, msg.decay, msg.sustain, msg.release, msg.level);

    if (_enum_param_list->isVisible() || _wt_enum_param_list->isVisible()) {
        _show_enum_param_list_obj(false);
    }
    //_show_param_obj(true);
    //_show_default_background(false, false);
    _multi_status_bar->hide();
    _main_list->hide();
    _edit_name->hide();
    _vcf_cutoff_chart->hide();

    // Show the envelope view
    _eg_chart->show();
}

//----------------------------------------------------------------------------
// update_adsr_envelope
//----------------------------------------------------------------------------
void MainWindow::update_adsr_envelope(const ShowAdsrEnvParamUpdateMsg& msg)
{
    // Update the EG  chart   
    _eg_chart->update_chart(msg.attack, msg.decay, msg.sustain, msg.release, msg.level);   

    // Check the selected item is within range
    if ((msg.selected_item != -1) && msg.selected_item < _params_list->count()) {
        QString str = msg.value_string;
        if (_param_value->font().pointSize() != PARAM_VALUE_NUM_FONT_SIZE) {
            _param_value->setFont(QFont(PARAM_VALUE_FONT_NAME, PARAM_VALUE_NUM_FONT_SIZE));
        }
        _param_value->setText(str);
        _param_value->adjustSize();

        int x = (VISIBLE_LCD_WIDTH - _param_value->width() - PARAM_VALUE_MARGIN_LEFT);
        int y = _params_list->y() + (_params_list->height() / 2) + (((_params_list->height() / 2) - _param_value->height()) / 2);
        _param_value->setGeometry(x, y, _param_value->width(), _param_value->height());

        // Its within range, so select that row
        _params_list->setCurrentRow(msg.selected_item);
        
        // Trick to make QT scroll LOGICALLY and CONSISTENTLY (duh)
        // Note this assumes that there are 6 items shown in the list
        uint index = msg.selected_item;
        if (index < 4)
            index = 0;
        if (index == 4)
            index = 3;
        _params_list->scrollToItem(_params_list->item(index), QAbstractItemView::PositionAtCenter);

        for (int i=0; i<_params_list->count(); i++) {
            auto item = reinterpret_cast<QLabel *>(_params_list->itemWidget(_params_list->item(i)));
            _list_label_set_text(item, _list_items[i].c_str(), (i == msg.selected_item), _list_items_enabled[i], _list_items_separator[i]);
        }

        // Set the ADSR geometry if needed
        if (_eg_chart->x() != EG_LST_MARGIN_LEFT) {
            _eg_chart->setGeometry (EG_LST_MARGIN_LEFT,
                                    EG_LST_MARGIN_TOP,
                                    EG_LST_WIDTH,
                                    EG_LST_HEIGHT);
        }      
    }
    else {
        // Was a display string specified? If so - show this as the value
        QString value_str = "";
        if (std::strlen(msg.display_string) > 0) {
            // Show the param value as a text string
            value_str = msg.display_string;           
        }
        else {
            // Show the param value as a numerical value
            value_str = msg.value_string;
        }
        
        // Update the param value
        _param_value_bar->set_param_name(msg.name);
        _param_value_bar->set_param_value(value_str);
        _param_value_bar->show();

        // Set the ADSR geometry if needed
        if (_eg_chart->x() != EG_STD_MARGIN_LEFT) {
            _eg_chart->setGeometry (EG_STD_MARGIN_LEFT,
                                    EG_STD_MARGIN_TOP,
                                    EG_STD_WIDTH,
                                    EG_STD_HEIGHT);
        }
        _show_default_background(false, false);
    }

    // Make sure the ADSR envelope is shown if needed
    if (_eg_chart->isHidden()) {
        _eg_chart->show();
    }
}

//----------------------------------------------------------------------------
// show_vcf_cutoff
//----------------------------------------------------------------------------
void MainWindow::show_vcf_cutoff(const ShowVcfCutoffParamMsg& msg)
{
    // Are we showing this param as a list object?
    if (msg.num_items == 0) {
        // Was a display string specified? If so - show this as the value
        QString value_str = "";
        if (std::strlen(msg.display_string) > 0) {
            // Show the param value as a text string
            value_str = msg.display_string;           
        }
        else {
            // Show the param value as a numerical value
            value_str = msg.value_string;
        }
        
        // Show the param name and value
        _param_value_bar->set_param_name(msg.name);
        _param_value_bar->set_param_value(value_str);
        _param_value_bar->show();

        if (_enum_param_list->isVisible() || _wt_enum_param_list->isVisible()) {
            _show_enum_param_list_obj(false);
        }
        _show_param_obj(false);
        _params_list->clear();
        _list_items.clear();
        _list_items_enabled.clear();
        _list_items_separator.clear();

        _vcf_cutoff_chart->setGeometry (EG_STD_MARGIN_LEFT,
                                        EG_STD_MARGIN_TOP,
                                        EG_STD_WIDTH,
                                        EG_STD_HEIGHT);
        _show_default_background(false, false);           
    }
    else {
        // Update the Left status object
        _status_bar->set_left_status(msg.name);

        // Update the Cutoff chart
        _vcf_cutoff_chart->update_chart(msg.hp_filter, msg.lp_filter);

        // Add the items to the list
        // Note: If the list has items, scroll to the start of the list to reset the scroll
        if (_params_list->count()) {
            _params_list->scrollToItem(_params_list->item(0), QAbstractItemView::PositionAtCenter);
        }
        _params_list->clear();
        _list_items.clear();
        _list_items_enabled.clear();
        _list_items_separator.clear();

        for (uint i=0; i<msg.num_items; i++) {
            _label_set_text(_dummy_label, msg.list_items[i], PARAM_LIST_WIDTH-30);
            auto label = new QLabel(this);
            auto item = new QListWidgetItem(_params_list);
            label->setFont(QFont(STANDARD_FONT_NAME, LIST_FONT_SIZE));
            _list_label_set_text(label, _dummy_label->text(), (i == msg.selected_item), true, false); //msg.params.list_item_enabled[i], msg.list_item_separator[i]);
            item->setSizeHint(QSize(PARAM_LIST_WIDTH, LIST_ROW_HEIGHT));
            _params_list->addItem(item);
            _params_list->setItemWidget(item, label);
            _list_items.push_back(_dummy_label->text().toStdString());
            _list_items_enabled.push_back(true); //msg.list_item_enabled[i]);
            _list_items_separator.push_back(false); //(msg.list_item_separator[i]);
        }

        //if (msg.selected_item < msg.num_items)
            //_params_list->setCurrentRow(msg.selected_item);
        //else
            //_params_list->setCurrentRow(0);
        _params_list->setGeometry (LIST_MARGIN_LEFT,
                                PARAM_LIST_MARGIN_TOP, 
                                PARAM_LIST_WIDTH,
                                LIST_HEIGHT);             
        _params_list->setVisible(true);
        _param_value_bar->hide();

        // Show the param value as a numerical value
        QString str = msg.value_string;
        _param_value->setFont(QFont(PARAM_VALUE_FONT_NAME, PARAM_VALUE_NUM_FONT_SIZE));
        _param_value->setText(str);
        _param_value->adjustSize();
        _param_value->show();

        // Set the value geometry
        uint width = _param_value->width();
        while (width > PARAM_VALUE_WIDTH) {
            str = str.mid(0, str.size() - 1);
            QString trunc_str = str + "~";
            _param_value->setText(trunc_str);
            _param_value->adjustSize();
            width = _param_value->width();
        }
        int x = (VISIBLE_LCD_WIDTH - width - PARAM_VALUE_MARGIN_LEFT);
        //int y = STATUS_BAR_HEIGHT + ((VISIBLE_LCD_HEIGHT - STATUS_BAR_HEIGHT - BOTTOM_BAR_HEIGHT - _param_value->height()) / 2);
        int y = _params_list->y() + (_params_list->height() / 2) + (((_params_list->height() / 2) - _param_value->height()) / 2);
        _param_value->setGeometry(x, y, width, _param_value->height());
        _param_value_tag->hide();

        // Trick to make QT scroll LOGICALLY and CONSISTENTLY (duh)
        // Note this assumes that there are 6 items shown in the list
        uint index = msg.selected_item;
        if (index < 4)
            index = 0;
        if (index == 4)
            index = 3;
        _params_list->scrollToItem(_params_list->item(index), QAbstractItemView::PositionAtCenter);
        _vcf_cutoff_chart->setGeometry (EG_LST_MARGIN_LEFT,
                                        EG_LST_MARGIN_TOP,
                                        EG_LST_WIDTH,
                                        EG_LST_HEIGHT);
        _show_default_background(false, true);        
    }

    if (_enum_param_list->isVisible() || _wt_enum_param_list->isVisible()) {
        _show_enum_param_list_obj(false);
    }
    //_show_param_obj(true);
    
    _multi_status_bar->hide();
    _main_list->hide();
    _edit_name->hide();
    _eg_chart->hide();

    // Show the envelope view
    _vcf_cutoff_chart->show();
}

//----------------------------------------------------------------------------
// update_vcf_cutoff
//----------------------------------------------------------------------------
void MainWindow::update_vcf_cutoff(const ShowVcfCutoffParamUpdateMsg& msg)
{
    // Update the Cutoff chart
    _vcf_cutoff_chart->update_chart(msg.hp_filter, msg.lp_filter);   

    // Make sure the Cutoff chart is shown
    _vcf_cutoff_chart->show();

    // Check the selected item is within range
    if ((msg.selected_item != -1) && msg.selected_item < _params_list->count()) {
        // Its within range, so select that row
        _params_list->setCurrentRow(msg.selected_item);

        QString str = msg.value_string;
        _param_value->setText(str);
        _param_value->adjustSize();

        int x = (VISIBLE_LCD_WIDTH - _param_value->width() - PARAM_VALUE_MARGIN_LEFT);
        int y = _params_list->y() + (_params_list->height() / 2) + (((_params_list->height() / 2) - _param_value->height()) / 2);
        _param_value->setGeometry(x, y, _param_value->width(), _param_value->height());

        // Trick to make QT scroll LOGICALLY and CONSISTENTLY (duh)
        // Note this assumes that there are 6 items shown in the list
        uint index = msg.selected_item;
        if (index < 4)
            index = 0;
        if (index == 4)
            index = 3;
        _params_list->scrollToItem(_params_list->item(index), QAbstractItemView::PositionAtCenter);

        for (int i=0; i<_params_list->count(); i++) {
            auto item = reinterpret_cast<QLabel *>(_params_list->itemWidget(_params_list->item(i)));
            _list_label_set_text(item, _list_items[i].c_str(), (i == msg.selected_item), _list_items_enabled[i], _list_items_separator[i]);
        }
    }
    else {
        // Was a display string specified? If so - show this as the value
        QString value_str = "";
        if (std::strlen(msg.display_string) > 0) {
            // Show the param value as a text string
            value_str = msg.display_string;           
        }
        else {
            // Show the param value as a numerical value
            value_str = msg.value_string;
        }
        
        // Update the param value
        _param_value_bar->set_param_name(msg.name);
        _param_value_bar->set_param_value(value_str);
        _param_value_bar->show();
        _show_default_background(false, false);    
    }    
}

//----------------------------------------------------------------------------
// main_screen_capture
//----------------------------------------------------------------------------
void MainWindow::main_screen_capture()
{
    // If the USB drive is mounted as "/media"
    if (std::filesystem::exists("/media")) {
        char name[sizeof("/media/CAPTURE-000.png")];

        // Create the filename
        std::sprintf(name, "/media/CAPTURE-%03d.png", _screen_capture_index);

        // Capture the screen and save to the USB
        QPainter painter;
        QPixmap pixmap(LCD_WIDTH, LCD_HEIGHT);
        painter.begin(&pixmap);
        this->render(&painter);
        painter.end();
        pixmap.save(name);
        sync();
        _screen_capture_index++;
    }
}

//----------------------------------------------------------------------------
// _create_gui_objs
//----------------------------------------------------------------------------
void MainWindow::_create_gui_objs()
{
    // Set the default background to all black, no image
    _background = new Background(this);

    // Create the default background object - the MONIQUE logo
    _default_background = new Background(this);
    _default_background->set_image(MONIQUE_LOGO_PNG_RES);

    // Create the Sound Scope
    _sound_scope = new SoundScope(_sound_scope_mode, this);
    _sound_scope->setGeometry(OSC_SOUND_SCOPE_MARGIN_LEFT, SOUND_SCOPE_MARGIN_TOP, OSC_SOUND_SCOPE_WIDTH, SOUND_SCOPE_HEIGHT);
    _sound_scope->start();

    // Create the status bar object
    _status_bar = new StatusBar(this);
    _status_bar->setGeometry (STATUS_BAR_MARGIN_LEFT, STATUS_BAR_MARGIN_TOP, STATUS_BAR_WIDTH, STATUS_BAR_HEIGHT);

    // Create the multi status bar object
    _multi_status_bar = new MultiStatusBar(this);
    _multi_status_bar->setGeometry (MULTI_STATUS_BAR_MARGIN_LEFT, MULTI_STATUS_BAR_MARGIN_TOP, MULTI_STATUS_BAR_WIDTH, MULTI_STATUS_BAR_HEIGHT);
    _multi_status_bar->hide();

    // Create the param value bar object
    _param_value_bar = new ParamValueBar(this);
    _param_value_bar->setGeometry (MULTI_STATUS_BAR_MARGIN_LEFT, MULTI_STATUS_BAR_MARGIN_TOP, MULTI_STATUS_BAR_WIDTH, MULTI_STATUS_BAR_HEIGHT);
    _param_value_bar->hide();

    // Create the Params List object
    _params_list = new QListWidget(this);
    _params_list->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _params_list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _params_list->setFont(QFont(STANDARD_FONT_NAME, LIST_FONT_SIZE));
    _params_list->setSpacing(0);
	_params_list->setStyleSheet(
        "QListWidget { border: 0px; background-color: transparent; color : " + utils::get_css_system_colour_str() + "; }");
    _params_list->hide();
    _params_list->setGeometry (LIST_MARGIN_LEFT,
                               LIST_MARGIN_TOP, 
                               PARAM_LIST_WIDTH,
                               LIST_HEIGHT);
    
    // Create the Param Value object
    _param_value = new QLabel(this);
	_param_value->setFont(QFont(PARAM_VALUE_FONT_NAME, PARAM_VALUE_NUM_FONT_SIZE));
	_param_value->setStyleSheet("QLabel { color : " + utils::get_css_system_colour_str() + "; }");
    _params_list->hide();

    // Create the Param Value tag object
    _param_value_tag = new QLabel(this);
	_param_value_tag->setFont(QFont(STANDARD_FONT_NAME, PARAM_VALUE_TAG_FONT_SIZE));
	_param_value_tag->setStyleSheet("QLabel { color : " + utils::get_css_system_colour_str() + "; }");
    _param_value_tag->hide();

    // Create the Enum Param List object
    _enum_param_list = new QListWidget(this);
    _enum_param_list->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _enum_param_list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _enum_param_list->setFont(QFont(STANDARD_FONT_NAME, LIST_FONT_SIZE));
	_enum_param_list->setStyleSheet(
        "QListWidget { background-color: black; color : " + utils::get_css_system_colour_str() + "; }"
        "QListWidget::item { padding-left: 20px; padding-right: 10px; border-bottom-width: 0.5px; border-bottom-style: solid; border-bottom-color: " + utils::get_css_system_colour_str() + "; }"
        "QListWidget::item::selected { color: black; background: " + utils::get_css_system_colour_str() + "; }");
    _enum_param_list->hide();
    _enum_param_list->setGeometry (LIST_MARGIN_LEFT,
                                   LIST_MARGIN_TOP, 
                                   LIST_WIDTH,
                                   LIST_HEIGHT);

    // Create the WT Enum Param List object
    _wt_enum_param_list = new QListWidget(this);
    _wt_enum_param_list->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _wt_enum_param_list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _wt_enum_param_list->setFont(QFont(STANDARD_FONT_NAME, LIST_FONT_SIZE));
	_wt_enum_param_list->setStyleSheet(
        "QListWidget { background-color: black; color : " + utils::get_css_system_colour_str() + "; }"
        "QListWidget::item { padding-left: 20px; padding-right: 10px; border-bottom-width: 0.5px; border-bottom-style: solid; border-bottom-color: " + utils::get_css_system_colour_str() + "; }"
        "QListWidget::item::selected { color: black; background: " + utils::get_css_system_colour_str() + "; }");
    _wt_enum_param_list->hide();
    _wt_enum_param_list->setGeometry (LIST_MARGIN_LEFT,
                                      LIST_MARGIN_TOP, 
                                      WT_LIST_WIDTH,
                                      LIST_HEIGHT);

    // Create the envelope scope
    _eg_chart = new EgChart(this);
    _eg_chart->setGeometry (EG_STD_MARGIN_LEFT,
                            EG_STD_MARGIN_TOP,
                            EG_STD_WIDTH,
                            EG_STD_HEIGHT);

    // Create the cutoff chart
    _vcf_cutoff_chart = new VcfCutoffChart(this);
    _vcf_cutoff_chart->setGeometry (VCF_CUTOFF_MARGIN_LEFT,
                                VCF_CUTOFF_MARGIN_TOP,
                                VCF_CUTOFF_WIDTH,
                                VCF_CUTOFF_HEIGHT);

    // Create the WT scope
    _wt_scope = new WtScope(this);
    _wt_scope->setGeometry (WT_CHART_MARGIN_LEFT,
                            WT_CHART_MARGIN_TOP,
                            WT_CHART_WIDTH,
                            WT_CHART_HEIGHT);

    // Create the Main List object
    _main_list = new MainList(NUM_LIST_ROWS, this);    
    _main_list->setGeometry (LIST_MARGIN_LEFT, LIST_MARGIN_TOP, LIST_WIDTH, LIST_HEIGHT);

    // Create the Bottom Bar object
    _bottom_bar = new BottomBar(this);
    _bottom_bar->setGeometry (BOTTOM_BAR_MARGIN_LEFT, BOTTOM_BAR_MARGIN_TOP, VISIBLE_LCD_WIDTH, BOTTOM_BAR_HEIGHT);

    // Create the Edit Name object
    _edit_name = new EditName(this);
    _edit_name->setGeometry (MAIN_AREA_MARGIN_LEFT, MAIN_AREA_MARGIN_TOP, MAIN_AREA_WIDTH, MAIN_AREA_HEIGHT);

    // Create the Message Box
    _msg_box = new MsgBox(this);
    _msg_box->setGeometry (MAIN_AREA_MARGIN_LEFT, MAIN_AREA_MARGIN_TOP, MAIN_AREA_WIDTH, MAIN_AREA_HEIGHT);

    /* Create the Confirmation Screen objects */
    _msg_popup = new MsgPopup(this);
    _msg_popup->setGeometry (MAIN_AREA_MARGIN_LEFT, MAIN_AREA_MARGIN_TOP, MAIN_AREA_WIDTH, MAIN_AREA_HEIGHT);

    // Always show the boot warning until cleared
    _boot_warning_background = new Background(this);
    _boot_warning_background->set_image(BOOT_WARNING_PNG_RES);

    // Create the dummy label - not visible
    // This is used to calculate max string lengths (generally used in lists)
    _dummy_label = new QLabel(this);
	_dummy_label->setFont(QFont(STANDARD_FONT_NAME, LIST_FONT_SIZE)); 
    _dummy_label->setVisible(false);
}

//----------------------------------------------------------------------------
// _set_gui_objs_system_colour
//----------------------------------------------------------------------------
void MainWindow::_set_gui_objs_system_colour()
{
    // Set the system colour for all GUI objects
    // Default background object - the MONIQUE logo
    _default_background->set_image(MONIQUE_LOGO_PNG_RES);

    // Sound Scope
    _sound_scope->refresh_colour();

    // Status bar
	_status_bar->refresh_colour();

    // Multi status bar
    _multi_status_bar->refresh_colour();

    // Param value bar
    _param_value_bar->refresh_colour();

    // Bottom bar
    _bottom_bar->refresh_colour();

    // Params List object
	_params_list->setStyleSheet(
        "QListWidget { background-color: transparent; color : " + utils::get_css_system_colour_str() + "; }");
    
    // Param Value object
	_param_value->setStyleSheet("QLabel { color : " + utils::get_css_system_colour_str() + "; }");

    // Param Value tag object
	_param_value_tag->setStyleSheet("QLabel { color : " + utils::get_css_system_colour_str() + "; }");

    // num Param List object
	_enum_param_list->setStyleSheet(
        "QListWidget { background-color: black; color : " + utils::get_css_system_colour_str() + "; }"
        "QListWidget::item { padding-left: 20px; padding-right: 10px; border-bottom-width: 0.5px; border-bottom-style: solid; border-bottom-color: " + utils::get_css_system_colour_str() + "; }"
        "QListWidget::item::selected { color: black; background: " + utils::get_css_system_colour_str() + "; }");

    // WT enum param list
	_wt_enum_param_list->setStyleSheet(
        "QListWidget { background-color: black; color : " + utils::get_css_system_colour_str() + "; }"
        "QListWidget::item { padding-left: 20px; padding-right: 10px; border-bottom-width: 0.5px; border-bottom-style: solid; border-bottom-color: " + utils::get_css_system_colour_str() + "; }"
        "QListWidget::item::selected { color: black; background: " + utils::get_css_system_colour_str() + "; }");

    // EG chart
    _eg_chart->refresh_colour();

    // WT Scope
    _wt_scope->refresh_colour();

    // Main Area List object
	_main_list->refresh_colour();

    // Cutoff chart
    _vcf_cutoff_chart->refresh_colour();

    // Edit name
    _edit_name->refresh_colour();

    // Message Box
    _msg_box->refresh_colour();

    // Message Popup
    _msg_popup->refresh_colour();
}

//----------------------------------------------------------------------------
// _show_default_background
//----------------------------------------------------------------------------
void MainWindow::_show_default_background(bool show, bool show_scope)
{
    // If showing
    if (show) {
        // Show the Scope or default background
        if ((_sound_scope_mode == SoundScopeMode::SCOPE_MODE_OFF) || !show_scope) {
            // No Scope, show the default background        
            _sound_scope->clear_scope();
            _sound_scope->hide();
            _default_background->setVisible(true);
        }
        else {
            // Show the scope in the foreground and hide the default background
            _sound_scope->show(ScopeDisplayMode::FOREGROUND);
            _default_background->setVisible(false);            
        }
    }
    else {
        // Show/hide the Scope
        if ((_sound_scope_mode == SoundScopeMode::SCOPE_MODE_OFF) || !show_scope) {
            // No Scope
            _sound_scope->clear_scope();
            _sound_scope->hide();                    
        }
        else {
            // Show the scope in the background
            _sound_scope->show(ScopeDisplayMode::BACKGROUND);
        }

        // Hide the default background
        _default_background->setVisible(false); 
    }
}

//----------------------------------------------------------------------------
// _show_param_obj
//----------------------------------------------------------------------------
void MainWindow::_show_param_obj(bool show)
{
    // Show/hide the Param
    _param_value->setVisible(show);
    _param_value_tag->setVisible(show);
    _params_list->setVisible(show);
}

//----------------------------------------------------------------------------
// _show_enum_param_list_obj
//----------------------------------------------------------------------------
void MainWindow::_show_enum_param_list_obj(bool show)
{
    // Show/hide the enum Param list
    _enum_param_list->setVisible(show);
    _wt_enum_param_list->setVisible(show);

    // If hiding, make sure the WT chart is stopped and cleared
    if (!show) {
        _wt_scope->unload_wt_file();
        QCoreApplication::processEvents();
    }

    // Show/hide the WT chart
    _wt_scope->setVisible(show);  
}

//----------------------------------------------------------------------------
// _label_set_text
//----------------------------------------------------------------------------
void MainWindow::_label_set_text(QLabel *label, const char *text, int max_width)
{
    QString str = text;

    // Update the object text
    label->setText(str);
    label->adjustSize();
    if (max_width != -1) {
        uint width = label->width();
        while (width > (uint)max_width) {
            str = str.mid(0, str.size() - 1);
            QString trunc_str = str + "~";
            label->setText(trunc_str);
            label->adjustSize();
            width = label->width();
        }
    }
}

//----------------------------------------------------------------------------
// _wt_label_set_text
//----------------------------------------------------------------------------
void MainWindow::_wt_label_set_text(QLabel *label, const char *text)
{
    // Get the start and end strings to use if string truncation is needed
    QString str1 = text;
    QString str2 = text;
    str1 = str1.mid(0, (str1.size() - 3));
    str2 = str2.mid((str2.size()-3), 3);

    // Update the object text
    label->setText(text);
    label->adjustSize();
    uint width = label->width();
    while (width > (WT_LIST_WIDTH - 30)) {
        str1 = str1.mid(0, str1.size() - 1);
        QString trunc_str = str1 + "~" + str2;
        label->setText(trunc_str);
        label->adjustSize();
        width = label->width();
    }
}

//----------------------------------------------------------------------------
// _list_label_set_text
//----------------------------------------------------------------------------
void MainWindow::_list_label_set_text(QLabel *label, QString text, bool selected, bool enabled, bool separator)
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
// _list_add_item
//----------------------------------------------------------------------------
void MainWindow::_list_add_item(QListWidget *list, const char *text, int list_width)
{
    // Firstly find the text we can display, truncated if necessary
    // We do this with the dummy QLabel object  
    _label_set_text(_dummy_label, text, list_width);

    // Add the item to the list
    list->addItem(_dummy_label->text());
    auto item = list->item(list->count() - 1);
    item->setSizeHint(QSize(list_width, LIST_ROW_HEIGHT));   
}

//----------------------------------------------------------------------------
// _list_add_item
//----------------------------------------------------------------------------
void MainWindow::_list_add_item(QListWidget *list, const char *text, int list_width, bool selected, bool enabled)
{
    // Firstly find the text we can display, truncated if necessary
    // We do this with the dummy QLabel object  
    _label_set_text(_dummy_label, text, list_width);
    auto label = new QLabel(this);
    auto item = new QListWidgetItem(list);
    label->setFont(QFont(STANDARD_FONT_NAME, LIST_FONT_SIZE));
    _list_label_set_text(label, _dummy_label->text(), selected, enabled, false);
    item->setSizeHint(QSize(list_width, LIST_ROW_HEIGHT));
    list->addItem(item);
    list->setItemWidget(item, label);

    _list_items.push_back(_dummy_label->text().toStdString());
    _list_items_enabled.push_back(enabled);  
}
