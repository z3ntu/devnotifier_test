#include <QCoreApplication>

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
    void deviceAdded();
    void deviceRemoved();
private:
    IDeviceNotifier *notifier = nullptr;
};

MainClass::MainClass()
{
#ifdef Q_OS_LINUX
    IDeviceNotifier *notifier = new DeviceNotifierLinux();
#elif defined(Q_OS_DARWIN)
    IDeviceNotifier *notifier = new DeviceNotifierMac();
#else
    #error "Missing DeviceNotifier implementation for OS."
#endif
    notifier->setup();
    connect(notifier, &IDeviceNotifier::deviceAdded, this, &MainClass::deviceAdded);
    connect(notifier, &IDeviceNotifier::deviceRemoved, this, &MainClass::deviceRemoved);
}

void MainClass::deviceAdded()
{
}

void MainClass::deviceRemoved()
{
}

int main(int argc, char *argv[])
{
#ifdef Q_OS_DARWIN
    // Lets Qt use the CF loop, otherwise CFRunLoopRun() would have to be used.
    qputenv("QT_EVENT_DISPATCHER_CORE_FOUNDATION", "1", true);
#endif
    QCoreApplication app(argc, argv);

    MainClass mainClass;

    return app.exec();
}

#include "main.moc"
