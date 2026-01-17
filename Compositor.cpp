#include "Compositor.h"
#include <QDebug>

namespace Pulse {

Compositor::Compositor(QObject* parent)
    : QWaylandCompositor(parent)
    , m_windowManager(new WindowManager(this)) {
    
    qDebug() << "Pulse Compositor initialized";
    
    // Connect signals
    connect(this, &QWaylandCompositor::surfaceCreated,
            this, &Compositor::onSurfaceCreated);
    
    connect(m_windowManager, &WindowManager::windowAdded,
            this, &Compositor::onWindowAdded);
    connect(m_windowManager, &WindowManager::windowRemoved,
            this, &Compositor::onWindowRemoved);
}

Compositor::~Compositor() {
    qDebug() << "Pulse Compositor shutting down";
}

void Compositor::onSurfaceCreated(QWaylandSurface* surface) {
    qDebug() << "Surface created for process:" << surface->client()->processId();
    
    // Create window for this surface
    Window* window = m_windowManager->createWindow(surface);
    
    connect(surface, &QWaylandSurface::destroyed,
            this, &Compositor::onSurfaceDestroyed);
    
    if (window) {
        qDebug() << "Window created for surface:" << window->title();
    }
}

void Compositor::onSurfaceDestroyed() {
    auto* surface = qobject_cast<QWaylandSurface*>(sender());
    if (surface) {
        Window* window = m_windowManager->windowForSurface(surface);
        if (window) {
            m_windowManager->destroyWindow(window);
        }
    }
}

void Compositor::onWindowAdded(Window* window) {
    qDebug() << "Window added to compositor:" << window->title()
             << "Total windows:" << m_windowManager->windowCount();
}

void Compositor::onWindowRemoved(Window* window) {
    qDebug() << "Window removed from compositor:" << window->title()
             << "Remaining windows:" << m_windowManager->windowCount();
}

void Compositor::closeActiveWindow() {
    if (m_windowManager->activeWindow()) {
        m_windowManager->closeWindow(m_windowManager->activeWindow());
    }
}

void Compositor::toggleMaximizeActiveWindow() {
    if (m_windowManager->activeWindow()) {
        m_windowManager->toggleMaximize(m_windowManager->activeWindow());
    }
}

void Compositor::arrangeWindows() {
    m_windowManager->arrangeWindows();
}

void Compositor::tileWindows() {
    m_windowManager->tileWindows();
}

void Compositor::cascadeWindows() {
    m_windowManager->cascadeWindows();
}

} // namespace Pulse
