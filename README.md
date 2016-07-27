
mysqlhs
=======

Build Environment
-----------------
* GCC 4.9 - Debian 8.5 (Boost 1.55)
* Clang 3.4 - FreeBSD 10.3 (Boost 1.55)

Install MariaDB
---------------
* sudo apt-get update
* sudo apt-get upgrade
* sudo apt-get install mariadb-server
* sudo /etc/init.d/mysql restart
* sudo mysql -u root -p
```
create database test default character set utf8 default collate utf8_general_ci;
create table movie (id int not null auto_increment primary key, genre varchar(20) not null, title varchar(100) not null, view_count int default 0, key(genre)) engine innodb;
grant all privileges on test.* to 'dbuser'@'localhost' identified by 'passwd' with grant option;
flush privileges;
exit
```

Install HandlerSocket
---------------------
* sudo nano /etc/mysql/my.cnf
```
[mysqld]
..
handlersocket_address = 127.0.0.1
handlersocket_port = 9998
handlersocket_port_wr = 9999
```

* sudo mysql -u root -p
* INSTALL PLUGIN handlersocket SONAME 'handlersocket.so';
* exit
* sudo /etc/init.d/mysql restart
* sudo mysql -u root -p
* SHOW PROCESSLIST;
* exit

Install Boost Libraries
-----------------------
* sudo apt-get install libboost-all-dev

Install mysqlhs
---------------
* sudo apt-get install git
* git clone https://github.com/wasppdotorg/mysqlhs.git
* cd mysqhs
* sudo apt-get install build-essential
* sudo apt-get install cmake
* mkdir cmake_build
* cd cmake_build
* cmake ..
* make

Run
---
* ./mysqlhs

Memory Leak Check
-----------------
* sudo apt-get install valgrind
* valgrind ./mysqlhs

For FreeBSD
-----------
* sudo pkg install mariadb101-client
* sudo pkg install mariadb101-server

