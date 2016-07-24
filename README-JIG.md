Jig Setup
=========

The test jig runs on a Raspberry Pi using the standard Raspbian base.

Base Setup
----------

1. Grab the latest version of Raspbian from https://www.raspberrypi.org/downloads/
2. Write it to an SD card using dd
3. Open up fdisk and create a third partition.  Have it fill up the card.  Place it after partition 2.
4. Reformat the filesystem as f2fs.  mkfs.f2fs /dev/sdx3
5. Load the SD card into the target device
6. Connect the target device to Ethernet, HDMI, and a USB keyboard
7. Connect USB power
8. Open up a terminal window
9. sudo passwd pi # change password to "kosagi"
10. Edit /etc/apt/sources.list and uncomment "deb-src"
11. sudo apt-get update
12. Install a replacement syslog that doesn't write to disk: sudo apt-get install busybox-syslogd
13. Remove stuff we don't need: sudo apt-get remove --purge --auto-remove mathematica-fonts wolfram-engine scratch sonic-pi nodered oracle-java8-jdk libreoffice-writer libreoffice-calc libreoffice-base libreoffice-impress libreoffice-draw libreoffice-math libreoffice oracle-java8-jdk greenfoot bluej anacron logrotate dphys-swapfile xserver-common lightdm
14. sudo apt-get -y dist-upgrade
15. Comment out checks in /etc/init.d/check*.sh (or maybe remove these files entirely)
16. Set the timezone. sudo dpkg-reconfigure tzdata
17. Make / readonly by default.  Add ",ro" to /etc/fstab
18. Fix resolv.conf: sudo rm /etc/resolv.conf; sudo ln -sf /tmp/resolv.conf /etc/resolv.conf
19. Format the log partition: mkfs.f2fs /dev/mmcblk0p3
20. Mount it by default: mkdir /logs; echo '/dev/mmcblk0p3 /logs f2fs defaults,noatime 0 0' >> /etc/fstab
21. Run "sudo raspi-config" and enable the camera, give the GPU 256 MB of RAM, and have the system boot to console.
22. reboot


Tester Setup
------------

1. Make / read-write: sudo mount -oremount,rw /
2. Change the owner of /logs to "pi": sudo chown pi:pi /logs/
3. Install build dependencies for everything: sudo apt-get build-dep -y openocd qt4-x11 libqt5gui5; sudo apt-get install -y libudev-dev libinput-dev libts-dev libxcb-xinerama0-dev autoconf libtool libftdi-dev gstreamer1.0-omx libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev usbmount resolvconf; sudo apt-get clean
4. Check out, build, and install openocd: mkdir ~/Code/; cd ~/Code; sudo mkdir /opt/openocd; sudo chown pi:pi /opt/openocd; git clone git://git.code.sf.net/p/openocd/code openocd; cd openocd; ./bootstrap; ./configure --enable-bcm2835gpio --enable-sysfsgpio --prefix=/opt/openocd; make -j4; make install; sudo chown root:root /opt/openocd/bin/openocd && sudo chmod 4775 /opt/openocd/bin/openocd
5. Create directory for qt5pi: sudo mkdir /opt/qt5pi; sudo chown pi:pi /opt/qt5pi
6. Copy stuff over: rsync -essh -avz qt5pi pi@raspberrypi.local:/opt
7. Add qt libs: echo /opt/qt5pi/lib | sudo tee /etc/ld.so.conf.d/qt5pi.conf; sudo ldconfig
8. Re-link libGLES and libEGL:  sudo rm /usr/lib/arm-linux-gnueabihf/libEGL.so.1.0.0 /usr/lib/arm-linux-gnueabihf/libGLESv2.so.2.0.0; sudo ln -s /opt/vc/lib/libEGL.so /usr/lib/arm-linux-gnueabihf/libEGL.so.1.0.0; sudo ln -s /opt/vc/lib/libGLESv2.so /usr/lib/arm-linux-gnueabihf/libGLESv2.so.2.0.0
9. Install the systemd script: sudo cp deploy/factory-test.service /etc/systemd/system; sudo systemctl enable factory-test

Development Setup
-----------------

Information adapted from https://wiki.qt.io/RaspberryPi2EGLFS
