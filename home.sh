#!/bin/sh -e

NODE=$1
CAN=can0

canmat -f $CAN nmt $NODE reset-node
sleep 1


canmat -f $CAN dict-dl $NODE 'modes of operation' 0x6

canmat -f $CAN nmt $NODE start
sleep 1

canmat -f $CAN dict-dl $NODE 'controlword' 0x6
canmat -f $CAN dict-dl $NODE 'controlword' 0x7
canmat -f $CAN dict-dl $NODE 'controlword' 0xf

canmat -f $CAN dict-dl $NODE 'controlword' 0xff
