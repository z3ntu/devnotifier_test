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

#ifndef DEVICENOTIFIERMAC_H
#define DEVICENOTIFIERMAC_H

#include "../src/idevicenotifier.h"

/**
 * @todo write docs
 */
class DeviceNotifierMac : public IDeviceNotifier
{
public:
    virtual ~DeviceNotifierMac() override;
    virtual bool setup() override;
private:
//    struct udev *udev = NULL;
//    struct udev_monitor *mon = NULL;

private slots:
//    void udevEvent(int fd);
};

#endif // DEVICENOTIFIERMAC_H

