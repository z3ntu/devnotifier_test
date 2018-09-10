project('devnotifier_test', 'cpp')

qt5_dep = dependency('qt5', modules : ['Core'])
deps = [qt5_dep]

src = ['src/main.cpp']

if build_machine.system() == 'linux'
  libudev = dependency('libudev')
  deps += [libudev]
  #src = ['linux/udev_monitor_usb.c']
  src += ['linux/devicenotifierlinux.cpp']
  moc_headers = ['src/idevicenotifier.h']
else
  iokit = dependency('appleframeworks',
                     modules : ['CoreFoundation', 'IOKit'])
  deps += [iokit]
  src += ['mac/USBPrivateDataSample.c']
endif

qt5 = import('qt5')
src += qt5.preprocess(moc_headers : moc_headers)

executable('devnotifier_test', src, dependencies : deps)
