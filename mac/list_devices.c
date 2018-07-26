#include <IOKit/IOKitLib.h>
#include <IOKit/usb/IOUSBLib.h>

int main(int argc, const char *argv[])
{
    CFMutableDictionaryRef matchingDict;
    io_iterator_t iter;
    kern_return_t kr;
    io_service_t device;

    /* set up a matching dictionary for the class */
    matchingDict = IOServiceMatching(kIOUSBDeviceClassName);
    if (matchingDict == NULL)
    {
        return -1; // fail
    }

    /* Now we have a dictionary, get an iterator.*/
    kr = IOServiceGetMatchingServices(kIOMasterPortDefault, matchingDict, &iter);
    if (kr != KERN_SUCCESS)
    {
        return -1;
    }

    /* iterate */
    while ((device = IOIteratorNext(iter)))
    {
        /* do something with device, eg. check properties */
        /* ... */
        /* And free the reference taken before continuing to the next item */
        IOObjectRelease(device);
    }

   /* Done, release the iterator */
   IOObjectRelease(iter);
   return 0;
}

