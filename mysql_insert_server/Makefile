ALL: contacts mysql_insert_server

contacts: contacts.c
	gcc contacts.c -o contacts

mysql_insert_server: mysql_insert_server.c
	gcc -O mysql_insert_server.c `mysql_config --cflags --libs` -o mysql_insert_server

clean:
	rm contacts mysql_insert_server
