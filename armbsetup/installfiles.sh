#!/bin/bash
# This file is free software, C 2018 - 2024 Rainer Müller

# General
cp -p files/rc.local /etc/
chmod +x /etc/rc.local

cp -p files/temp-blacklist.conf /etc/modprobe.d/
cp -p files/i2card.conf /etc/modprobe.d/
cp -p files/canhandling /etc/init.d/

# HTTP Server
cp -p files/lighttpd.conf /etc/lighttpd/
cp -p files/10-cgi.conf /etc/lighttpd/conf-enabled/
cp -rp ../www/ /
 
# prepare service handling
cp -p files/canhandling /etc/init.d/
chmod +x /etc/init.d/canhandling

systemctl daemon-reload
systemctl enable canhandling

# allow daemon to write into /usr/local/etc directory
chgrp daemon /usr/local/etc
chmod g+w /usr/local/etc

# enable user to shut down
chmod 4711 /bin/systemctl

# update initramfs
update-initramfs -u
