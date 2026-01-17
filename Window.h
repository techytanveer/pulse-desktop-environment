#pragma once

#include <QObject>
#include <QWaylandSurface>
#include <QRect>
#include <QSharedPointer>

namespace Pulse {

class Window : public QObject {
    Q_OBJECT
    Q_PROPERTY(QRect geometry READ geometry NOTIFY geometryChanged)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(bool focused READ focused NOTIFY focusedChanged)
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    
public:
    enum class State {
        Normal,
        Maximized,
        Minimized,
        Fullscreen
    };
    Q_ENUM(State)
    
    explicit Window(QWaylandSurface* surface, QObject* parent = nullptr);
    ~Window();
    
    // Getters
    QWaylandSurface* surface() const { return m_surface; }
    QRect geometry() const { return m_geometry; }
    QString title() const;
    bool focused() const { return m_focused; }
    State state() const { return m_state; }
    
    // Window management
    void setGeometry(const QRect& geometry);
    void setFocused(bool focused);
    void setState(State state);
    void close();
    void raise();
    
    // Decorations
    int borderSize() const { return 1; }
    int titleBarHeight() const { return 30; }
    QRect clientArea() const;
    
signals:
    void geometryChanged(const QRect& geometry);
    void titleChanged(const QString& title);
    void focusedChanged(bool focused);
    void stateChanged(State state);
    void closed();
    
private:
    QWaylandSurface* m_surface = nullptr;
    QRect m_geometry;
    bool m_focused = false;
    State m_state = State::Normal;
    quint32 m_id = 0;
    
    static quint32 s_nextId;
};

using WindowPtr = QSharedPointer<Window>;

} // namespace Pulse
