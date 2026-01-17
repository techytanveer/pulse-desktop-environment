#pragma once

#include "Window.h"
#include <QObject>
#include <QList>
#include <QMap>

namespace Pulse {

class WindowManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(int windowCount READ windowCount NOTIFY windowCountChanged)
    Q_PROPERTY(Window* activeWindow READ activeWindow NOTIFY activeWindowChanged)
    
public:
    explicit WindowManager(QObject* parent = nullptr);
    ~WindowManager();
    
    // Window management
    Window* createWindow(QWaylandSurface* surface);
    void destroyWindow(Window* window);
    Window* windowForSurface(QWaylandSurface* surface) const;
    
    // Window operations
    void setActiveWindow(Window* window);
    void closeWindow(Window* window);
    void minimizeWindow(Window* window);
    void maximizeWindow(Window* window);
    void toggleMaximize(Window* window);
    void moveWindow(Window* window, const QPoint& delta);
    void resizeWindow(Window* window, const QSize& delta, Qt::Edges edges);
    
    // Getters
    int windowCount() const { return m_windows.size(); }
    QList<Window*> windows() const { return m_windows.values(); }
    Window* activeWindow() const { return m_activeWindow; }
    Window* windowAt(const QPoint& pos) const;
    
    // Layout
    void arrangeWindows();
    void tileWindows();
    void cascadeWindows();
    
signals:
    void windowAdded(Window* window);
    void windowRemoved(Window* window);
    void activeWindowChanged(Window* window);
    void windowCountChanged(int count);
    
private:
    QMap<quint32, Window*> m_windows;
    Window* m_activeWindow = nullptr;
    
    void updateWindowStack();
};

} // namespace Pulse
