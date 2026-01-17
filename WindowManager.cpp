#include "WindowManager.h"
#include <QWaylandSurface>
#include <QDebug>
#include <algorithm>

namespace Pulse {

WindowManager::WindowManager(QObject* parent)
    : QObject(parent) {
    qDebug() << "WindowManager initialized";
}

WindowManager::~WindowManager() {
    // Clean up all windows
    for (auto window : m_windows) {
        delete window;
    }
    m_windows.clear();
}

Window* WindowManager::createWindow(QWaylandSurface* surface) {
    if (!surface) {
        qWarning() << "Cannot create window for null surface";
        return nullptr;
    }
    
    // Check if window already exists for this surface
    for (auto window : m_windows) {
        if (window->surface() == surface) {
            return window;
        }
    }
    
    // Create new window
    Window* window = new Window(surface, this);
    m_windows.insert(window->surface()->client()->processId(), window);
    
    // Set initial position (cascade)
    static int cascadeOffset = 30;
    QRect geometry = window->geometry();
    geometry.moveTopLeft(QPoint(cascadeOffset, cascadeOffset));
    window->setGeometry(geometry);
    
    cascadeOffset += 30;
    if (cascadeOffset > 200) cascadeOffset = 30;
    
    // Make it active
    setActiveWindow(window);
    
    qDebug() << "Window created, total:" << m_windows.size();
    emit windowAdded(window);
    emit windowCountChanged(m_windows.size());
    
    return window;
}

void WindowManager::destroyWindow(Window* window) {
    if (!window || !m_windows.values().contains(window)) {
        return;
    }
    
    quint32 keyToRemove = 0;
    for (auto it = m_windows.begin(); it != m_windows.end(); ++it) {
        if (it.value() == window) {
            keyToRemove = it.key();
            break;
        }
    }
    
    if (keyToRemove) {
        m_windows.remove(keyToRemove);
        
        if (m_activeWindow == window) {
            m_activeWindow = nullptr;
            if (!m_windows.isEmpty()) {
                setActiveWindow(m_windows.first());
            }
        }
        
        qDebug() << "Window destroyed, remaining:" << m_windows.size();
        emit windowRemoved(window);
        emit windowCountChanged(m_windows.size());
        
        window->deleteLater();
    }
}

Window* WindowManager::windowForSurface(QWaylandSurface* surface) const {
    for (auto window : m_windows) {
        if (window->surface() == surface) {
            return window;
        }
    }
    return nullptr;
}

void WindowManager::setActiveWindow(Window* window) {
    if (m_activeWindow == window) {
        return;
    }
    
    // Deactivate current window
    if (m_activeWindow) {
        m_activeWindow->setFocused(false);
    }
    
    // Activate new window
    m_activeWindow = window;
    if (m_activeWindow) {
        m_activeWindow->setFocused(true);
        m_activeWindow->raise();
    }
    
    emit activeWindowChanged(m_activeWindow);
}

void WindowManager::closeWindow(Window* window) {
    if (window) {
        window->close();
        // In real implementation, we'd wait for client to close
        destroyWindow(window);
    }
}

void WindowManager::minimizeWindow(Window* window) {
    if (window) {
        window->setState(Window::State::Minimized);
    }
}

void WindowManager::maximizeWindow(Window* window) {
    if (window) {
        window->setState(Window::State::Maximized);
        // In real implementation, set geometry to screen size
    }
}

void WindowManager::toggleMaximize(Window* window) {
    if (!window) return;
    
    if (window->state() == Window::State::Maximized) {
        window->setState(Window::State::Normal);
    } else {
        maximizeWindow(window);
    }
}

void WindowManager::moveWindow(Window* window, const QPoint& delta) {
    if (window) {
        QRect geometry = window->geometry();
        geometry.translate(delta);
        window->setGeometry(geometry);
    }
}

void WindowManager::resizeWindow(Window* window, const QSize& delta, Qt::Edges edges) {
    if (!window) return;
    
    QRect geometry = window->geometry();
    
    if (edges & Qt::LeftEdge) {
        geometry.setLeft(geometry.left() + delta.width());
    }
    if (edges & Qt::RightEdge) {
        geometry.setRight(geometry.right() + delta.width());
    }
    if (edges & Qt::TopEdge) {
        geometry.setTop(geometry.top() + delta.height());
    }
    if (edges & Qt::BottomEdge) {
        geometry.setBottom(geometry.bottom() + delta.height());
    }
    
    // Ensure minimum size
    if (geometry.width() < 100) geometry.setWidth(100);
    if (geometry.height() < 100) geometry.setHeight(100);
    
    window->setGeometry(geometry);
}

Window* WindowManager::windowAt(const QPoint& pos) const {
    // Check from top-most to bottom-most
    QList<Window*> windows = m_windows.values();
    std::reverse(windows.begin(), windows.end());
    
    for (Window* window : windows) {
        if (window->geometry().contains(pos)) {
            return window;
        }
    }
    
    return nullptr;
}

void WindowManager::arrangeWindows() {
    // Simple vertical arrangement
    int y = 50;
    for (Window* window : m_windows) {
        QRect geometry = window->geometry();
        geometry.moveTopLeft(QPoint(50, y));
        window->setGeometry(geometry);
        y += geometry.height() + 20;
    }
}

void WindowManager::tileWindows() {
    if (m_windows.isEmpty()) return;
    
    // Simple 2-column tiling
    int count = m_windows.size();
    int columns = qMin(2, count);
    int rows = (count + columns - 1) / columns;
    
    int screenWidth = 1920;  // Should get from output
    int screenHeight = 1080;
    
    int colWidth = screenWidth / columns;
    int rowHeight = screenHeight / rows;
    
    int i = 0;
    for (Window* window : m_windows) {
        int col = i % columns;
        int row = i / columns;
        
        QRect geometry(col * colWidth, 
                      row * rowHeight,
                      colWidth - 10,
                      rowHeight - 10);
        
        window->setGeometry(geometry);
        i++;
    }
}

void WindowManager::cascadeWindows() {
    int offset = 30;
    for (Window* window : m_windows) {
        QRect geometry = window->geometry();
        geometry.moveTopLeft(QPoint(offset, offset));
        window->setGeometry(geometry);
        offset += 30;
    }
}

void WindowManager::updateWindowStack() {
    // Implementation for z-order management
    // This would manage window stacking order
}

} // namespace Pulse
