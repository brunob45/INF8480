#!/bin/bash 
tracename=$(($(date +%s%N)/1000000))
lttng create --output=trace_files/$tracename
lttng enable-channel -u --subbuf-size 16384 --num-subbuf 128 ustchannel
lttng enable-event -u -a -c ustchannel
lttng add-context -u -t vpid -t vtid -c ustchannel
lttng enable-channel -k --subbuf-size 16384 --num-subbuf 512 mykernelchannel
lttng enable-event -k block_*,lttng_statedump_*,sched_*,net_*,irq_*,timer_* -c mykernelchannel
lttng enable-event -k --syscall -a -c mykernelchannel
lttng add-context -k -t tid -t pid -t procname -c mykernelchannel
lttng start
./server &