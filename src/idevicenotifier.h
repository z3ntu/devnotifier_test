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

#ifndef IDEVICENOTIFIER_H
#define IDEVICENOTIFIER_H

#include <QObject>

/**
 * @todo write docs
 */
class IDeviceNotifier : public QObject
{
    Q_OBJECT
public:
    virtual ~IDeviceNotifier() {}
    virtual bool setup() = 0;
signals:
    void deviceAdded(/*QString vid, QString pid*/);
    void deviceRemoved();
};

#endif // IDEVICENOTIFIER_H
