#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>
#include <vector>
#include "texture.h"
#include "camera.h"
#include "matrix4x4.h"
#include "visualobject.h"
#include "input.h"

class QOpenGLContext;
class Shader;
class MainWindow;

enum ballDirection
{
    UP = 0,
    DOWN  = 1,
    LEFT  = 2,
    RIGHT  = 3
};

/// This inherits from QWindow to get access to the Qt functionality and
/// OpenGL surface.
/// We also inherit from QOpenGLFunctions, to get access to the OpenGL functions
/// This is the same as using glad and glw from general OpenGL tutorials
class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow() override;

    QOpenGLContext *context() { return mContext; }

    void exposeEvent(QExposeEvent *) override;
    void toggleWireframe();

    void checkForGLerrors();

    static std::vector<gsl::Vector3D> mapToGrid(const std::vector<gsl::Vector3D> &points, int xGrid, int zGrid, gsl::Vector3D min, gsl::Vector3D max);
    static float length(const gsl::Vector3D &a, const gsl::Vector3D &b);


    void moveBall(float deltaTime);
    void inputMoveBall(ballDirection direction, float deltaTime);
private slots:
    void render();

private:
    void init();
    void setCameraSpeed(float value);

    QOpenGLContext *mContext{nullptr};
    bool mInitialized{false};

    Texture *mTexture[4]{nullptr}; //We can hold 4 textures
    Shader *mShaderProgram[4]{nullptr}; //We can hold 4 shaders

    void setupPlainShader(int shaderIndex);
    GLint mMatrixUniform0{-1};
    GLint vMatrixUniform0{-1};
    GLint pMatrixUniform0{-1};

    void setupTextureShader(int shaderIndex);
    GLint mMatrixUniform1{-1};
    GLint vMatrixUniform1{-1};
    GLint pMatrixUniform1{-1};
    GLint mTextureUniform{-1};

    std::vector<VisualObject*> mVisualObjects;
    std::vector<Vertex> mTerrainVertices;

    std::vector<Triangle> mTerrainTriangles;
    GLuint mTerrainVAO;
    float getTerrainHeight(gsl::Vector3D inLocation);

    //For player movement
    gsl::Vector3D playerCoords;
    float playerHeight{0.f};

    std::pair<bool, gsl::vec3> isColliding(VisualObject *ball, float ballRadius);
    gsl::Vector3D projectPlane(VisualObject *ball, float ballRadius);
    Triangle* getBallToPlaneTriangle(gsl::vec3 ballPos);

    Camera *mCurrentCamera{nullptr};

    bool mWireframe{false};

    float mSimulationTime;

    Input mInput;
    float mCameraSpeed{0.05f};
    float mCameraRotateSpeed{0.1f};
    int mMouseXlast{0};
    int mMouseYlast{0};

    QTimer *mRenderTimer{nullptr};  //timer that drives the gameloop
    QElapsedTimer mTimeStart;       //time variable that reads the actual FPS

    float mAspectratio{1.f};

    MainWindow *mMainWindow{nullptr};    //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};

    void calculateFramerate();

    void startOpenGLDebugger();

    void handleInput();

protected:
    //The QWindow that we inherit from has these functions to capture
    // mouse and keyboard. Uncomment to use
    //
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
};

#endif // RENDERWINDOW_H
