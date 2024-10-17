/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023-2024 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  gui_msg_thread.h
 * @brief GUI Message Thread class definitions.
 *-----------------------------------------------------------------------------
 */
#ifndef GUI_MSG_THREAD_H
#define GUI_MSG_THREAD_H

#include <atomic>
#include <QThread>
#include "gui_msg.h"
#include "gui_common.h"

// GUI Message Thread class
class GuiMsgThread : public QThread
{
	Q_OBJECT
public:
    GuiMsgThread(QObject *parent);
    ~GuiMsgThread();
    void run();

signals:
    void left_status_msg(const SetLeftStatusMsg& msg);
    void layer_status_msg(const SetLayerStatusMsg& msg);
    void midi_status_msg(const SetMidiStatusMsg& msg);
    void tempo_status_msg(const SetTempoStatusMsg& msg);
    void home_screen_msg(const ShowHomeScreenMsg& msg);
    void list_items_msg(const ShowListItemsMsg& msg);
    void list_select_item_msg(const ListSelectItemMsg& msg);
    void select_layer_name_msg(const SelectLayerNameMsg& msg);
    void soft_buttons_text_msg(const SetSoftButtonsTextMsg& msg);
    void soft_buttons_state_msg(const SetSoftButtonsStateMsg& msg);
    void param_update_msg(const ShowNormalParamMsg& msg);
    void param_value_update_msg(const ShowNormalParamUpdateMsg& msg);
    void enum_param_update_msg(const ShowEnumParamUpdateMsg& msg);
    void enum_param_value_update_msg(const ListSelectItemMsg& msg);
    void edit_name_msg(const ShowEditNameMsg& msg);
    void edit_name_select_char_msg(const EditNameSelectCharMsg& msg);
    void edit_name_change_char_msg(const EditNameChangeCharMsg& msg);
    void msg_popup_msg(const ShowMsgPopupMsg& msg);
    void msg_box_msg(const ShowMsgBoxMsg& msg);
    void clear_boot_warning_msg();
    void set_system_colour_msg(const SetSystemColour &msg);
    void show_adsr_envelope_msg(const ShowAdsrEnvParamMsg &msg);
    void update_adsr_envelope_msg(const ShowAdsrEnvParamUpdateMsg &msg);
    void show_vcf_cutoff_msg(const ShowVcfCutoffParamMsg &msg);
    void update_vcf_cutoff_msg(const ShowVcfCutoffParamUpdateMsg &msg);
    void main_screen_capture_msg();

private:
    std::atomic<bool> _exit_gui_msgs_thread;
};

#endif
