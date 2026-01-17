#include "WindowRenderer.h"
#include <QSGSimpleRectNode>
#include <QSGSimpleTextureNode>
#include <QQuickWindow>
#include <QDebug>

namespace Pulse {

WindowRenderer::WindowRenderer(QQuickItem* parent)
    : QQuickItem(parent) {
    setFlag(ItemHasContents, true);
    setAcceptedMouseButtons(Qt::AllButtons);
}

WindowRenderer::~WindowRenderer() {
}

void WindowRenderer::setWindow(Window* window) {
    if (m_window == window) return;
    
    // Disconnect old window
    if (m_window) {
        disconnect(m_window, &Window::geometryChanged,
                   this, &WindowRenderer::updateGeometry);
        disconnect(m_window, &Window::focusedChanged,
                   this, &WindowRenderer::updateFocus);
    }
    
    m_window = window;
    
    // Connect new window
    if (m_window) {
        connect(m_window, &Window::geometryChanged,
                this, &WindowRenderer::updateGeometry);
        connect(m_window, &Window::focusedChanged,
                this, &WindowRenderer::updateFocus);
        
        // Initial update
        updateGeometry();
        updateFocus();
    }
    
    emit windowChanged(window);
    update();
}

void WindowRenderer::setBorderColor(const QColor& color) {
    if (m_borderColor != color) {
        m_borderColor = color;
        emit borderColorChanged(color);
        update();
    }
}

void WindowRenderer::setTitleBarColor(const QColor& color) {
    if (m_titleBarColor != color) {
        m_titleBarColor = color;
        emit titleBarColorChanged(color);
        update();
    }
}

QSGNode* WindowRenderer::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* data) {
    Q_UNUSED(data)
    
    if (!m_window) {
        delete oldNode;
        return nullptr;
    }
    
    // Create or reuse node
    QSGNode* parentNode = oldNode;
    if (!parentNode) {
        parentNode = new QSGNode;
    }
    
    // Clear existing children
    while (parentNode->firstChild()) {
        parentNode->removeChildNode(parentNode->firstChild());
    }
    
    // Draw window decorations
    drawWindowDecorations(parentNode);
    
    return parentNode;
}

void WindowRenderer::drawWindowDecorations(QSGNode* parentNode) {
    if (!m_window) return;
    
    // Draw border
    auto* borderNode = new QSGSimpleRectNode;
    borderNode->setRect(boundingRect());
    borderNode->setColor(m_window->focused() ? QColor("#4a90e2") : m_borderColor);
    parentNode->appendChildNode(borderNode);
    
    // Draw title bar
    QRect titleRect(0, 0, width(), m_window->titleBarHeight());
    auto* titleNode = new QSGSimpleRectNode;
    titleNode->setRect(titleRect);
    titleNode->setColor(m_window->focused() ? QColor("#357ae8") : m_titleBarColor);
    parentNode->appendChildNode(titleNode);
    
    // Draw client area (simulated content)
    QRect clientRect = m_window->clientArea();
    clientRect.moveTopLeft(QPoint(m_window->borderSize(), m_window->titleBarHeight()));
    auto* clientNode = new QSGSimpleRectNode;
    clientNode->setRect(QRectF(clientRect));
    clientNode->setColor(Qt::white);
    parentNode->appendChildNode(clientNode);
}

void WindowRenderer::updateGeometry() {
    if (!m_window) return;
    
    QRect geometry = m_window->geometry();
    setX(geometry.x());
    setY(geometry.y());
    setWidth(geometry.width());
    setHeight(geometry.height());
    
    update();
}

void WindowRenderer::updateFocus() {
    update();
}

} // namespace Pulse
