arpcheck
--------

Linux kernel module that checks for duplicated entries in the ARP table.
This is part of a set of linux kernel programming exercises. The first proposed task is to develop a kernel module aimed to check the ARP table to detect when something fishy is going on. Requisites:

*	Solution has to be a kernel module, as it can be done without harming the kernel tree. ARP checking can be easily done in user space reading /proc/net/arp but that won't help in undestanding how the linux kernel works.
*	run checks at regular intervals of time without any busy waiting.
*	Checks should detect duplicated entries in the ARP table. That's an acceptable approssimation to detect arp spoofing.

[read more](http://rikiji.it/post/26)
