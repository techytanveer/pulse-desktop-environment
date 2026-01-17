#include "Window.h"
#include <QWaylandSurface>
#include <QDebug>

namespace Pulse {

quint32 Window::s_nextId = 1;

Window::Window(QWaylandSurface* surface, QObject* parent)
    : QObject(parent)
    , m_surface(surface)
    , m_id(s_nextId++) {
    
    // Initial geometry
    m_geometry = QRect(100, 100, 800, 600);
    
    qDebug() << "Window created:" << m_id << "for surface:" << surface;
}

Window::~Window() {
    qDebug() << "Window destroyed:" << m_id;
}

QString Window::title() const {
    if (!m_surface) return "Untitled";
    
    // Try to get title from surface
    // In real implementation, this would come from xdg-toplevel
    return QString("Window %1 - PID %2").arg(m_id).arg(m_surface->client()->processId());
}

void Window::setGeometry(const QRect& geometry) {
    if (m_geometry != geometry) {
        m_geometry = geometry;
        emit geometryChanged(geometry);
        qDebug() << "Window" << m_id << "geometry changed:" << geometry;
    }
}

void Window::setFocused(bool focused) {
    if (m_focused != focused) {
        m_focused = focused;
        emit focusedChanged(focused);
        qDebug() << "Window" << m_id << "focus:" << (focused ? "gained" : "lost");
    }
}

void Window::setState(State state) {
    if (m_state != state) {
        m_state = state;
        emit stateChanged(state);
        qDebug() << "Window" << m_id << "state changed to:" << static_cast<int>(state);
    }
}

void Window::close() {
    if (m_surface) {
        // Send close request to client
        // For now, just destroy
        qDebug() << "Closing window:" << m_id;
        emit closed();
    }
}

void Window::raise() {
    qDebug() << "Raising window:" << m_id;
    // Implementation would reorder window stack
}

QRect Window::clientArea() const {
    return QRect(m_geometry.x() + borderSize(),
                 m_geometry.y() + titleBarHeight(),
                 m_geometry.width() - 2 * borderSize(),
                 m_geometry.height() - titleBarHeight() - borderSize());
}

} // namespace Pulse
