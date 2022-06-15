#!/bin/bash

esphome -s external_components_source components ${1:-run} debug-esp32-example.yaml
