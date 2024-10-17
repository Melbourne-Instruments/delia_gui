/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023-2024 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  main_window.h
 * @brief Main Window class definitions.
 *-----------------------------------------------------------------------------
 */
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <thread>
#include <atomic>
#include <QMainWindow>
#include <QLabel>
#include <QListWidget>
#include <QTreeWidget>
#include <QTimer>
#include "background.h"
#include "timer.h"
#include "gui_msg_thread.h"
#include "status_bar.h"
#include "multi_status_bar.h"
#include "param_value_bar.h"
#include "bottom_bar.h"
#include "sound_scope.h"
#include "wt_scope.h"
#include "eg_chart.h"
#include "vcf_cutoff_chart.h"
#include "main_list.h"
#include "edit_name.h"
#include "msg_box.h"
#include "msg_popup.h"
#include "sound_scope_msg_thread.h"

// Param structure
struct Param
{
    QString name;
    QString value;
    bool value_is_str;
};

// Main Window class
class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
    // Constructor
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void set_left_status(const SetLeftStatusMsg& msg);
    void set_layer_status(const SetLayerStatusMsg& msg);
    void set_midi_status(const SetMidiStatusMsg& msg);
    void set_tempo_status(const SetTempoStatusMsg& msg);
    void show_home_screen(const ShowHomeScreenMsg& msg);
    void show_list_items(const ShowListItemsMsg& msg);
    void list_select_item(const ListSelectItemMsg& msg);
    void select_layer_name(const SelectLayerNameMsg& msg);
    void set_soft_buttons_text(const SetSoftButtonsTextMsg& msg);
    void set_soft_buttons_state(const SetSoftButtonsStateMsg& msg);
    void process_param_update(const ShowNormalParamMsg& msg);
    void process_param_value_update(const ShowNormalParamUpdateMsg& msg);
    void process_enum_param_update(const ShowEnumParamUpdateMsg& msg);
    void process_enum_param_value_update(const ListSelectItemMsg& msg);
    void process_edit_name(const ShowEditNameMsg& msg);
    void process_edit_name_select_char(const EditNameSelectCharMsg& msg);
    void process_edit_name_change_char(const EditNameChangeCharMsg& msg);
    void show_msg_popup(const ShowMsgPopupMsg& msg);
    void show_msg_box(const ShowMsgBoxMsg& msg);
    void clear_boot_warning();
    void set_system_colour(const SetSystemColour& msg);
    void show_adsr_envelope(const ShowAdsrEnvParamMsg& msg);
    void update_adsr_envelope(const ShowAdsrEnvParamUpdateMsg& msg);
    void show_vcf_cutoff(const ShowVcfCutoffParamMsg& msg);
    void update_vcf_cutoff(const ShowVcfCutoffParamUpdateMsg& msg);
    void main_screen_capture();

private:
    // Private variables
    Background *_background;
    Background *_boot_warning_background;
    Background *_default_background;
    StatusBar *_status_bar;
    MultiStatusBar *_multi_status_bar;
    ParamValueBar *_param_value_bar;
    BottomBar *_bottom_bar;
    MainList *_main_list;
    SoundScope *_sound_scope;
    EgChart *_eg_chart;
    VcfCutoffChart *_vcf_cutoff_chart;
    WtScope *_wt_scope;
    EditName *_edit_name;
    MsgBox *_msg_box;
    MsgPopup *_msg_popup;
    SoundScopeMode _sound_scope_mode;
    GuiMsgThread *_gui_msg_thread;
    SoundScopeMsgThread *_sound_scope_msg_thread;
    QLabel *_param_value;
    QLabel *_param_value_tag;
    QListWidget *_params_list;
    QListWidget *_enum_param_list;
    QListWidget *_wt_enum_param_list;
    QLabel *_dummy_label;
    std::vector<std::string> _list_items;
    std::vector<bool> _list_items_enabled;
    std::vector<bool> _list_items_separator;
    std::vector<std::string> _enum_list_items;
    std::vector<Param> _params;
    uint _screen_capture_index;

    // Private functions
    void _show_default_background(bool show, bool show_scope);
    void _show_logo_obj(bool show);
    void _show_param_obj(bool show);
    void _show_enum_param_list_obj(bool show);
    void _create_gui_objs();
    void _set_gui_objs_system_colour();  
    void _label_set_text(QLabel *label, const char *text, int max_width);
    void _wt_label_set_text(QLabel *label, const char *text);
    void _list_label_set_text(QLabel *label, QString text, bool selected, bool enabled, bool separator);
    void _list_add_item(QListWidget *list, const char *text, int list_width);
    void _list_add_item(QListWidget *list, const char *text, int list_width, bool selected, bool enabled);
};

#endif
