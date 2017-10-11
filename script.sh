#!/bin/bash

for i in {1..219}
do
  sshpass -p 'cloud123' ssh cloud@172.16.254.$i 'echo cloud123 | sudo -S echo 'test' | wall'
done
