#include <QCoreApplication>

#include "idevicenotifier.h"
#include "../linux/devicenotifierlinux.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    IDeviceNotifier *notifier = new DeviceNotifierLinux();
    notifier->setup();

    return app.exec();
}
