#include "cancollector.h"

CanCollector::CanCollector(QObject *parent) : QObject(parent)
{
    anIf(CanCollectorDbgEn, anTrk("Construct Object"));
    localDb = QSqlDatabase::addDatabase("QMYSQL");
    localDb.setHostName("localhost");
    localDb.setDatabaseName("raspberry");
    localDb.setUserName("root");
    localDb.setPassword("Ascenx123");
    localDb.setPort(3306);
    if (localDb.open())
    {
        anIf(CanCollectorDbgEn, anAck("OK Local Database Connected !"));
        currentQuery = QSqlQuery();
    }
    else
    {
        anIf(CanCollectorDbgEn, anError("Failed To Connect Local Database !"));
    }
    timer.setParent(this);
    timer.setInterval(1000);
    QObject::connect(&timer, &QTimer::timeout, this, [&](){
        anIf(CanCollectorDbgEn,
             anAck("Wait Timed Out! Send Present Request !");
             anInfo(CanProtocol::PresentRequest.toString());
        );
        emit CommandOut(CanProtocol::PresentRequest);
    });
}

void CanCollector::start()
{
    anIf(CanCollectorDbgEn, anTrk("Start Working"));
    timer.start();
    isRunning = true;
}

void CanCollector::stop()
{
    anIf(CanCollectorDbgEn, anTrk("Stop Working"));
    timer.stop();
    isRunning = false;
}

void CanCollector::dataFromSDCS(const QCanBusFrame &CanMsg)
{
    timer.stop();
    if (isRunning)
    {
        currentCP = CanProtocol(CanMsg);
        QString currentRFID = QString(currentCP.GetRFID());
        currentQuery.exec("SELECT GlobalID,RFID FROM stations WHERE sdcsAddr=" + QString::number(currentCP.GetSdcsId()) +
                            " AND sdcsCH=" + QString::number(currentCP.GetChId()));
        if (currentQuery.next())
        {
            if (currentQuery.value("RFID").toString()!=("R" + currentRFID))
            {
                emit updateRFIDData(currentQuery.value("GlobalID").toInt(),currentRFID);
            }
        }
        else
        {
            anIf(CanCollectorDbgEn, anWarn("Not Found In Database !"));
        }
        timer.start();
    }
}
