#!/bin/sh
PATH=$PATH:.
bitfile=novena_fpga.bit
if ! [ -z $1 ]
then
	bitfile="$1"
fi

reset_gpio() {
	name="$1"
	num="$2"
	echo -n ${num} | sudo tee /sys/class/gpio/unexport 2> /dev/null > /dev/null
}

set_gpio() {
	name="$1"
	num="$2"
	dir="$3"
	val="$4"

	echo -n ${num} | sudo tee /sys/class/gpio/export 2> /dev/null > /dev/null
	if [ "x${dir}" = "xout" ]
	then
		echo "    ${name} ${dir} -> ${val} (gpio ${num})"
		echo -n ${dir} | sudo tee /sys/class/gpio/gpio${num}/direction 2> /dev/null > /dev/null
		echo -n ${val} | sudo tee /sys/class/gpio/gpio${num}/value 2> /dev/null > /dev/null
	elif [ "x${dir}" = "xin" ]
	then
		echo "    ${name} ${dir} (gpio ${num})"
		echo -n ${dir} | sudo tee /sys/class/gpio/gpio${num}/direction 2> /dev/null > /dev/null
	else
		echo "    [Error: Invalid direction ${dir} for pin ${name} (${num})]"
	fi
}

echo -n "Setting export of reset pin: "
echo 135 | sudo tee /sys/class/gpio/export 2> /dev/null
echo -n "setting reset pin to: "
echo out | sudo tee /sys/class/gpio/gpio135/direction 2> /dev/null
echo -n "flipping reset: "
echo -n 0 | sudo tee /sys/class/gpio/gpio135/value 2> /dev/null
echo -n " "
echo 1 | sudo tee /sys/class/gpio/gpio135/value 2> /dev/null

echo "configuring FPGA"

sudo dd if="${bitfile}" of=/dev/spidev2.0 bs=128

echo "turning on clock to FPGA"
sudo ./devmem2 0x020c8160 w 0x00000D2B

echo "Setting GPIOs: "

set_gpio 5v_mode 59 out 1

echo "fixing up GPIO exports"

sudo ./devmem2 0x20e01bc w 0x5  # EIM_DA8
sudo ./devmem2 0x20e01b4 w 0x5  # EIM_DA6
sudo ./devmem2 0x20e01b0 w 0x5  # EIM_DA5
sudo ./devmem2 0x20e0118 w 0x5  # EIM_A18
sudo ./devmem2 0x20e018c w 0x5  # EIM_DA10
sudo ./devmem2 0x20e01d8 w 0x5  # EIM_OE


#set_gpio reset 64 out 0
#set_gpio sck 66 out 0
#set_gpio mosi 65 out 0
#set_gpio miso 53 in
#set_gpio errled 68 out 1
#set_gpio vfyled 67 out 1

# Disable "test mode"
#set_gpio test 17 out 0
