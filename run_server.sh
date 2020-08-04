#!/bin/bash
git add .
git commit -a -m "run"
git push 
echo ip:
read ip
echo password:
read pass
echo $pass > ssh root@$ip "echo hi" > test
