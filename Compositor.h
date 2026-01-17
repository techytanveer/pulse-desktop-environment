#pragma once

#include <QWaylandCompositor>
#include <QWaylandSurface>
#include "WindowManager.h"

namespace Pulse {

class Compositor : public QWaylandCompositor {
    Q_OBJECT
    
public:
    explicit Compositor(QObject* parent = nullptr);
    ~Compositor();
    
    WindowManager* windowManager() const { return m_windowManager; }
    
public slots:
    void closeActiveWindow();
    void toggleMaximizeActiveWindow();
    void arrangeWindows();
    void tileWindows();
    void cascadeWindows();
    
private slots:
    void onSurfaceCreated(QWaylandSurface* surface);
    void onSurfaceDestroyed();
    void onWindowAdded(Window* window);
    void onWindowRemoved(Window* window);
    
private:
    WindowManager* m_windowManager;
};

} // namespace Pulse
