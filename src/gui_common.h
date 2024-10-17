/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023-2024 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  gui_common.h
 * @brief GUI common definitions.
 *-----------------------------------------------------------------------------
 */
#ifndef _COMMON_H
#define _COMMON_H

#include <cstring>
#include <iostream>
#include <chrono>
#include <QMetaType>

//--------------
// Build options
//--------------

// Constants
constexpr uint LCD_HEIGHT                           = 480;
constexpr uint LCD_WIDTH                            = 854;
constexpr char MELB_INST_SPLASH_PNG_RES[]           = ":res/melb_inst_splash.png";
constexpr char MONIQUE_LOGO_PNG_RES[]               = ":res/monique_logo.png";
constexpr char BOOT_WARNING_PNG_RES[]               = ":res/boot_warning.png";
constexpr char TWELVE_VOICE_STATUS_PNG_RES[]        = ":res/12-note-icon.png";
constexpr char ZERO_VOICE_ACTIVE_LAYER_PNG_RES[]    = ":res/0-voice-active-layer.png";
constexpr char ONE_VOICE_ACTIVE_LAYER_PNG_RES[]     = ":res/1-voice-active-layer.png";
constexpr char TWO_VOICE_ACTIVE_LAYER_PNG_RES[]     = ":res/2-voice-active-layer.png";
constexpr char THREE_VOICE_ACTIVE_LAYER_PNG_RES[]   = ":res/3-voice-active-layer.png";
constexpr char FOUR_VOICE_ACTIVE_LAYER_PNG_RES[]    = ":res/4-voice-active-layer.png";
constexpr char FIVE_VOICE_ACTIVE_LAYER_PNG_RES[]    = ":res/5-voice-active-layer.png";
constexpr char SIX_VOICE_ACTIVE_LAYER_PNG_RES[]     = ":res/6-voice-active-layer.png";
constexpr char ZERO_VOICE_INACTIVE_LAYER_PNG_RES[]  = ":res/0-voice-inactive-layer.png";
constexpr char ONE_VOICE_INACTIVE_LAYER_PNG_RES[]   = ":res/1-voice-inactive-layer.png";
constexpr char TWO_VOICE_INACTIVE_LAYER_PNG_RES[]   = ":res/2-voice-inactive-layer.png";
constexpr char THREE_VOICE_INACTIVE_LAYER_PNG_RES[] = ":res/3-voice-inactive-layer.png";
constexpr char FOUR_VOICE_INACTIVE_LAYER_PNG_RES[]  = ":res/4-voice-inactive-layer.png";
constexpr char FIVE_VOICE_INACTIVE_LAYER_PNG_RES[]  = ":res/5-voice-inactive-layer.png";
constexpr char SIX_VOICE_INACTIVE_LAYER_PNG_RES[]   = ":res/6-voice-inactive-layer.png";
constexpr char HOURGLASS_GIF_RES[]                  = ":res/hourglass-animated.gif";
constexpr char OCR_A_FONT_RES[]                     = ":res/OCR-A.ttf";
constexpr char OCR_B_FONT_RES[]                     = ":res/OCR-B.ttf";
constexpr char DSEG7_CLASSIC_FONT_RES[]             = ":res/DSEG7Classic-BoldItalic.ttf";
constexpr char STANDARD_FONT_NAME[]                 = "OCR-B";
constexpr char PARAM_VALUE_FONT_NAME[]              = "DSEG7 Classic";
constexpr uint WT_CHART_REFRESH_RATE                = std::chrono::milliseconds(17).count();
constexpr uint SCOPE_NUM_SAMPLES                    = 128;
constexpr uint SCOPE_SAMPLES_MSG_SIZE               = (SCOPE_NUM_SAMPLES * 2);

// MACRO to show a string on the console
#define MSG(str) do { std::cout << str << std::endl; } while( false )

// MACRO to show a debug string on the console
#ifndef NDEBUG
#define DEBUG_MSG(str) MSG(str)
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

#endif  // _COMMON_H
