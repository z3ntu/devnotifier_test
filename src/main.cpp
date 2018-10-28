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
    void deviceAdded(QString vid, QString pid, QString path);
    void deviceRemoved(QString vid, QString pid, QString path);
private:
    IDeviceNotifier *notifier = nullptr;
};

MainClass::MainClass()
{
#ifdef Q_OS_LINUX
    notifier = new DeviceNotifierLinux();
#elif defined(Q_OS_DARWIN)
    notifier = new DeviceNotifierMac();
#else
    #error "Missing DeviceNotifier implementation for OS."
#endif
    notifier->setup();
    connect(notifier, &IDeviceNotifier::deviceAdded, this, &MainClass::deviceAdded);
    connect(notifier, &IDeviceNotifier::deviceRemoved, this, &MainClass::deviceRemoved);
}

void MainClass::deviceAdded(QString vid, QString pid, QString path)
{
    qDebug() << "Action:" << "add" << "- VID:" << vid << "- PID:" << pid << "- PATH:" << path;
}

void MainClass::deviceRemoved(QString vid, QString pid, QString path)
{
    qDebug() << "Action:" << "remove" << "- VID:" << vid << "- PID:" << pid << "- PATH:" << path;
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
