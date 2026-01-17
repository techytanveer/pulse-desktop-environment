#pragma once

#include <QObject>
#include <QString>
#include <memory>

namespace Pulse {

enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error,
    Critical
};

class Logger : public QObject {
    Q_OBJECT
    
public:
    explicit Logger(QObject* parent = nullptr);
    ~Logger();
    
    bool initialize();
    void shutdown();
    
    // Logging methods
    void debug(const QString& message, const QString& component = QString());
    void info(const QString& message, const QString& component = QString());
    void warning(const QString& message, const QString& component = QString());
    void error(const QString& message, const QString& component = QString());
    void critical(const QString& message, const QString& component = QString());
    
    // Configuration
    void setLogLevel(LogLevel level);
    void setLogFile(const QString& filePath);
    void enableConsoleOutput(bool enabled);
    
signals:
    void messageLogged(Pulse::LogLevel level, const QString& message, const QString& component);
    
private:
    class Private;
    std::unique_ptr<Private> d;
    
    void log(LogLevel level, const QString& message, const QString& component);
};

} // namespace Pulse
