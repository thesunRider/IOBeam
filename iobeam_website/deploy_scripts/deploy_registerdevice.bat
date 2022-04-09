setlocal EnableDelayedExpansions
rem 1630 - Beam Steerer
rem 1678 - Beam Chopper


set dev_uid=%random%%random%
set dev_type=1630
set dev_version=1.2
set usr_email=testdev@test.com
set usr_pass=testdev@123

set host=https://projectiobeam.herokuapp.com

curl -k -d "device_uid=%dev_uid%&device_type=%dev_type%&device_version=%dev_version%&email=%usr_email%&password=%usr_pass%" ^
  -X POST %host%/polls/connector_registerdevice
