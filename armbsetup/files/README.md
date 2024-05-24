# armbsetup files

### HTTP Server configuration
- /etc/lighttpd/conf-enabled/10-cgi.conf
- /etc/lighttpd/lighttpd.conf

### configuration of other services
- /etc/rc.local
- /etc/init.d/canhandling
- /etc/modprobe.d/i2card.conf
- /etc/modprobe.d/temp-blacklist.conf

### automatic installation of additional drivers when upgrading kernel
- /etc/kernel/postinst.d/driverinst
- /etc/kernel/prerm.d/driverrem
- /etc/apt/apt.conf.d/20-driverupdate
