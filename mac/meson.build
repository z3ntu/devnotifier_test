project('mac_test', 'c')

iokit = dependency('appleframeworks',
    modules : ['CoreFoundation', 'IOKit'])

executable('mac_test', 'list_devices.c', dependencies : iokit)
executable('USBPrivateDataSample', 'USBPrivateDataSample.c', dependencies : iokit)
