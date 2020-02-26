ALL: contacts contacts_server

contacts: contacts.c
	gcc contacts.c -o contacts

contacts_server: contacts_server.c
	gcc -O contacts_server.c `mysql_config --cflags --libs` -o contacts_server

clean:
	rm contacts contacts_server
