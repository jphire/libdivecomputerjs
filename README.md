# libdivecomputerjs

This project is a wrapper for [libdivecomputer](https://www.libdivecomputer.org/) and enables you to use libdivecomputer with nodeJS.

This project tries to stay as close to the original project regarding the calling patterns and naming with some exceptions.

As an example see `test/download-eonsteel.ts` as a reference.

## Dependencies

-   `cmake`
-    autoconf
-    build-essentials
-    pkg-config
-   `libusb`
-   `libhidapi`
-   `BlueZ`

For ubuntu run `sudo apt install libusb-1.0-0 libhidapi-libusb0 libbluetooth3 build-essentials autoconf pkg-config cmake`

## Installation

Run `yarn install`

run `yarn test` to check if it works

## Deviations from the original project

-   I used class for all major parts of the project instead of functions operating on objects. For instance `Parser` has all functionality of `dc_parser_t` and `USBHIDTransport` for `dc_usbhid_device_t`
-   All device types in libdivecomputer are renamed to transports. Eg. `dc_usbhid_device_t` is named `USBHIDTransport` to prevent confusion with the `Device` class, wrapping `dc_device_t`.
-   All iterate functions are static methods off the class it iterates. Eg `Descriptor.iterate` for `dc_descriptor_iterator`
-   `Parser.getDatetime` returns a string instead of a datetime. As of writing `Napi::Date` doesn't have a way to set the timezone. This I needed to return a custom object, or format. I settled on returning an ISO date time, I think it is the easiest format to parse in JavaScript or other languages.

## Common problems

### Linux: The library compiles and tests work, but for my USB based device the `new Device` fails.

For linux you need to define how and who can access newly plugged in devices. You can put [this](https://github.com/libdivecomputer/libdivecomputer/blob/master/contrib/udev/libdivecomputer.rules) file in /etc/udev/rules.d/ . Ensure your user is part of the `plugdev` group (and ensure it exists). After a re-login to ensure your user groups are reloaded, and after you re plugin the device. it should work fine.

### Linux: It keeps throwing UNSUPPORTED errors for USB or bluetooth.

Chances are libdivecomputer compiled without the required libraries. Ensure you can run the following commando's

- `pkg-config --exists --print-errors libusb-1.0 && echo 'LIBUSB found'`
- `pkg-config --exists --print-errors bluez && echo 'BlueZ found'`

if both echo that they found the lib, great, if not, ensure you have `libusb-1.0-0` and `libbluetooth3` installed, allong with all the build tools like `build-essentials autoconf pkg-config cmake`. if it found them. conpile again `yarn clean` and `yarn build`