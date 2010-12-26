#!/bin/sh

valgrind --log-file=dump --leak-check=full -v ./SimpleWin &

exit 0
