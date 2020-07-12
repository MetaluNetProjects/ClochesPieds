#!/bin/bash

ping -q -W 1 -c 1 $1 > /dev/null && echo ok || echo no
