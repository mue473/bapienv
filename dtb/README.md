# Device Tree Modifications
see the Armbian documentation
https://docs.armbian.com/User-Guide_Allwinner_overlays/


- owc-uart.dts is a user overlay used to achieve OpenWrt compatible UART configuration.
It disables UART7 and enables UART2 instead.
```
sudo armbian-add-overlay owc-uart.dts
```

- sun7i-a20-can.dts is a orrection for the broken system overlay since 2021, see
https://forum.armbian.com/topic/18349-can-controller-in-bananapi-does-not-work-in-21051-image-solution-available/
```
dtc -@ -I dts -O dtb -o sun7i-a20-can.dtbo sun7i-a20-can.dts 
sudo cp sun7i-a20-can.dtbo /boot/dtb/overlay/
``` 
