#include <QCoreApplication>
#include <QDebug>

#include "idevicenotifier.h"

#ifdef Q_OS_LINUX
#include "../linux/devicenotifierlinux.h"
#elif defined(Q_OS_DARWIN)
#include "../mac/devicenotifiermac.h"
#endif

class MainClass : public QObject
{
    Q_OBJECT
public:
    MainClass();
public slots:
    void triggerRediscover();
private:
    IDeviceNotifier *notifier = nullptr;
};

MainClass::MainClass()
{
    notifier = new DeviceNotifier();
    notifier->setup();
    connect(notifier, &IDeviceNotifier::triggerRediscover, this, &MainClass::triggerRediscover);
}

void MainClass::triggerRediscover()
{
    qDebug() << "TRIGGER REDISCOVER";
}

int main(int argc, char *argv[])
{
#ifdef Q_OS_DARWIN
    // Lets Qt use the CF loop, otherwise CFRunLoopRun() would have to be used.
    qputenv("QT_EVENT_DISPATCHER_CORE_FOUNDATION", "1");
#endif
    QCoreApplication app(argc, argv);

    MainClass mainClass;

    return app.exec();
}

#include "main.moc"
