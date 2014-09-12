Packet-Sniffer
==============

Networking Senior Design Project. Done with Group of 4 During April/May/June 2014
Goal was to make a reliable form of client-side botnet detection via packet sniffing. We test this using virtual machines, but did not get very far due to limitations in VM's. Tried various algorithms, but settled on one where the number of packets sent would be our measuring stick for determining if infected via botnet.


UDPs - Simple UDP echo server.
  -Needs Port Number as command arguement

UDPc - Multithreaded UDP Client, threads ping server and output ping for returning packet from echo.
  - Needs IP, Port, Number of thread, # of pings for average as command arguements

sniffer - Packet sniffer that uses libpcap, analyzes destination IP and payloads to prevent flooding a specific IP but also somewhat prevent false positives due to legititmate traffic, such as large downloads. If payload was repeated X number of time then a warning message is output, where X was a hardcoded value during the project.
-No command line arguements needed. 

