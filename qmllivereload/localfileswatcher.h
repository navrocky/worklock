#pragma once

#include <QRegularExpression>

#include "recursivedirwatcher.h"
#include "sourcefileswatcher.h"

class QTimer;

class LocalFilesWatcher : public SourceFilesWatcher
{
    Q_OBJECT
public:
    LocalFilesWatcher(const QString& rootDir, const QRegularExpression& filesPattern,
                      QObject* parent = 0);

private:
    void filesChangedSlot(const QList<RecursiveDirWatcher::FileChange>& changes);
    void emitSignal();

    RecursiveDirWatcher* fileSystemWatcher_;
    QStringList changedFiles_;
    QTimer* timer_;
};
