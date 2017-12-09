#!/bin/sh

cd /sys/bus/platform/drivers/omap-rproc/

echo 55020000.ipu > unbind 
echo 55020000.ipu > bind

