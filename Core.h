#pragma once

#include <QObject>
#include <QString>
#include <memory>

namespace Pulse {

// Forward declarations
class Config;
class PluginManager;
class Logger;
class DBusInterface;

class Core : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString version READ version CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    
public:
    enum class State {
        Uninitialized,
        Initializing,
        Running,
        ShuttingDown,
        Error
    };
    Q_ENUM(State)
    
    // Singleton instance
    static Core* instance();
    
    // Initialize the core system
    bool initialize(const QStringList& args = QStringList());
    
    // Shutdown the core system
    void shutdown();
    
    // Accessors
    QString version() const { return "0.1.0"; }
    QString name() const { return "Pulse Desktop"; }
    State state() const { return m_state; }
    
    // Subsystem access
    Config* config() const;
    PluginManager* pluginManager() const;
    Logger* logger() const;
    DBusInterface* dbus() const;
    
signals:
    void stateChanged(State newState);
    void initialized();
    void aboutToShutdown();
    
private:
    explicit Core(QObject* parent = nullptr);
    ~Core();
    
    void setState(State state);
    
    class Private;
    std::unique_ptr<Private> d;
    State m_state = State::Uninitialized;
};

} // namespace Pulse
