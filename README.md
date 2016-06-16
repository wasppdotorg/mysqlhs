mysqlhs
=======

Build Environment
-----------------
* GCC 4.9 - Debian 8
* Clang 3.4 - FreeBSD 10.2
* Visual Studio 2013 - Windows 7

Install MariaDB
---------------
* sudo apt-get update
* sudo apt-get upgrade
* sudo apt-get install mariadb-server
* sudo /etc/init.d/mysql restart
* sudo mysql -u root -p
```
create database test default character set utf8 default collate utf8_general_ci;
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

Install mysqlhs
-------------
* sudo apt-get install git
* git clone https://github.com/wasppdotorg/mysqlhs.git
* cd mysqhs
* sudo apt-get install build-essential
* sudo apt-get install cmake
* mkdir cmakebuild
* cd cmakebuild
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
* sudo pkg install mariadb100-client
* sudo pkg install mariadb100-server

