@echo off
setlocal EnableDelayedExpansion
chcp 65001 >nul
title FileRequester - manualny test

rem =============================================================
rem  FileRequester.exe - kompletny manualny testovaci beh
rem =============================================================
rem  Ako to funguje:
rem   1. Pre kazdy test sa najprv vypise, co sa ma objavit / stat.
rem   2. Stlacenim ENTER sa test spusti (spusti sa FileRequester.exe
rem      s danymi parametrami).
rem   3. Over dialog podla popisu - over predvyplnene hodnoty, potom
rem      vyber subor/priecinok (alebo dialog zrus) podla pokynu.
rem   4. Potvrd vysledok: Y = spravne, N = chyba, S = preskocit.
rem   5. Vysledky sa priebezne zapisuju do test-results.txt.
rem
rem  Pouzitie:
rem   test-manual.bat                 - najde exe sam (Release/Debug)
rem   test-manual.bat "cesta\k.exe"   - pouzije zadanu cestu k exe
rem =============================================================

set "SCRIPT_DIR=%~dp0"
set "EXE="

if not "%~1"=="" set "EXE=%~1"
if not defined EXE if exist "%SCRIPT_DIR%Release\FileRequester.exe" set "EXE=%SCRIPT_DIR%Release\FileRequester.exe"
if not defined EXE if exist "%SCRIPT_DIR%Debug\FileRequester.exe" set "EXE=%SCRIPT_DIR%Debug\FileRequester.exe"
if not defined EXE if exist "%SCRIPT_DIR%FileRequester.exe" set "EXE=%SCRIPT_DIR%FileRequester.exe"

if not defined EXE (
    echo [CHYBA] FileRequester.exe sa nenasiel v Release\, Debug\ ani v priecinku skriptu.
    echo Zostav projekt vo Visual Studiu, alebo zadaj cestu k exe ako parameter:
    echo   %~nx0 "C:\cesta\FileRequester.exe"
    exit /b 1
)
for %%F in ("%EXE%") do set "EXE=%%~fF"

set "LOG=%SCRIPT_DIR%test-results.txt"

set /a TOTAL=0
set /a PASS=0
set /a FAIL=0
set /a SKIP=0

echo. >> "%LOG%"
echo ============================================================ >> "%LOG%"
echo Test beh: %DATE% %TIME% >> "%LOG%"
echo Exe: %EXE% >> "%LOG%"
echo ============================================================ >> "%LOG%"

cls
echo ============================================================
echo   FileRequester.exe - KOMPLETNY MANUALNY TEST
echo ============================================================
echo Testovany subor: %EXE%
echo.
echo Postup pre kazdy test:
echo   1. Precitaj si, co sa ma stat / objavit.
echo   2. Stlac ENTER - test sa spusti.
echo   3. Over dialog podla popisu, potom ho zavri (vyber alebo zrus).
echo   4. Potvrd vysledok: Y = spravne, N = chyba, S = preskocit.
echo.
echo Vysledky sa priebezne zapisuju do: %LOG%
echo Kedykolvek mozes cely beh ukoncit cez CTRL+C.
echo ============================================================
pause

rem =============================================================
rem A. ZAKLADNE PARAMETRE A CHYBOVE STAVY
rem =============================================================
echo.
echo ############################################################
echo # A. ZAKLADNE PARAMETRE A CHYBOVE STAVY
echo ############################################################

set "ID=A1-help"
set "DESC=Vypise sa kompletny zoznam parametrov a priklad pouzitia do konzoly. Nema sa otvorit ziadny dialog. Errorlevel ma byt 0."
call :runtest -help

set "ID=A2-basic-open"
set "DESC=Otvori sa standardny dialog na vyber existujuceho suboru s titulkom Otvor subor - predvoleny rezim je Open. Vyber lubovolny subor - jeho cesta sa vypise do konzoly, errorlevel 0."
call :runtest -t "Otvor subor"

set "ID=A3-unknown-param"
set "DESC=V konzole sa vypise Unknown parameter a strucna sprava Run with -help for usage information - NIE cela napoveda. Ziadny dialog sa neotvori."
call :runtest -neexistujuciParameter hodnota

set "ID=A4-cancel"
set "DESC=Otvor dialog a klikni Zrusit namiesto vyberu suboru. Do konzoly sa nic nevypise, errorlevel ma byt 1."
call :runtest -t "Zrus tento dialog"

rem =============================================================
rem B. REZIMY (OPEN / SAVE / DIRECTORY)
rem =============================================================
echo.
echo ############################################################
echo # B. REZIMY (OPEN / SAVE / DIRECTORY)
echo ############################################################

set "ID=B1-open-explicit"
set "DESC=Dialog na vyber existujuceho suboru - rovnake spravanie ako bez parametrov."
call :runtest -open -t "Vyber existujuci subor"

set "ID=B2-save"
set "DESC=Dialog na ulozenie - cielovy subor nemusi existovat, s poliom na zadanie nazvu."
call :runtest -save -t "Uloz ako"

set "ID=B3-directory"
set "DESC=Dialog umoznuje vybrat LEN priecinok, nie subor - v zozname sa neponukaju subory na vyber."
call :runtest -directory -t "Vyber priecinok"

set "ID=B4-open-save-conflict"
set "DESC=V konzole sa vypise chyba, ze parametre open/save su navzajom v konflikte. Ziadny dialog sa neotvori, errorlevel 1."
call :runtest -open -save -t "Konflikt rezimov"

rem =============================================================
rem C. PREDVYPLNENE HODNOTY
rem =============================================================
echo.
echo ############################################################
echo # C. PREDVYPLNENE HODNOTY
echo ############################################################

set "ID=C1-filename"
set "DESC=Pole s nazvom suboru je vopred vyplnene textom novy_subor.txt."
call :runtest -save -t "Predvyplneny nazov" -filename "novy_subor.txt"

set "ID=C2-ok-label"
set "DESC=Potvrdzovacie tlacidlo ma vlastny text Vybrat namiesto standardneho Otvorit/Ulozit."
call :runtest -t "Vlastny text tlacidla" -ok "Vybrat"

set "ID=C3-path"
set "DESC=Dialog sa otvori priamo v priecinku C:\Windows namiesto standardneho Dokumenty."
call :runtest -t "Zaciatok v C:\Windows" -path "C:\Windows"

set "ID=C4-drawersonly"
set "DESC=V dialogu Open nie je mozne vybrat subor, len priecinok - bez toho, aby islo o samostatny rezim Directory."
call :runtest -open -drawersonly -t "Len priecinky v Open rezime"

rem =============================================================
rem D. FILTER TYPOV SUBOROV
rem =============================================================
echo.
echo ############################################################
echo # D. FILTER TYPOV SUBOROV
echo ############################################################

set "ID=D1-filter-single"
set "DESC=V rozbalovacom zozname typov suborov je len jedna polozka Text a priponou .txt - vidiet len .txt subory."
call :runtest -t "Len .txt subory" -filter "Text|*.txt"

set "ID=D2-filter-multiple"
set "DESC=V rozbalovacom zozname su dve polozky - Text subory s priponou .txt a Vsetky subory, prepinatelne."
call :runtest -t "Viac typov filtra" -filter "Text subory|*.txt|Vsetky subory|*.*"

set "ID=D3-filter-bad"
set "DESC=V konzole sa vypise Error - bad filter, kedze pocet casti oddelenych znakom pipe je neparny. Dialog sa napriek tomu otvori bez filtra."
call :runtest -t "Zly format filtra" -filter "Text|*.txt|Neparne"

goto :summary

rem =============================================================
rem Pomocna rutina: spusti jeden test podla premennych ID a DESC,
rem argumenty pre FileRequester.exe su prevzate z parametrov call-u.
rem =============================================================
:runtest
set ARGS=%*
set /a TOTAL+=1
cls
echo ============================================================
echo TEST !ID!   (%TOTAL%. v poradi)
echo ------------------------------------------------------------
echo Ocakavane:
echo   !DESC!
echo.
echo Prikaz: "%EXE%" !ARGS!
echo ============================================================
echo Stlac ENTER pre spustenie testu...
pause >nul

"%EXE%" !ARGS!
set "RC=%ERRORLEVEL%"

echo.
echo Dialog sa zatvoril. Navratovy kod (errorlevel): %RC%
choice /c YNS /n /m "Zodpoveda vysledok popisu vyssie? [Y]ano [N]ie [S]kip: "
if errorlevel 3 goto :runtest_skip
if errorlevel 2 goto :runtest_fail

echo [ OK ] !ID! - !DESC! ^| errorlevel=!RC! >> "%LOG%"
set /a PASS+=1
goto :runtest_done

:runtest_fail
echo [FAIL] !ID! - !DESC! ^| errorlevel=!RC! >> "%LOG%"
set /a FAIL+=1
goto :runtest_done

:runtest_skip
echo [SKIP] !ID! - !DESC! ^| errorlevel=!RC! >> "%LOG%"
set /a SKIP+=1

:runtest_done
exit /b 0

:summary
echo.
echo ============================================================
echo VYSLEDOK: %PASS% OK, %FAIL% chyba, %SKIP% preskocenych, spolu %TOTAL%
echo Podrobny log: %LOG%
echo ============================================================
echo Zhrnutie: %PASS% OK, %FAIL% chyba, %SKIP% preskocenych, spolu %TOTAL% >> "%LOG%"
endlocal
pause
exit /b 0
