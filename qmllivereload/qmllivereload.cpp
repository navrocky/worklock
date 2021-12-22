#include "qmllivereload.h"

#include <QLoggingCategory>
#include <QQuickWindow>
#include <QRect>

Q_LOGGING_CATEGORY(qmlLiveReload, "QmlLiveReload")

struct WindowState
{
    QPointer<QQuickWindow> window;
    QRect geometry;
    Qt::WindowFlags flags;
};

QmlLiveReload::QmlLiveReload(QQmlApplicationEngine* engine, const QString& url,
                             SourceFilesWatcher* sourceWatcher, QObject* parent)
    : QObject(parent)
    , engine_(engine)
    , sourceWatcher_(sourceWatcher)
    , url_(url)
    , forceStayOnTop_(false)
    , started_(false)
{
    connect(sourceWatcher, &SourceFilesWatcher::filesChanged, this, &QmlLiveReload::sourcesChanged);
    QObject::connect(engine_, &QQmlApplicationEngine::objectCreated, this,
                     &QmlLiveReload::qmlObjectCreated, Qt::QueuedConnection);
}

void QmlLiveReload::start()
{
    started_ = true;
    reload();
}

void QmlLiveReload::sourcesChanged(const QStringList& urls)
{
    qDebug() << "Sources changed:" << urls;
    reload();
}

void QmlLiveReload::reload()
{
    if (existingWindow_)
    {
        windowState_ = std::make_shared<WindowState>();
        windowState_->geometry = existingWindow_->geometry();
        windowState_->flags = existingWindow_->flags();
        delete existingWindow_;
    }
    engine_->clearComponentCache();
    engine_->load(url_);
}

void QmlLiveReload::qmlObjectCreated(QObject* obj, const QUrl& objUrl)
{
    qCDebug(qmlLiveReload) << "Object created:" << objUrl << obj;
    auto quickWindow = qobject_cast<QQuickWindow*>(obj);
    if (quickWindow)
    {
        auto flags = quickWindow->flags();
        if (windowState_)
        {
            quickWindow->setGeometry(windowState_->geometry);
            flags = windowState_->flags;
        }
        if (forceStayOnTop_)
            flags = flags | Qt::WindowStaysOnTopHint;
        quickWindow->setFlags(flags);
        existingWindow_ = quickWindow;
    }
}

void QmlLiveReload::setForceStayOnTop(bool forceStayOnTop)
{
    if (forceStayOnTop_ == forceStayOnTop)
        return;
    forceStayOnTop_ = forceStayOnTop;
    if (started_)
        reload();
}
