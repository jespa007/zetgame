#!/bin/bash
echo RASPBIAN_ROOTFS=$RASPBIAN_ROOTFS
if [ "$RASPBIAN_ROOTFS" = "" ]
then
	echo "RASPBIAN_ROOTFS environment variable not set"
	exit
fi

echo RASPBIAN_TOOLCHAIN=$RASPBIAN_TOOLCHAIN
if [ "$RASPBIAN_TOOLCHAIN" = "" ]
then
	echo "RASPBIAN_TOOLCHAIN environment variable not set"
	exit
fi

#$RASPBIAN_TOOLCHAIN/bin/arm-linux-gnueabihf-gcc -g -O0 test_window_rpi.c -I -L -lbrcmEGL -lbrcmGLESv2 -lbcm_host -lvchostif -lvcos -lvchiq_arm -pthread  -lm -ldl -lbrcmEGL -lbrcmGLESv2 -lbcm_host -lvchostif -lvcos -lvchiq_arm -lrt -I${RASPBIAN_ROOTFS}/user/include -L${RASPBIAN_ROOTFS}/user/lib -I${RASPBIAN_ROOTFS}/opt/vc/include -L${RASPBIAN_ROOTFS}/opt/vc/lib -o test_window_rpi
$RASPBIAN_TOOLCHAIN/bin/arm-linux-gnueabihf-gcc -g -O0 test_window_rpi.c -lbcm_host -pthread -lbrcmGLESv2 -lbrcmEGL -lvcos -lvchiq_arm -lm -ldl -I${RASPBIAN_ROOTFS}/user/include -L${RASPBIAN_ROOTFS}/user/lib -I${RASPBIAN_ROOTFS}/opt/vc/include -L${RASPBIAN_ROOTFS}/opt/vc/lib -o test_window_rpi
