project('devnotifier_test', 'c')

if build_machine.system() == 'linux'
  libudev = dependency('libudev')
  deps = [libudev]
  src = ['linux/udev_monitor_usb.c']
else
  iokit = dependency('appleframeworks',
                     modules : ['CoreFoundation', 'IOKit'])
  deps = [iokit]
  src = ['mac/USBPrivateDataSample.c']
endif

executable('devnotifier_test', src, dependencies : deps)
