#!/bin/bash

esphome -s external_components_source components ${1:-run} ${2:-smartsolar-mppt-esp8266-example.yaml}
