#!/bin/sh

# Set default policy to drop
/sbin/iptables --policy FORWARD DROP

# Only PDP traffic is allowed by default
/sbin/iptables -A FORWARD -d 192.168.1.19 -j ACCEPT

# The route back
/sbin/iptables -A FORWARD -d 10.8.0.0/24 -j ACCEPT

# Enable stateful firewalling
/sbin/iptables -A FORWARD -m state --state ESTABLISHED,RELATED -j ACCEPT
