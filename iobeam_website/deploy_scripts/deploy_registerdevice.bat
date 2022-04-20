@echo off
setlocal EnableDelayedExpansion
rem 1630 - Beam Steerer
rem 1631 - Beam Chopper

set /p us_uid=Enter uid:
set dev_uid=%random%%random%
set dev_type=%1
set dev_version=1.2
set usr_email=testdev@test.com
set usr_pass=testdev@123

set host=http://localhost:8000
rem https://projectiobeam.herokuapp.com

set url_fetch="device_uid=%dev_uid%&device_type=%dev_type%&device_version=%dev_version%&user_uid=%us_uid%"
echo.
echo URL_FETCH:
echo %url_fetch%
echo.
echo.

curl -k -d %url_fetch% -X POST %host%/polls/connector_registerdevice -o out.html
