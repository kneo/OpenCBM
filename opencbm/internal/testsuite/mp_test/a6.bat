REM $Id: a6.bat,v 1.2 2006-03-11 15:30:18 wmsr Exp $
:a
del test6.bin
cbmctrl download 8 0xc000 0x0001 test6.bin
goto :a