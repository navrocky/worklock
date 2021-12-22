#include "recursivedirwatcher.h"

#include <QDir>
#include <QFileSystemWatcher>
#include <QSet>

RecursiveDirWatcher::RecursiveDirWatcher(const QString& path,
                                         const QRegularExpression& filesPattern, QObject* parent)
    : QObject(parent)
    , path_(path)
    , filesPattern_(filesPattern)
    , watcher_(new QFileSystemWatcher(this))
{
    connect(watcher_, &QFileSystemWatcher::directoryChanged, this,
            &RecursiveDirWatcher::dirChanged);
    watcher_->addPath(path);

    auto entries = getEntries(path);
    foreach (const auto& file, entries)
    {
        if (file.isDir() || isMatchPattern(file.fileName()))
            files_.insert(file.fileName(), makeFileInfo(file));
    }
}

void RecursiveDirWatcher::dirChanged(const QString&)
{
    auto entries = getEntries(path_);
    const auto& keys = files_.keys();
    auto keysToDelete = QSet<QString>(keys.begin(), keys.end());

    QList<FileChange> changes;

    foreach (const auto& file, entries)
    {
        const auto& fileName = file.fileName();
        if (!file.isDir() && !isMatchPattern(fileName))
            continue;
        keysToDelete.remove(fileName);
        const auto& it = files_.find(fileName);
        if (it != files_.end())
        {
            // existing file
            if (it->lastModified != file.lastModified())
            {
                // changed
                FileChange change;
                change.path = it->path;
                change.type = Modify;
                changes.append(change);

                it->lastModified = file.lastModified();
            }
        }
        else
        {
            // new file
            const auto& fi = makeFileInfo(file);
            files_.insert(fileName, fi);

            FileChange change;
            change.path = fi.path;
            change.type = Create;
            changes.append(change);
        }
    }

    foreach (const auto& key, keysToDelete)
    {
        // deleted file
        const auto& it = files_.find(key);
        if (it != files_.end())
        {
            FileChange change;
            change.type = Delete;
            change.path = it->path;
            changes.append(change);

            files_.remove(key);
        }
    }

    if (!changes.isEmpty())
        emit fileChanges(changes);
}

bool RecursiveDirWatcher::isMatchPattern(const QString& fileName)
{
    const auto& match = filesPattern_.match(fileName);
    return match.hasMatch();
}

QList<QFileInfo> RecursiveDirWatcher::getEntries(const QString& path)
{
    return QDir(path).entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
}

RecursiveDirWatcher::FileInfo RecursiveDirWatcher::makeFileInfo(const QFileInfo& fi)
{
    FileInfo res;
    res.lastModified = fi.lastModified();
    res.path = QDir(path_).filePath(fi.fileName());
    if (fi.isDir())
    {
        res.dirWatcher = std::make_shared<RecursiveDirWatcher>(res.path, filesPattern_);
        connect(res.dirWatcher.get(), &RecursiveDirWatcher::fileChanges, this,
                &RecursiveDirWatcher::fileChanges);
    }
    else
    {
        res.fileWatcher = std::make_shared<QFileSystemWatcher>(QStringList() << res.path);
        connect(res.fileWatcher.get(), &QFileSystemWatcher::fileChanged, this,
                &RecursiveDirWatcher::dirChanged);
    }
    return res;
}
