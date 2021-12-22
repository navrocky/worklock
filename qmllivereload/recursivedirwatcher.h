#pragma once

#include <QDateTime>
#include <QMap>
#include <QObject>
#include <QRegularExpression>
#include <memory>

class QFileSystemWatcher;
class QFileInfo;

class RecursiveDirWatcher : public QObject
{
    Q_OBJECT
public:
    enum ChangeType
    {
        Create,
        Modify,
        Delete
    };

    struct FileChange
    {
        ChangeType type;
        QString path;
    };

    RecursiveDirWatcher(const QString& path, const QRegularExpression& filesPattern,
                        QObject* parent = 0);

signals:
    void fileChanges(QList<FileChange> changes);

private:
    struct FileInfo
    {
        QDateTime lastModified;
        QString path;
        std::shared_ptr<RecursiveDirWatcher> dirWatcher;
        std::shared_ptr<QFileSystemWatcher> fileWatcher;
    };

    void dirChanged(const QString& path);
    bool isMatchPattern(const QString& fileName);
    QList<QFileInfo> getEntries(const QString& path);
    FileInfo makeFileInfo(const QFileInfo& fi);

    QString path_;
    QRegularExpression filesPattern_;
    QFileSystemWatcher* watcher_;
    QMap<QString, FileInfo> files_;
};
