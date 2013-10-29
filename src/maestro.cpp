#include "maestro.h"
#include "protocol.h"

const uint16_t Maestro::vendorId = 0x1ffb;
const size_t Maestro::nProductIds = 4;
const uint16_t Maestro::productIds[] = { 0x0089, 0x008a, 0x008b, 0x008c };

Maestro::Maestro() {
  libusb_init(&ctx);

  libusb_device** list;
  int count = libusb_get_device_list(ctx, &list);

  // scan usb devices looking for a Micro Maestro
  bool found = false;
  for (int i = 0; i < count; i++) {
    libusb_device* device = list[i];

    // check if this is a known device
    struct libusb_device_descriptor desc;
    libusb_get_device_descriptor(device, &desc);
    if (!isMaestroDevice(desc)) { continue; }
    productId = desc.idProduct;

    // attempt to open device
    int ret = libusb_open(device, &deviceHandle);
    throwIfError<MaestroInitializationError>(ret);

    // found a matching device that succesfully opened
    found = true;
    break;
  }

  libusb_free_device_list(list, 1);

  if (!found) {
    // there is no open device handle, so no need to close it
    libusb_exit(ctx);

    // fail fast; more clear than trying to open devices in the background
    throw NoMaestroFoundError();
  }
}

Maestro::~Maestro() {
  // libusb cleanup
  libusb_close(deviceHandle);
  libusb_exit(ctx);
}

bool Maestro::isMaestroDevice(libusb_device_descriptor& desc) {
  if (desc.idVendor != vendorId) { return false; }
  for (int i = 0; i < nProductIds; i++) {
    if (desc.idProduct == productIds[i]) {
      // vendor and product both matched
      return true;
    }
  }
  // no product id match
  return false;
}

int Maestro::controlTransfer(uint8_t type, uint8_t request, uint16_t value, uint16_t index) {
  int ret = libusb_control_transfer(
    deviceHandle, type, request,
    value, index,
    (unsigned char*) 0, 0, 5000);
  return throwIfError<MaestroControlError>(ret);
}

template <class T>
int Maestro::throwIfError(int code) {
  if (code < 0) {
    // code is an error, throw
    T* err = new T(errorDescription(code));
    throw err;
  }
  return code;
}

std::string Maestro::errorDescription(int error) {
  switch (error) {
    case -1: return "I/O error.";
    case -2: return "Invalid parameter.";
    case -3: return "Access denied.";
    case -4: return "Device does not exist.";
    case -5: return "No such entity.";
    case -6: return "Busy.";
    case -7: return "Timeout.";
    case -8: return "Overflow.";
    case -9: return "Pipe error.";
    case -10: return "System call was interrupted.";
    case -11: return "Out of memory.";
    case -12: return "Unsupported/unimplented operation.";
    case -99: return "Other error";
    default: return "Unknown error code.";
  };
}

void Maestro::setTarget(uint8_t servo, uint16_t value) {
  controlTransfer(0x40, REQUEST_SET_TARGET, value, servo);
}

void Maestro::setSpeed(uint8_t servo, uint16_t value) {
  controlTransfer(0x40, REQUEST_SET_SERVO_VARIABLE, value, servo);
}

void Maestro::setAcceleration(uint8_t servo, uint16_t value) {
  // setting high bit on servo number indicates acceleration
  controlTransfer(0x40, REQUEST_SET_SERVO_VARIABLE, value, servo | 0x80);
}
