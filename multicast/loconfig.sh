#########################################################################
# File Name: loconfig.sh
# Author: Julian
# Email:  1042029354@qq.com
# Created Time: Wed 29 Apr 2015 07:35:40 PM CST
#########################################################################
#!/bin/bash

sudo route add -net 224.0.0.0 netmask 240.0.0.0 dev lo
