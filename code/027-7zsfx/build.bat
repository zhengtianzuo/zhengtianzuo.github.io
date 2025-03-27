7za a archive.7z "release\*"
copy /b /y 7zS.sfx+config.txt+archive.7z installer.exe
del archive.7z
