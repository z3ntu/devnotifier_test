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

#include "devicenotifiermac.h"

// TODO Which headers are actually needed?
#include <CoreFoundation/CoreFoundation.h>

#include <IOKit/IOKitLib.h>
#include <IOKit/IOMessage.h>
#include <IOKit/IOCFPlugIn.h>
#include <IOKit/usb/IOUSBLib.h>

#include <QDebug>
#include <QSocketNotifier>

DeviceNotifier::~DeviceNotifier()
{

}

// TODO Put in header file?
io_iterator_t gAddedIter;
io_iterator_t gRemovedIter;

void deviceConnectedCallback(void *refCon, io_iterator_t iterator)
{
    qDebug() << "deviceConnectedCallback() called";
    kern_return_t kr;
    io_service_t usbDevice;
    (void) refCon;

    while((usbDevice = IOIteratorNext(iterator))) {
        io_name_t deviceName;
        QString name;

        kr = IORegistryEntryGetName(usbDevice, deviceName);
        if(kr == KERN_SUCCESS)
            name = QString::fromLocal8Bit(deviceName);
        qDebug() << "device" << name << "in deviceConnectedCallback";

        IOCFPlugInInterface **plugInInterface;
        SInt32 score;
        kr = IOCreatePlugInInterfaceForService(usbDevice, kIOUSBDeviceUserClientTypeID, kIOCFPlugInInterfaceID, &plugInInterface, &score);
        if((kr != kIOReturnSuccess) || !plugInInterface) {
            qDebug() << "Error calling IOCreatePlugInInterfaceForService. kr:" << kr;
            return;
        }

        IOUSBDeviceInterface **deviceInterface;
        HRESULT res;
        res = (*plugInInterface)->QueryInterface(plugInInterface, CFUUIDGetUUIDBytes(kIOUSBDeviceInterfaceID), (LPVOID*) &deviceInterface);
        (*plugInInterface)->Release(plugInInterface);

        if(res || deviceInterface == NULL) {
            qDebug() << "Error calling QueryInterface. res:" << res;
            return;
        }

        UInt16 vid, pid;
        kr = (*deviceInterface)->GetDeviceVendor(deviceInterface, &vid);
        assert(kr == kIOReturnSuccess);
        kr = (*deviceInterface)->GetDeviceProduct(deviceInterface, &pid);
        assert(kr == kIOReturnSuccess);
        qDebug() << "vid:" << QString::number(vid, 16).rightJustified(4, '0')  << "pid:" << QString::number(pid, 16).rightJustified(4, '0');
        USBDeviceAddress addr;
        kr = (*deviceInterface)->GetDeviceAddress(deviceInterface, &addr);
        qDebug() << "device address:" << addr;


        kr = IOObjectRelease(usbDevice);
    }
}

void deviceDisconnectedCallback(void *refCon, io_iterator_t iterator)
{
    qDebug() << "deviceDisconnectedCallback() called";
    kern_return_t kr;
    io_service_t usbDevice;
    (void) refCon;

    while((usbDevice = IOIteratorNext(iterator))) {
        // TODO
        io_name_t deviceName;

        kr = IORegistryEntryGetName(usbDevice, deviceName);
        QString name;
        if(kr == KERN_SUCCESS)
            name = QString::fromLocal8Bit(deviceName);
        qDebug() << "device" << name << "in deviceDisconnectedCallback";

        kr = IOObjectRelease(usbDevice);
    }
}

bool DeviceNotifier::setup()
{
    qDebug() << "setup() called";
    CFMutableDictionaryRef matchingDict = IOServiceMatching(kIOUSBDeviceClassName);

/*  Filtering here doesn't work.
    UInt32 vendorId = 0x1532;
    CFNumberRef cfVendorValue = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &vendorId);
    CFDictionaryAddValue(matchingDict, CFSTR(kUSBVendorID), cfVendorValue);
    CFRelease(cfVendorValue);
*/

    IONotificationPortRef notificationPort = IONotificationPortCreate(kIOMasterPortDefault);
    CFRunLoopSourceRef runLoopSource = IONotificationPortGetRunLoopSource(notificationPort);

    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopDefaultMode);

    // IOServiceAddMatchingNotification releases this, so we retain for the next call
    CFRetain(matchingDict);

    kern_return_t kr;
    kr = IOServiceAddMatchingNotification(notificationPort,
                                          kIOMatchedNotification,
                                          matchingDict,
                                          deviceConnectedCallback,
                                          NULL,
                                          &gAddedIter);

    kr = IOServiceAddMatchingNotification(notificationPort,
                                          kIOTerminatedNotification,
                                          matchingDict,
                                          deviceDisconnectedCallback,
                                          NULL,
                                          &gRemovedIter);

    // Iterate once to get already-present devices and arm the notification
    deviceConnectedCallback(NULL, gAddedIter);
    deviceDisconnectedCallback(NULL, gRemovedIter);

    return true;
}

