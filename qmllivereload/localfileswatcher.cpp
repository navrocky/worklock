#include "localfileswatcher.h"

#include <QLoggingCategory>
#include <QTimer>

namespace
{
Q_LOGGING_CATEGORY(logger, "LocalFilesWatcher")
}

LocalFilesWatcher::LocalFilesWatcher(const QString& rootDir, const QRegularExpression& filesPattern,
                                     QObject* parent)
    : SourceFilesWatcher(parent)
    , fileSystemWatcher_(new RecursiveDirWatcher(rootDir, filesPattern, this))
    , timer_(new QTimer(this))
{
    qCDebug(logger) << "Initialized with rootDir:" << rootDir;
    connect(fileSystemWatcher_, &RecursiveDirWatcher::fileChanges, this,
            &LocalFilesWatcher::filesChangedSlot);
    connect(timer_, &QTimer::timeout, this, &LocalFilesWatcher::emitSignal);
    timer_->setInterval(500);
}

void LocalFilesWatcher::filesChangedSlot(const QList<RecursiveDirWatcher::FileChange>& changes)
{
    foreach (const auto& change, changes)
    {
        changedFiles_ << change.path;
    }
    timer_->stop();
    timer_->start();
}

void LocalFilesWatcher::emitSignal()
{
    emit filesChanged(changedFiles_);
    changedFiles_.clear();
    timer_->stop();
}
