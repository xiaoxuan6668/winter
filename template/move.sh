#!/bin/sh

unlink winter/bin/winter
unlink bin/module/libproject_name.so
cp winter/bin/winter bin/project_name
cp lib/libproject_name.so bin/module/