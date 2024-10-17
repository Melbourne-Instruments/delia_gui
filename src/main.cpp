/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023-2024 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  main.cpp
 * @brief Main entry point to the MONIQUE GUI.
 *-----------------------------------------------------------------------------
 */
#include <signal.h>
#include <QApplication>
#include <QScreen>
#include <QSplashScreen>
#include "main_window.h"
#include "gui_common.h"
#include "utils.h"
#include "version.h"

// Local functions
QPixmap _set_pixmap_colour(const QPixmap& pixmap, QColor colour);
void _print_delia_gui_info();
void _sigint_handler([[maybe_unused]] int sig);

//----------------------------------------------------------------------------
// main
//----------------------------------------------------------------------------
int main(int argc, char *argv[ ])
{
	QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication app(argc, argv);

    // Initialise the utilties module
    utils::init();

    // Show the first splash while we are getting ready
    QPixmap pixmap1(MELB_INST_SPLASH_PNG_RES);
    QSplashScreen splash1(_set_pixmap_colour(pixmap1, utils::get_system_colour()));
    splash1.show();    

    // Setup the exit signal handler (e.g. ctrl-c, kill)
    signal(SIGINT, _sigint_handler);
    signal(SIGTERM, _sigint_handler);

    // Show the app info
    _print_delia_gui_info();

    // Show the second splash while we are getting ready
    QPixmap pixmap2(MONIQUE_LOGO_PNG_RES);
    QSplashScreen splash2(_set_pixmap_colour(pixmap2, utils::get_system_colour()));
    splash1.finish(&splash2);
    splash2.show();

    // Show the main window and execute the app
    MainWindow main_window;
    splash2.finish(&main_window);
    main_window.show();
    int res = app.exec();

    // DELIA GUI has exited
    MSG("DELIA GUI exited");    
    return res; 
}

//----------------------------------------------------------------------------
// _set_pixmap_colour
//----------------------------------------------------------------------------
QPixmap _set_pixmap_colour(const QPixmap& pixmap, QColor colour)
{
    // Convert the Pixmap colour
    QColor default_colour(0xFF, 0x00, 0x00);
    QImage tmp(pixmap.toImage());
    for (int y=0; y<tmp.height(); y++) {
        for (int x=0; x<tmp.width(); x++){
            // Change the pixel colour if needed
            colour.setAlpha(tmp.pixelColor(x, y).alpha());
            default_colour.setAlpha(tmp.pixelColor(x, y).alpha());
            if (tmp.pixelColor(x, y) == default_colour)
                tmp.setPixelColor(x, y, colour);
        }
    }
    return QPixmap::fromImage(tmp);
}

//----------------------------------------------------------------------------
// _print_delia_gui_info
//----------------------------------------------------------------------------
void _print_delia_gui_info()
{
    MSG("DELIA GUI - Copyright (c) 2023-2024 Melbourne Instruments, Australia");
#ifdef MONIQUE_GUI_BETA_RELEASE    
    MSG("Version " << MONIQUE_GUI_MAJOR_VERSION << "." 
                   << MONIQUE_GUI_MINOR_VERSION << "." 
                   << MONIQUE_GUI_PATCH_VERSION << "-beta" 
                   << " (" << MONIQUE_GUI_GIT_COMMIT_HASH << ")");
#else
    MSG("Version " << MONIQUE_GUI_MAJOR_VERSION << "." 
                   << MONIQUE_GUI_MINOR_VERSION << "." 
                   << MONIQUE_GUI_PATCH_VERSION
                   << " (" << MONIQUE_GUI_GIT_COMMIT_HASH << ")");
#endif
    MSG("");
}

//----------------------------------------------------------------------------
// _sigint_handler
//----------------------------------------------------------------------------
void _sigint_handler([[maybe_unused]] int sig)
{
    // Quit the QT app and clean everything up
    qApp->quit();
}
