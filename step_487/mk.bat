copy /y .\project\MDK-ARM\Debug\StepYarn.hex .\


hex2pgx.exe --headall --exec-8b 0x8830:StepYarn.hex ssss.hex StepYarn.pgh

hex2pgx.exe --headall --exec-8b 0x8840:StepYarn.hex ssss.hex StepTriangle.pgh