#!/bin/sh
cd `dirname $0`
FLAG=
#FLAG+="--read-only"
while true; do
    open-stage-control $FLAG -s localhost:18080 -t ./theme-contrast.css -l ./teleco-OSC.json
    sleep 5
done
