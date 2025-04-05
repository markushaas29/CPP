#!/usr/bin/python3
import os
from shutil import copyfile
import shutil, errno
import datetime
import sys

#Zielverzeichnis auf Festplatte
MusicDirectory="/home/markus/Musik"
DvdDirectory="/media/markus/DVD_VIDEO_RECORDER/"
DvdDirectoryFolder="VIDEO_TS"
MusicFile="VTS_01_1.VOB"
FileName=datetime.datetime.now().strftime('%d%m%Y')


#if os.path.exists(DvdDirectory+DvdDirectoryFolder):
if 4>3:
    musicFile = DvdDirectory+DvdDirectoryFolder+'/'+MusicFile
    print("TEST" + musicFile)
    destinationFile = MusicDirectory + '/' + MusicFile
    name = sys.argv[1]
    if os.path.exists(musicFile):
        print("Copy" + musicFile + " to " + destinationFile)
        copyfile(musicFile, destinationFile)
        
        fileName = name + "_"+FileName
        if len(sys.argv) == 2:
            command = 'ffmpeg -i ' + destinationFile + ' "'+ fileName +'.mp3"'
        elif len(sys.argv) == 3:
            time = sys.argv[2]
            command = 'ffmpeg -ss ' + time + ' -i ' + destinationFile + ' "' + fileName +'.mp3"'
        else:
            print("Wrong input arguments!")
        print("Execute command: " + command)
        os.system(command)
        print("File created: " + fileName)
        sourceFile = os.path.abspath(".") + '/' + fileName +'.mp3'
        shutil.move(sourceFile, MusicDirectory)
        print("Move " + fileName + ".mp3 to " + MusicDirectory)
    else:
        print("File not found!")
else:
    print("Directory not found!")

