@echo off

REM 切到 bat 所在目錄（tools）
cd /d %~dp0

REM 設定 data 資料夾路徑（相對於 tools）
set DATA_DIR=..\data

for %%i in (%DATA_DIR%\*.png) do (
    echo Converting %%~nxi ...
    python img2rgb565.py "%%i" "%DATA_DIR%\%%~ni.raw"
)

echo Done!
pause