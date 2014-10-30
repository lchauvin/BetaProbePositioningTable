#ifndef _ARCUSPERFORMAXDRIVER_H_
#define _ARCUSPERFORMAXDRIVER_H_

//libusb standard header file
#include "libusb-1.0/libusb.h"

//these are defined by Arcus
#define PERFORMAX_RETURN_SERIAL_NUMBER	0x0
#define PERFORMAX_RETURN_DESCRIPTION	0x1
#define PERFORMAX_MAX_DEVICE_STRLEN		256

//these defines are to conveniently turn these types transparent
//note: C does not have a bool type.
typedef int				AR_BOOL;
typedef long			AR_DWORD;
typedef void			AR_VOID;
//typedef usb_dev_handle*	AR_HANDLE;
typedef libusb_device_handle*	AR_HANDLE;

//the function definitions are the same as the windows API
bool fnPerformaxComGetNumDevices(long *numDevices); 
bool fnPerformaxComGetProductString(long dwNumDevice, void* lpDeviceString, long dwOptions);
bool fnPerformaxComOpen(long dwDeviceNum, libusb_device_handle** pHandle);
bool fnPerformaxComClose(libusb_device_handle* pHandle);
bool fnPerformaxComSetTimeouts(long dwReadTimeout, long dwWriteTimeout);
bool fnPerformaxComSendRecv(libusb_device_handle* Handle, void *wBuffer, long dwNumBytesToWrite, long dwNumBytesToRead, void *rBuffer);
bool fnPerformaxComFlush(libusb_device_handle* Handle);

//the following does _not_ need to be called before using the other functions. It is safe to ignore its existence
bool InitializePerformaxLibrary(void);

#endif

