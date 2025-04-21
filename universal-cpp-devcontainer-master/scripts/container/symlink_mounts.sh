#!/bin/sh

rmdir ~/workspace && ln -s /mnt/workspace/ ~/workspace
if [ -e /mnt/.ssh/ ]; then rm -r ~/.ssh/ && ln -s /mnt/.ssh/ ~/.ssh; fi
if [ -e /mnt/.gitconfig ]; then ln -s /mnt/.gitconfig ~/.gitconfig; fi
if [ -e /mnt/.gitignore ]; then ln -s /mnt/.gitignore ~/.gitignore; fi