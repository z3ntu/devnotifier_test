project('devnotifier_test', 'cpp', default_options : ['cpp_std=c++11'])

qt5_dep = dependency('qt5', modules : ['Core'])
deps = [qt5_dep]

src = ['src/main.cpp']

if build_machine.system() == 'linux'
  libudev = dependency('libudev')
  deps += [libudev]
  #src = ['linux/udev_monitor_usb.c']
  src += ['linux/devicenotifierlinux.cpp']
else
  iokit = dependency('appleframeworks',
                     modules : ['CoreFoundation', 'IOKit'])
  deps += [iokit]
  src += ['mac/devicenotifiermac.cpp']
endif

moc_headers = ['src/idevicenotifier.h']

qt5 = import('qt5')
src += qt5.preprocess(moc_headers : moc_headers)

executable('devnotifier_test', src, dependencies : deps)
