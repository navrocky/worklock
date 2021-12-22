#pragma once

#include <QPointer>
#include <QQmlApplicationEngine>
#include <memory>

#include "sourcefileswatcher.h"

struct WindowState;
class QQuickWindow;

class QmlLiveReload : public QObject
{
    Q_OBJECT
public:
    QmlLiveReload(QQmlApplicationEngine* engine, const QString& url,
                  SourceFilesWatcher* sourceWatcher, QObject* parent = 0);

    void start();

    void setForceStayOnTop(bool forceStayOnTop);

private:
    void sourcesChanged(const QStringList&);
    void reload();
    void qmlObjectCreated(QObject* obj, const QUrl& objUrl);

    QQmlApplicationEngine* engine_;
    SourceFilesWatcher* sourceWatcher_;
    QString url_;
    QPointer<QQuickWindow> existingWindow_;
    std::shared_ptr<WindowState> windowState_;
    bool forceStayOnTop_;
    bool started_;
};
