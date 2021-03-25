# libdivecomputerjs

This project is a wrapper for [libdivecomputer](https://www.libdivecomputer.org/) and enables you to use libdivecomputer with nodeJS.

This project tries to stay as close to the original project regarding the calling patterns and naming with some exceptions.

As an example see `test/download-eonsteel.ts` as a reference.

## Installation


## Deviations from the original project

-   I used class for all major parts of the project instead of functions operating on objects. For instance `Parser` has all functionality of `dc_parser_t` and `USBHIDTransport` for `dc_usbhid_device_t`
-   All device types in libdivecomputer are renamed to transports. Eg. `dc_usbhid_device_t` is named `USBHIDTransport` to prevent confusion with the `Device` class, wrapping `dc_device_t`.
-   All iterate functions are static methods off the class it iterates. Eg `Descriptor.iterate` for `dc_descriptor_iterator`
-   `Parser.getDatetime` returns a string instead of a datetime. As of writing `Napi::Date` doesn't have a way to set the timezone. This I needed to return a custom object, or format. I settled on returning an ISO date time, I think it is the easiest format to parse in JavaScript or other languages.
