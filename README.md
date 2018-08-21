
mysqlhs
=======

Build Environment
-----------------
* GCC 7.3 - Ubuntu 18.04 LTS
* Visual Studio 2017 - Windows 10

Install MariaDB
---------------
* sudo apt update
* sudo apt upgrade
* sudo apt install mariadb-server
* sudo /etc/init.d/mysql restart

* mysql -u root -p
```
create database test default character set utf8 default collate utf8_general_ci;
use test;
create table movie (id int not null auto_increment primary key, genre varchar(20) not null, title varchar(100) not null, view_count int default 0, key(genre)) engine innodb;
grant all privileges on test.* to 'dbuser'@'localhost' identified by 'passwd' with grant option;
flush privileges;
exit
```

Install HandlerSocket
---------------------
* sudo nano /etc/mysql/conf.d/mariadb.cnf
```
[mysqld]
..
handlersocket_address = 127.0.0.1
handlersocket_port = 9998
handlersocket_port_wr = 9999
```

* mysql -u root -p
* INSTALL PLUGIN handlersocket SONAME 'handlersocket.so';
* exit
* sudo /etc/init.d/mysql restart
* mysql -u root -p
* SHOW PROCESSLIST;
* exit

Install mysqlhs
---------------
* sudo apt install git
* git clone https://github.com/wasppdotorg/mysqlhs.git
* cd mysqhs
* sudo apt install build-essential
* sudo apt install cmake
* mkdir cmake_build
* cd cmake_build
* cmake ..
* make

Run
---
* ./mysqlhs

Memory Leak Check
-----------------
* sudo apt install valgrind
* valgrind ./mysqlhs

