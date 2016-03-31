#General Sensor Gateway
---------------------------------------------------------------------------
##Introduction
This project is part of the course "System Software" in KU Leuven, Belgium.</br>

For detail information of the project requirement, please check *assignment_2015.pdf*


This code is a sensor gateway, which mimic the Internet commutications and sensor data processing in linux terminal. Some basic data structure, such as circular queue, linked list were implemented in it.</br>

The function of this code includes:
1. collecting sensor data from centain sensor nodes;
2. process and store data via multiple threads.
3. manage MySQL database directly via C codes.


---------------------------------------------------------------------------

##Prerequesite:

MySQL server must be installed. if no database with the setting name exists, the program will crash, so please doulbe check if this database name have been created in your server before you run the main gateway.</br>
For those who is not familiar with mysql server, please go to: https://www.linode.com/docs/databases/mysql/install-mysql-on-ubuntu-14-04</br>


---------------------------------------------------------------------------

##Compiling:

1. compile the sensor_node.c file.</br>
the comment is: *make sensor_node*</br>

2. make the main project via makefile</br>

3. run different sensor nodes in different terminals. </br>
the comment is: *./sensor_node sensor_id time_interval_of_sending_signal host_ip_address host_port_number*</br>
for example: *./sensor_node 4 2 127.0.0.1 1234* </br>
means: make a sensor node that id is 4, sending signal every 2 seconds, the ip address of target sensor gateway is 127.0.0.1, port number is 1234.  </br>

4. run the main program in a new terminal. </br>
the comment is as simple as *./sensor_gateway*</br>


For commercial uses of this code, please contact me first: castoryan1991@gmail.com

Qinrui Yan
