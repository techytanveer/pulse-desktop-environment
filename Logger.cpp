#include "Logger.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <iostream>

namespace Pulse {

class Logger::Private {
public:
    LogLevel currentLevel = LogLevel::Info;
    QString logFilePath;
    QFile logFile;
    QTextStream fileStream;
    bool consoleOutput = true;
    
    QString levelToString(LogLevel level) {
        switch(level) {
            case LogLevel::Debug: return "DEBUG";
            case LogLevel::Info: return "INFO";
            case LogLevel::Warning: return "WARNING";
            case LogLevel::Error: return "ERROR";
            case LogLevel::Critical: return "CRITICAL";
            default: return "UNKNOWN";
        }
    }
};

Logger::Logger(QObject* parent)
    : QObject(parent)
    , d(std::make_unique<Private>()) {
}

Logger::~Logger() {
    shutdown();
}

bool Logger::initialize() {
    if (d->logFile.isOpen()) {
        d->logFile.close();
    }
    
    if (!d->logFilePath.isEmpty()) {
        d->logFile.setFileName(d->logFilePath);
        if (d->logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            d->fileStream.setDevice(&d->logFile);
            info("Logger initialized with file: " + d->logFilePath);
            return true;
        } else {
            std::cerr << "Failed to open log file: " << d->logFilePath.toStdString() << std::endl;
        }
    }
    
    info("Logger initialized (console output only)");
    return true;
}

void Logger::shutdown() {
    if (d->logFile.isOpen()) {
        info("Logger shutting down");
        d->logFile.close();
    }
}

void Logger::debug(const QString& message, const QString& component) {
    log(LogLevel::Debug, message, component);
}

void Logger::info(const QString& message, const QString& component) {
    log(LogLevel::Info, message, component);
}

void Logger::warning(const QString& message, const QString& component) {
    log(LogLevel::Warning, message, component);
}

void Logger::error(const QString& message, const QString& component) {
    log(LogLevel::Error, message, component);
}

void Logger::critical(const QString& message, const QString& component) {
    log(LogLevel::Critical, message, component);
}

void Logger::setLogLevel(LogLevel level) {
    d->currentLevel = level;
}

void Logger::setLogFile(const QString& filePath) {
    d->logFilePath = filePath;
}

void Logger::enableConsoleOutput(bool enabled) {
    d->consoleOutput = enabled;
}

void Logger::log(LogLevel level, const QString& message, const QString& component) {
    if (level < d->currentLevel) {
        return;
    }
    
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
    QString levelStr = d->levelToString(level);
    QString logMessage;
    
    if (!component.isEmpty()) {
        logMessage = QString("[%1] [%2] [%3] %4")
            .arg(timestamp, levelStr, component, message);
    } else {
        logMessage = QString("[%1] [%2] %3")
            .arg(timestamp, levelStr, message);
    }
    
    // Console output
    if (d->consoleOutput) {
        std::ostream& stream = (level >= LogLevel::Warning) ? std::cerr : std::cout;
        stream << logMessage.toStdString() << std::endl;
    }
    
    // File output
    if (d->logFile.isOpen()) {
        d->fileStream << logMessage << "\n";
        d->fileStream.flush();
    }
    
    // Signal for GUI log viewers
    emit messageLogged(level, message, component);
}

} // namespace Pulse
