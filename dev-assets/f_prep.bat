@echo off

:: convert *.egg into *.egg.bam
for %%f in (./assets/*.egg) do (
    echo %%~nf
    Rem must be *.egg.bam, otherwise fails
    egg2bam -ps rel -o "./assets/%%~nf.egg.bam" "./assets/%%~nf.egg"
)

:END
endlocal
