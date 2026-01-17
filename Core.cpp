#include "Core.h"
#include "Logger.h"
#include "../pulse-config/Config.h"
#include "../pulse-ipc/DBusInterface.h"
#include "../pulse-plugins/PluginManager.h"

#include <QDebug>

namespace Pulse {

class Core::Private {
public:
    std::unique_ptr<Logger> logger;
    std::unique_ptr<Config> config;
    std::unique_ptr<DBusInterface> dbus;
    std::unique_ptr<PluginManager> pluginManager;
};

Core* Core::instance() {
    static Core instance;
    return &instance;
}

Core::Core(QObject* parent) 
    : QObject(parent)
    , d(std::make_unique<Private>()) {
    
    // Create subsystems
    d->logger = std::make_unique<Logger>();
    d->config = std::make_unique<Config>();
    d->dbus = std::make_unique<DBusInterface>();
    d->pluginManager = std::make_unique<PluginManager>();
}

Core::~Core() {
    shutdown();
}

bool Core::initialize(const QStringList& args) {
    if (m_state != State::Uninitialized) {
        qWarning() << "Core already initialized";
        return false;
    }
    
    setState(State::Initializing);
    
    // Initialize subsystems in order
    if (!d->logger->initialize()) {
        qCritical() << "Failed to initialize logger";
        setState(State::Error);
        return false;
    }
    
    if (!d->config->initialize()) {
        qCritical() << "Failed to initialize config";
        setState(State::Error);
        return false;
    }
    
    if (!d->dbus->initialize()) {
        qWarning() << "Failed to initialize DBus (continuing without it)";
        // DBus is optional for now
    }
    
    if (!d->pluginManager->initialize()) {
        qWarning() << "Failed to initialize plugin manager";
        // Plugin manager is optional for now
    }
    
    setState(State::Running);
    emit initialized();
    
    d->logger->info("Pulse Core initialized successfully");
    return true;
}

void Core::shutdown() {
    if (m_state != State::Running) {
        return;
    }
    
    emit aboutToShutdown();
    setState(State::ShuttingDown);
    
    // Shutdown in reverse order
    d->pluginManager->shutdown();
    d->dbus->shutdown();
    d->config->shutdown();
    d->logger->shutdown();
    
    setState(State::Uninitialized);
}

void Core::setState(State state) {
    if (m_state != state) {
        m_state = state;
        emit stateChanged(state);
    }
}

Config* Core::config() const {
    return d->config.get();
}

PluginManager* Core::pluginManager() const {
    return d->pluginManager.get();
}

Logger* Core::logger() const {
    return d->logger.get();
}

DBusInterface* Core::dbus() const {
    return d->dbus.get();
}

} // namespace Pulse
