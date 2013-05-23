Readme:

To start this on single PC, several IPs need to be assigned to computer.

My eth0 is not connected to anything, so I'm using it for testing.

First we need to add IP addresses:
# ifconfig eth0 10.10.10.1 netmask 255.255.255.0
# ifconfig eth0:1 10.10.10.2 netmask 255.255.255.0
# ifconfig eth0:X 10.10.10.X+1 netmask 255.255.255.0
Repeat changing X untill required number of IPs are configured.

Now we need to start sensors:
# ./sensor v4 10.10.10.1
# ./sensor v4 10.10.10.2
# ./sensor v4 10.10.10.X
Repeat changing X untill required number of sensors are running.

Start the action:
./user
Option [2] Connect to one of sensors.
Option [3] Make sensor master node.