# PI NAS - PI Source Code

### Please make sure to perform the following:

- sudo apt install bc (Required for uptime.sh)
- Add the full paths to the top of the "get_stats_verbose.c" file before compiling it
- Add a crontask using the command "crontab -e" and appending @reboot "PATH TO COMPILIED 'get_stats_verbose.c'"