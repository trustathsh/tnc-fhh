#!/bin/sh
echo "/sbin/iptables -D FORWARD -s $1 -j ACCEPT"
/sbin/iptables -D FORWARD -s $1 -j ACCEPT