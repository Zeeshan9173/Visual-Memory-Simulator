@echo off
setlocal EnableDelayedExpansion

:: ==========================================
:: CONFIGURATION
:: ==========================================
set SIM_SOURCE=main.cpp
:: ==========================================

echo [VMS] Starting Visual Memory Simulator Workflow (Assuming trace.out exists)...

:: 1. Cleanup old Node servers on Port 3000
echo [VMS] Cleaning up old servers on port 3000...
for /f "tokens=5" %%a in ('netstat -a -n -o ^| findstr :3000') do (
    taskkill /F /PID %%a >nul 2>&1
)

:: 2. Verify trace.out exists
if not exist "trace.out" (
    echo [ERROR] trace.out not found! Please ensure it is in the current directory.
    pause
    exit /b
)
echo [VMS] trace.out found!

:: 3. Compile the Simulator Engine
echo [VMS] Compiling the C++ Simulator Engine...
g++ "%SIM_SOURCE%" -o cache_sim.exe
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to compile %SIM_SOURCE%
    pause
    exit /b
)

:: 4. Start the Node.js Backend Server
echo [VMS] Starting Node.js backend (server.js)...
start /B node server.js

:: Give the server 2 seconds to boot up
timeout /t 2 /nobreak >nul

:: 5. Open the Frontend in Windows Browser
echo [VMS] Opening index.html in your default browser...
start index.html

echo [VMS] All set! Click 'Run Simulation' on the webpage.
echo [VMS] Keep this window open. Close it to stop the Node server.
pause