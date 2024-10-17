/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  scope.cpp
 * @brief Scope class implementation.
 *-----------------------------------------------------------------------------
 */
#include <QPainter>
#include "scope.h"
#include <QOpenGLShaderProgram>
#include "gui_common.h"
#include "utils.h"

// Constants
constexpr float FOREGROUND_ALPHA  = 1.0f;
constexpr float BACKGROUND_ALPHA  = 0.5f;
constexpr uint DEFAULT_LINE_WIDTH = 4;

// Vertex shader
static const char *_vertex_shader_source_core =
    "#version 310 es\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

// Fragment shader
static const char *_fragment_shader_source_core =
    "#version 310 es\n"
        "precision mediump float;\n"
        "out vec4 FragColor;\n"
        "uniform vec4 system_colour;\n"
        "void main()\n"
        "{\n"
        "   FragColor = system_colour;\n"
        "}\0";

//----------------------------------------------------------------------------
// Scope
//----------------------------------------------------------------------------
Scope::Scope(uint num_samples, QWidget *parent) : QOpenGLWidget(parent)
{
    // Initialise class variables
    _vertices = new float[num_samples * 3];
    for (uint i=0; i<num_samples; i++) {
        _vertices[(i*3)] = -1.0f + ((qreal(i) / num_samples) * 2);
        _vertices[(i*3)+1] = 0.0f;
        _vertices[(i*3)+2] = 0.0f;      
    }    
    _num_samples = num_samples;
    _program = nullptr;
    _alpha = FOREGROUND_ALPHA;
    _pen_width = DEFAULT_LINE_WIDTH;
}

//----------------------------------------------------------------------------
// ~Scope
//----------------------------------------------------------------------------
Scope::~Scope()
{
    // Perform any cleanup actions
    cleanup();
}

//----------------------------------------------------------------------------
// shown
//----------------------------------------------------------------------------
bool Scope::shown() const
{
    // Return if the scope is shown or not
    return isVisible();
}

//----------------------------------------------------------------------------
// display_mode
//----------------------------------------------------------------------------
ScopeDisplayMode Scope::display_mode() const
{
    // The scope in the foreground if the alpha is 1.0
    return _alpha == FOREGROUND_ALPHA ? 
                ScopeDisplayMode::FOREGROUND : ScopeDisplayMode::BACKGROUND;
}

//----------------------------------------------------------------------------
// show
//----------------------------------------------------------------------------
void Scope::show()
{
    // Show the scope (don't change the display mode)
    setVisible(true);
}

//----------------------------------------------------------------------------
// show
//----------------------------------------------------------------------------
void Scope::show(ScopeDisplayMode display_mode)
{
    // Show the scope and set the display mode
    setVisible(true);
    _alpha = display_mode == ScopeDisplayMode::FOREGROUND ? 
                FOREGROUND_ALPHA : BACKGROUND_ALPHA;
}

//----------------------------------------------------------------------------
// hide
//----------------------------------------------------------------------------
void Scope::hide(bool reset_display_mode)
{
    // Hide the scope - also make sure the display mode is reset if needed
    setVisible(false);
    if (reset_display_mode)
        _alpha = FOREGROUND_ALPHA;
}

//----------------------------------------------------------------------------
// set_colour
//----------------------------------------------------------------------------
void Scope::set_colour(QColor colour)
{
    // Set the scope colour
    _colour = colour;
}

//----------------------------------------------------------------------------
// set_pen_width
//----------------------------------------------------------------------------
void Scope::set_pen_width(uint width)
{
    // Set the pen width
    _pen_width = width;
}

//----------------------------------------------------------------------------
// refresh_data
//----------------------------------------------------------------------------
void Scope::refresh_data(const QVector<QPointF>& data)
{
    // Make sure we actually have useful data
    if (data.size() >= _num_samples) {    
        // Update the vertices data, and refresh the scope
        for (uint i=0; i<_num_samples; i++) {
            _vertices[(i*3)] = data[i].x();
            _vertices[(i*3)+1] = data[i].y();
        }    
        update();
    }
}

//----------------------------------------------------------------------------
// show_zero_scope
//----------------------------------------------------------------------------
void Scope::show_zero_scope()
{
    QVector<QPointF> data;

    // Add the zero points to the data
    for (uint i=0; i<_num_samples; i++) {
        auto x = -1.0f + ((qreal(i) / _num_samples) * 2);
        data.append(QPointF(x, 0));
    }

    // Refresh the chart
    refresh_data(data); 
}

//----------------------------------------------------------------------------
// clear_scope
//----------------------------------------------------------------------------
void Scope::clear_scope()
{
    QVector<QPointF> data;

    // Set all points as 0,0 - no lines will be drawn, but the WT scope will
    // be cleared
    for (uint i=0; i<_num_samples; i++) {
        data.append(QPointF(0, 0));
    }

    // Refresh the chart
    refresh_data(data);
}

//----------------------------------------------------------------------------
// cleanup
//----------------------------------------------------------------------------
void Scope::cleanup()
{
    // If the shader program is not defined
    if (_program == nullptr) {
        // Just delete any allocated memory
        delete [] _vertices;
    }
    else {
        // Clean up the shader program
        makeCurrent();
        delete [] _vertices;
        delete _program;
        _program = nullptr;
        doneCurrent();        
        QObject::disconnect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &Scope::cleanup);
    }
}

//----------------------------------------------------------------------------
// initializeGL
//----------------------------------------------------------------------------
void Scope::initializeGL()
{
    // Make sure we handle any Open GL clean-up correctly
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &Scope::cleanup);

    // Intialise Open GL - including enabling blend (for alpha control), and setting
    // the clear colour
    initializeOpenGLFunctions();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0, 0, 0, 0);

    // Create the Open GL shader program - adding our vertex and fragment processing
    _program = new QOpenGLShaderProgram;
    _program->addShaderFromSourceCode(QOpenGLShader::Vertex, _vertex_shader_source_core);
    _program->addShaderFromSourceCode(QOpenGLShader::Fragment, _fragment_shader_source_core);
    _program->bindAttributeLocation("vertex", 0);
    _program->link();
    _program->bind();
    _colour_loc = _program->uniformLocation("system_colour");
    
    // Create our Vertex Array Object (VAO), and bind it to our Vertex Buffer Object (VBO)
    _vao.create();
    QOpenGLVertexArrayObject::Binder vao_binder(&_vao);
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    _vbo.create();
    _vbo.bind();
    _vbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    _vbo.allocate(_vertices, (_num_samples * 3) * sizeof(GLfloat));
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                             nullptr);
    f->glEnableVertexAttribArray(0);
    _vbo.release();
    _program->release();
}

//----------------------------------------------------------------------------
// paintGL
//----------------------------------------------------------------------------
void Scope::paintGL()
{
    // Clear the scope
    glClear(GL_COLOR_BUFFER_BIT);

    // Update our line VBO vertices and draw it
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
     QOpenGLVertexArrayObject::Binder vao_binder(&_vao);
    _program->bind();
    _vbo.bind();
    _vbo.allocate(_vertices, (_num_samples * 3) * sizeof(GLfloat));
    _vbo.release();
    _program->setUniformValue(_colour_loc, QVector4D(_colour.redF(), _colour.greenF(), _colour.blueF(), _alpha));
    f->glLineWidth(_pen_width);
    glDrawArrays(GL_LINE_STRIP, 0, _num_samples);
    _program->release();
}
