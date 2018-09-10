/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2018  Luca Weiss <luca@z3ntu.xyz>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// Thanks to Benjamin Tissoires for implementing something similar in mtdiag-qt!

#include "devicenotifierlinux.h"

#include <QDebug>
#include <QSocketNotifier>

DeviceNotifierLinux::~DeviceNotifierLinux()
{
    udev_unref(udev);
    udev_monitor_unref(mon);
}

bool DeviceNotifierLinux::setup()
{
    udev = udev_new();
    if (!udev) {
        qCritical("Could not call udev_new()");
        return false;
    }

    mon = udev_monitor_new_from_netlink(udev, "udev");
    // TODO: Maybe monitor "input" instead of "hidraw"? bentiss uses input
    udev_monitor_filter_add_match_subsystem_devtype(mon, "hidraw", NULL);
    udev_monitor_enable_receiving(mon);

    QSocketNotifier *sn = new QSocketNotifier(udev_monitor_get_fd(mon), QSocketNotifier::Read, this);
    connect(sn, &QSocketNotifier::activated, this, &DeviceNotifierLinux::udevEvent);
    return true;
}

void DeviceNotifierLinux::udevEvent(int fd)
{
    if (fd != udev_monitor_get_fd(mon))
        return;

    struct udev_device *dev = udev_monitor_receive_device(mon);

    // TODO Get parent (USB) device
    struct udev_device *parent = udev_device_get_parent_with_subsystem_devtype(dev, "usb", "usb_device");
    if (parent == NULL) {
        qDebug() << "Unable to get parent device.";
        udev_device_unref(dev);
        return;
    }

    QString vid = QString(udev_device_get_sysattr_value(parent, "idVendor"));
    if (vid != "1532") {
        qDebug() << "Ignoring device with vid" << vid;
        udev_device_unref(dev);
        return;
    }

    QString pid = QString(udev_device_get_sysattr_value(parent, "idProduct"));

    QString sysname = QString(udev_device_get_sysname(dev)); // aka DEVNAME

    QString action = QString(udev_device_get_action(dev));
    qDebug() << "Action:" << action << "- VID:" << vid << "- PID:" << pid << "- SYSNAME:" << sysname;;
    if (action == "add")
        emit deviceAdded();
    else if (action == "remove")
        emit deviceRemoved();
    else
        qDebug() << "Action" << action << "ignored.";

    udev_device_unref(dev);
}
