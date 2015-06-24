# tide_data
A program that fetches and parses predective tide data from www.tidesandcurrents.noaa.gov.
Also contains a class for sending data via UART on the raspberry pi.
built for arduino1.6 and linux. 

To build for linux just clone the rbpi branch and run 

autoreconf
then run 
./configure
and finally 
make

If you have multiarch enabled you can cross compile.However, you must install the libcurl4-gnutls-dev for the arch you want to compile. For example, if you want to compile for armel you need to enable armel in multiarch remove your current libcurl4-gnutls-dev and install libcurl4-gnutls-dev:armel. You can then run 
autoreconf
./configure --host=arm-linux-gnueabi -prefix=/usr/bin/arm-linux-gnueabi 
make.
