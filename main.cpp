#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <localfileswatcher.h>
#include <qmllivereload.h>

#define VAL(str) #str
#define TOSTRING(str) VAL(str)

int main(int argc, char* argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

#ifdef NDEBUG
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

#else
    const auto sourcesRoot = QString::fromLocal8Bit(TOSTRING(SOURCES_ROOT));
    LocalFilesWatcher filesWatcher(sourcesRoot, QRegularExpression("\\.qml$"));
    QmlLiveReload liveReload(&engine, sourcesRoot + "/main.qml", &filesWatcher);
    liveReload.setForceStayOnTop(true);
    liveReload.start();
#endif

    return app.exec();
}
