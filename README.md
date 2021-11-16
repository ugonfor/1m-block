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

## Database

```shell
ugonfor@ubuntu:~/Desktop/1m-block$ sqlite3 ./top-1m.db 
SQLite version 3.31.1 2020-01-27 19:55:54
Enter ".help" for usage hints.
sqlite> select * from URLS where id < 10;
1|google.com
2|youtube.com
3|tmall.com
4|baidu.com
5|qq.com
6|sohu.com
7|facebook.com
8|taobao.com
9|360.cn
sqlite> .schema
CREATE TABLE URLS(Id INT, Name TEXT);
```
