#ifndef CANCOLLECTOR_H
#define CANCOLLECTOR_H

#define CanCollectorDbgEn 1

#include <QObject>
#include "anlogger.h"
#include "canprotocol.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTimer>

class CanCollector : public QObject
{
    Q_OBJECT
public:
    explicit CanCollector(QObject *parent = nullptr);

signals:
    void CommandOut(const CanProtocol &);
    void updateRFIDData(quint32, const QString &);
public slots:
    void start();
    void stop();
    void dataFromSDCS(const QCanBusFrame &CanMsg);
private:
    QTimer timer;
    QSqlDatabase localDb;
    QSqlQuery currentQuery;
    bool isRunning = false;
    CanProtocol currentCP;
};

#endif // CANCOLLECTOR_H
