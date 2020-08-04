#!/bin/bash
git add .
git commit -a -m "run"
git push 
echo ip:
read ip
echo user:
read user
ssh $user@$ip "cd Mr-Bossman-remove-plz  && git pull && ./haraka"
