#!/bin/bash
git add .
git commit -a -m "run"
git push 
echo user@ip:
read ip
ssh $ip "cd Mr-Bossman-remove-plz/haraka-avx512  && git pull && ./haraka"
