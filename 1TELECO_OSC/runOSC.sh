#!/bin/sh
cd `dirname $0`
FLAG=
#FLAG+="--read-only"
open-stage-control $FLAG -s localhost:18080 -t ./theme-contrast.css -l ./teleco-OSC.json

