# 1m-block

## How to use
### requirement
```
sudo apt install libnetfilter-queue-dev
sudo apt install g++
sudo apt-get install sqlite3 libsqlite3-dev
```

### Use
```shell
syntax : 1m-block <site list file>
sample : 1m-block top-1m.db
```

## Description
* `<site list file>` is host name set which we target to ban
* Should run this as root user
* execute `./start.sh` before run this
```shell
sudo iptables -F
sudo iptables -A OUTPUT -j NFQUEUE --queue-num 0
sudo iptables -A INPUT -j NFQUEUE --queue-num 0
```
* execute `./stop.sh` after run this
```shell
sudo iptables -F
```

## code
* I use sqlite3 to search url
* if you want to make new database, you should use the .csv file as input.
* the other case, you should use .db file.

