#include <QCoreApplication>

#include "idevicenotifier.h"

#ifdef Q_OS_LINUX
#include "../linux/devicenotifierlinux.h"
#elif defined(Q_OS_DARWIN)
#include "../mac/devicenotifiermac.h"
#endif

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

#ifdef Q_OS_LINUX
    IDeviceNotifier *notifier = new DeviceNotifierLinux();
#elif defined(Q_OS_DARWIN)
    IDeviceNotifier *notifier = new DeviceNotifierMac();
#else
    #error "Missing DeviceNotifier implementation for OS."
#endif
    notifier->setup();

    return app.exec();
}
