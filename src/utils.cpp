/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023-2024 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  utils.cpp
 * @brief Utility functions implementation.
 *-----------------------------------------------------------------------------
 */
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPixmap>
#include "gui_common.h"
#include "utils.h"

// Constants
constexpr char CONFIG_FILE[]           = "/udata/delia/config.json";
constexpr char DEFAULT_SYSTEM_COLOUR[] = "FFFFFF";

// Private variables
QColor _system_colour;
QString _system_colour_str;


//----------------------------------------------------------------------------
// set_system_colour
//----------------------------------------------------------------------------
void utils::init()
{
    // Set the default system colour string
    QString colour_str(DEFAULT_SYSTEM_COLOUR);

    // We need to read the system colour from the MONIQUE config file
    // Firstly open and read the config file
    QFile file;
    QString config_json;
    file.setFileName(CONFIG_FILE);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // Read the file contents
        config_json = file.readAll();
        file.close();

        // Now get the system colour
        QJsonDocument doc = QJsonDocument::fromJson(config_json.toUtf8());
        QJsonObject obj = doc.object();
        QJsonValue val = obj.value(QString("system_colour"));
        if ((val != QJsonValue::Undefined) && val.isString()) {
            // Get the system colour string
            colour_str = val.toString();            
        }
    }

    // Set the system colour
    set_system_colour(colour_str.toStdString().c_str());
}

//----------------------------------------------------------------------------
// get_system_colour
//----------------------------------------------------------------------------
QColor utils::get_system_colour()
{
    return _system_colour;
}

//----------------------------------------------------------------------------
// get_system_colour_str
//----------------------------------------------------------------------------
QString utils::get_system_colour_str()
{
    return _system_colour_str;
}

//----------------------------------------------------------------------------
// get_css_system_colour_str
//----------------------------------------------------------------------------
QString utils::get_css_system_colour_str()
{
    return "#" + _system_colour_str;
}

//----------------------------------------------------------------------------
// get_system_colour_brush
//----------------------------------------------------------------------------
QBrush utils::get_system_colour_brush()
{
    // Return the system colour brush
    int rgb = _system_colour.rgb();
    return QBrush(QColor(((rgb >> 16) & 0xFF), ((rgb >> 8) & 0xFF), (rgb & 0xFF)));
}

//----------------------------------------------------------------------------
// get_dimmed_css_system_colour_str
//----------------------------------------------------------------------------
QString utils::get_dimmed_css_system_colour_str(uint intensity)
{
    // Return the system colour dimmed (at the required intensity out of 10)
    int rgb = _system_colour.rgb();
    return QString("#%1%2%3").arg(((((rgb >> 16) & 0xFF) * intensity) / 10), 2, 16, QChar('0'))
                             .arg(((((rgb >> 8) & 0xFF) * intensity) / 10), 2, 16, QChar('0'))
                             .arg((((rgb & 0xFF) * intensity) / 10), 2, 16, QChar('0'));
}

//----------------------------------------------------------------------------
// get_system_colour_brush
//----------------------------------------------------------------------------
QBrush utils::get_dimmed_system_colour_brush(uint intensity)
{
    // Return the system colour dimmed brush (at the required intensity out of 10)
    int rgb = _system_colour.rgb();
    return QBrush(QColor(((((rgb >> 16) & 0xFF) * intensity) / 10),
                         ((((rgb >> 8) & 0xFF) * intensity) / 10),
                         (((rgb & 0xFF) * intensity) / 10)));
}

//----------------------------------------------------------------------------
// get_system_colour
//----------------------------------------------------------------------------
void utils::set_system_colour(const char *colour_str)
{
    // Convert the system colour to an integer value
    QRgb colour;
    try {
        colour = std::stoi(colour_str, nullptr, 16);
        _system_colour_str = colour_str;
    }
    catch (...) {
        colour = std::stoi(DEFAULT_SYSTEM_COLOUR, nullptr, 16);
        _system_colour_str = DEFAULT_SYSTEM_COLOUR;
    }
    _system_colour = QColor(colour);
}

//----------------------------------------------------------------------------
// _set_pixmap_colour
//----------------------------------------------------------------------------
QPixmap utils::set_pixmap_to_system_colour(const QPixmap& pixmap)
{
    // Convert the Pixmap colour
    QColor default_colour(0xFF, 0x00, 0x00);
    QColor system_colour = utils::get_system_colour();
    QImage tmp(pixmap.toImage());
    for (int y=0; y<tmp.height(); y++) {
        for (int x=0; x<tmp.width(); x++){
            // Change the pixel colour if needed
            system_colour.setAlpha(tmp.pixelColor(x, y).alpha());
            default_colour.setAlpha(tmp.pixelColor(x, y).alpha());
            if (tmp.pixelColor(x, y) == default_colour)
                tmp.setPixelColor(x, y, system_colour);
        }
    }
    return QPixmap::fromImage(tmp);
}
