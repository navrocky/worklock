#pragma once

#include <QObject>

/**
 * @brief The abstract SourceFilesWatcher class
 */
class SourceFilesWatcher : public QObject
{
    Q_OBJECT
public:
    SourceFilesWatcher(QObject* parent = 0);

signals:
    void filesChanged(const QStringList& urls);
};
