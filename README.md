cpp-Socket
==========

Some class to help to construct client/server with soket and serilized datas.

Test on Ubuntu 12.10 x64, but work everywhere.

Class
-----

All class are in ntw namespace.

* Serializer
    * allow you to add datas to u buffer that are convert in Big Endian for the network
    * allow you to add custom operators \<\< and \>\>  for easily use vour own class/stuct to convert (see src/server-main.cpp class Test for exemple)

* Socket
    * use C socket, and put them in a class to use them more  easily

* SocketSerialized
    * Extand Socket and Serializer.
    * Simply add that you want in this soket (using \<\< operator) send it, and get data (with \>\> operator)


Exemples
========


You can find exemples of use in:
* src/server-main.cpp
* src/client-main.cpp


You can build the exemple with "make".



