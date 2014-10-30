#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <unistd.h>
#include "ArcusPerformaxDriver.h"

//--------------------------------------------------
bool goToZero(libusb_device_handle* handle);
bool goToCenter(libusb_device_handle* handle);
bool moveXTo(libusb_device_handle* handle, int value);
bool moveXTo_mm(libusb_device_handle* handle, int value);
bool moveYTo(libusb_device_handle* handle, int value);
bool moveYTo_mm(libusb_device_handle* handle, int value);
bool moveXYTo(libusb_device_handle* handle, int xValue, int yValue);
bool moveXYTo_mm(libusb_device_handle* handle, int xValue, int yValue);
double getX(libusb_device_handle* handle);
double getX_mm(libusb_device_handle* handle);
double getY(libusb_device_handle* handle);
double getY_mm(libusb_device_handle* handle);

//--------------------------------------------------
int xMax = 80235;
int yMax = 80235;

int steps_mm = 800;

int tableWidth_mm = 100;
int tableHeight_mm = 100;

int tableWidth_steps = tableWidth_mm*steps_mm;
int tableHeight_steps = tableWidth_mm*steps_mm;

//--------------------------------------------------
int main(void)
{
  char 		lpDeviceString[PERFORMAX_MAX_DEVICE_STRLEN];
  libusb_device_handle*	Handle; //usb handle
  char		out[64];
  char		in[64];
  long    	num;
    
  int xReturnedValue = 0;
  int yReturnedValue = 0;
  
  memset(out,0,64);
  memset(in,0,64);

  //acquire information
	
  if(!fnPerformaxComGetNumDevices(&num))
    {
      std::cout << "error in fnPerformaxComGetNumDevices" << std::endl;
      return 1;
    }
  if(num<1)
    {
      std::cout <<  "No motor found" << std::endl;
      return 1;
    }
	
  if( !fnPerformaxComGetProductString(0, lpDeviceString, PERFORMAX_RETURN_SERIAL_NUMBER) ||
      !fnPerformaxComGetProductString(0, lpDeviceString, PERFORMAX_RETURN_DESCRIPTION) )
    {
      std::cout << "error acquiring product string" << std::endl;
      return 1;
    }
		
  //setup the connection
	
  if(!fnPerformaxComOpen(0,&Handle))
    {
      std::cout <<  "Error opening device" << std::endl;
      return 1;
    }
	
  if(!fnPerformaxComSetTimeouts(5000,5000))
    {
      std::cout << "Error setting timeouts" << std::endl;
      return 1;
    }
  if(!fnPerformaxComFlush(Handle))
    {
      std::cout << "Error flushing the coms" << std::endl;
      return 1;
    }
	
  // setup the device
	
  strcpy(out, "ABS"); //set control to absolute values
  if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
    {
      std::cout << "Could not send" << std::endl;
      return 1;
    }
	
  strcpy(out, "ID"); //read current
  if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
    {
      std::cout << "Could not send" << std::endl;
      return 1;
    }	
  std::cout << "Arcus Product: " << in << std::endl;
	
  strcpy(out, "VER"); //get 
  if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
    {
      std::cout << "Could not send" << std::endl;
      return 1;
    }
  std::cout << "Version: " << in << std::endl;
	
  strcpy(out, "DN"); //read current
  if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
    {
      std::cout << "Could not send" << std::endl;
      return 1;
    }	
  std::cout << "Device Number: " << in << std::endl;
	
  //--------------------------------------------------
  std::cout << "Enable motors...";
  strcpy(out, "EOX=1"); //enable device
  if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
    {
      std::cout << "Could not send" << std::endl;
      return 1;
    }
	
  strcpy(out, "EOY=1"); //enable device
  if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
    {
      std::cout << "Could not send" << std::endl;
      return 1;
    }
  std::cout << "OK" << std::endl;
	
  //--------------------------------------------------
  std::cout << "Set High speed to 1600...";
  strcpy(out, "HSPDX=1600"); //set high speed
  if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
    {
      std::cout << "Could not send" << std::endl;
      return 1;
    }
	
  strcpy(out, "HSPDY=1600"); //set high speed
  if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
    {
      std::cout << "Could not send" << std::endl;
      return 1;
    }
  std::cout << "OK" << std::endl;
	
  //--------------------------------------------------
  std::cout << "Set Low speed to 13...";
  strcpy(out, "LSPDX=13"); //set low speed
  if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
    {
      std::cout << "Could not send" << std::endl;
      return 1;
    }
	
  strcpy(out, "LSPDY=13"); //set low speed
  if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
    {
      std::cout << "Could not send" << std::endl;
      return 1;
    }
  std::cout << "OK" << std::endl;
	
  //--------------------------------------------------
  std::cout << "Set Accuracy to 300...";
  strcpy(out, "ACCX=300"); //set acceleration
  if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
    {
      std::cout << "Could not send" << std::endl;
      return 1;
    }
	
  strcpy(out, "ACCY=300"); //set acceleration
  if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
    {
      std::cout << "Could not send" << std::endl;
      return 1;
    }
  std::cout << "OK" << std::endl;
	
  //--------------------------------------------------
  std::cout << "Press [Enter] to start homing";
  std::cin.ignore();
  std::cout << "Homing...";

  goToZero(Handle);

  // Move to center
  if (!goToCenter(Handle))
    {
      std::cout << "Failed" << std::endl;
    }
	
  std::cout << "Done" << std::endl;
	

  //--------------------------------------------------
  std::cout << "Press [Enter] to start scanning";
  std::cin.ignore();
  std::cout << "Scanning...";

  // TODO: Define scanning pattern here
  moveXYTo_mm(Handle, 20, 80);
	
  std::cout << "Done" << std::endl;

  if(!fnPerformaxComClose(Handle))
    {
      std::cout <<  "Error Closing" << std::endl;
      return 1;
    }
	
  std::cout << "Motor connection has been closed" << std::endl;
}

//--------------------------------------------------
bool goToZero(libusb_device_handle* handle)
{
  int xReturned = 0;
  int yReturned = 0;
  char tmp[64];

  std::stringstream cmd;
  cmd << "J-";

  if(!fnPerformaxComSendRecv(handle, (char*)(cmd.str().c_str()), 64,64, tmp))
    {
      std::cout << "Failed" << std::endl;
      return false;
    }
  
  do
    {
      memset(tmp,0,64);
      cmd.str("");
      cmd << "MSTX";
      fnPerformaxComSendRecv(handle, (char*)(cmd.str().c_str()), 64,64, tmp);
      xReturned = atoi(tmp);
      
      memset(tmp,0,64);
      cmd.str("");
      cmd << "MSTY";
      fnPerformaxComSendRecv(handle, (char*)(cmd.str().c_str()), 64,64, tmp);
      yReturned = atoi(tmp);
      
      sleep(1);
    } while(((xReturned&32)==0) || ((yReturned&32)==0)); //wait for x and y-axis to reach limit

  return true;
}

//--------------------------------------------------
bool goToCenter(libusb_device_handle* handle)
{  
  return moveXYTo(handle, std::floor(xMax/2), std::floor(yMax/2));
}

//--------------------------------------------------
bool moveXTo(libusb_device_handle* handle, int value)
{
  int returnedValue = 0;
  char tmp[64];

  std::stringstream cmd;
  cmd << "X" << value;
  if(!fnPerformaxComSendRecv(handle, (char*)(cmd.str().c_str()), 64,64, tmp))
    {
      std::cout << "Failed" << std::endl;
      return false;
    }
  
  cmd.str("");
  cmd << "MSTX";

  do
    {
      memset(tmp,0,64);
      fnPerformaxComSendRecv(handle, (char*)(cmd.str().c_str()), 64,64, tmp);
      returnedValue = atoi(tmp);
           
      sleep(1);
    } while((returnedValue&7)!=0); //wait for x-axis to reach value
  
  return true;
}

//--------------------------------------------------
bool moveXTo_mm(libusb_device_handle* handle, int value)
{
  return moveXTo(handle, value*steps_mm);
}

//--------------------------------------------------
bool moveYTo(libusb_device_handle* handle, int value)
{
  int returnedValue = 0;
  char tmp[64];

  std::stringstream cmd;
  cmd << "Y" << value;
  if(!fnPerformaxComSendRecv(handle, (char*)(cmd.str().c_str()), 64,64, tmp))
    {
      std::cout << "Failed" << std::endl;
      return false;
    }
  
  cmd.str("");
  cmd << "MSTY";
  
  do
    {
      memset(tmp,0,64);
      fnPerformaxComSendRecv(handle, (char*)(cmd.str().c_str()), 64,64, tmp);
      returnedValue = atoi(tmp);
           
      sleep(1);
    } while((returnedValue&7)!=0); //wait for y-axis to reach value
  
  return true;
}

//--------------------------------------------------
bool moveYTo_mm(libusb_device_handle* handle, int value)
{
  return moveYTo(handle, value*steps_mm);
}

//--------------------------------------------------
bool moveXYTo(libusb_device_handle* handle, int xValue, int yValue)
{
  int xReturned = 0;
  int yReturned = 0;
  char tmp[64];

  std::stringstream cmd;
  cmd << "X" << xValue;
  if(!fnPerformaxComSendRecv(handle, (char*)(cmd.str().c_str()), 64,64, tmp))
    {
      std::cout << "Failed" << std::endl;
      return false;
    }
  
  cmd.str("");
  cmd << "Y" << yValue;
  if(!fnPerformaxComSendRecv(handle, (char*)(cmd.str().c_str()), 64,64, tmp))
    {
      std::cout << "Failed" << std::endl;
      return false;
    }

  do
    {
      memset(tmp,0,64);
      cmd.str("");
      cmd << "MSTX";
      fnPerformaxComSendRecv(handle, (char*)(cmd.str().c_str()), 64,64, tmp);
      xReturned = atoi(tmp);

      memset(tmp,0,64);
      cmd.str("");
      cmd << "MSTY";
      fnPerformaxComSendRecv(handle, (char*)(cmd.str().c_str()), 64,64, tmp);
      yReturned = atoi(tmp);
           
      sleep(1);
    } while(((xReturned&7)!=0) || ((yReturned&7)!=0)); //wait for x and y-axis to reach center

  return true;
}

//--------------------------------------------------
bool moveXYTo_mm(libusb_device_handle* handle, int xValue, int yValue)
{
  return moveXYTo(handle, xValue*steps_mm, yValue*steps_mm);
}

//--------------------------------------------------
double getX(libusb_device_handle* handle)
{
  std::stringstream cmd;
  char tmp[64];

  cmd << "PX";
  if(!fnPerformaxComSendRecv(handle, (char*)(cmd.str().c_str()), 64,64, tmp))
    {
      return false;
    }
  
  return atof(tmp);
}

//--------------------------------------------------
double getX_mm(libusb_device_handle* handle)
{
  return getX(handle)/steps_mm;
}

//--------------------------------------------------
double getY(libusb_device_handle* handle)
{
  std::stringstream cmd;
  char tmp[64];

  cmd << "PY";
  if(!fnPerformaxComSendRecv(handle, (char*)(cmd.str().c_str()), 64,64, tmp))
    {
      return false;
    }
  
  return atof(tmp);
}

//--------------------------------------------------
double getY_mm(libusb_device_handle* handle)
{
  return getY(handle)/steps_mm;
}


