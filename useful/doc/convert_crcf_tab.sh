#!/bin/bash

find $1 -type f -exec ./crcf_tab_convert {} \;
