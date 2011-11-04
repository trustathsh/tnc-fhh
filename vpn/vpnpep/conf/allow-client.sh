#!/bin/sh
echo "/sbin/iptables -I FORWARD 1 -s $1 -j ACCEPT"
/sbin/iptables -I FORWARD 1 -s $1 -j ACCEPT