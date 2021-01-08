a = 0

while a < 1

PRINT "Bitte geben Sie etwas in die Konsole ein. q zum Beenden"
INPUT ch$

PRINT "Eingabe lautet:"
PRINT ch$

if(ch$ = "q") then
a = 1
endif

wend
