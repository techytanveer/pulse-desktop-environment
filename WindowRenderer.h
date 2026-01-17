#pragma once

#include <QObject>
#include <QQuickItem>
#include <QPointer>
#include "Window.h"

namespace Pulse {

class WindowRenderer : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(Window* window READ window WRITE setWindow NOTIFY windowChanged)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged)
    Q_PROPERTY(QColor titleBarColor READ titleBarColor WRITE setTitleBarColor NOTIFY titleBarColorChanged)
    
public:
    explicit WindowRenderer(QQuickItem* parent = nullptr);
    ~WindowRenderer();
    
    Window* window() const { return m_window; }
    void setWindow(Window* window);
    
    QColor borderColor() const { return m_borderColor; }
    void setBorderColor(const QColor& color);
    
    QColor titleBarColor() const { return m_titleBarColor; }
    void setTitleBarColor(const QColor& color);
    
signals:
    void windowChanged(Window* window);
    void borderColorChanged(const QColor& color);
    void titleBarColorChanged(const QColor& color);
    void closeClicked();
    void minimizeClicked();
    void maximizeClicked();
    
protected:
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* data) override;
    
private slots:
    void updateGeometry();
    void updateFocus();
    
private:
    QPointer<Window> m_window;
    QColor m_borderColor = Qt::gray;
    QColor m_titleBarColor = Qt::darkGray;
    
    void drawWindowDecorations(QSGNode* node);
};

} // namespace Pulse
