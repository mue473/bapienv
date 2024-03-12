#!/bin/bash
# This file is free software, C 2018 - 2024 Rainer Müller

# allow daemon to write into /usr/local/etc directory
chgrp daemon /usr/local/etc
chmod g+w /usr/local/etc

# set ownership and restrict write access
chown -R root:www-data /www
find /www -type d -exec chmod 755 {} +
find /www -type f -exec chmod 644 {} +

# allow www-data to write config data
chmod -R ug+w /www/config/

# allow configuration changes
chmod ug+w /www/MaeCAN-Server/node/*.json

# allow adding and removing loco icons
chmod -R ug+w /www/MaeCAN-Server/html/loco_icons/
