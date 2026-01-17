#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    
    app.setApplicationName("Pulse Shell");
    app.setOrganizationName("Pulse Desktop");
    
    QQmlApplicationEngine engine;
    engine.load(QUrl::fromLocalFile("/home/developer/pulse/src/shell/pulse-shell-src/main.qml"));
    
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }
    
    return app.exec();
}
