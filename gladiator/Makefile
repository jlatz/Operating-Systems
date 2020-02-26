ALL:  sem_helper colosseum gladiator 

sem_helper: sem_helper.c
	gcc -c sem_helper.c

colosseum: sem_helper colosseum.c
	gcc -c colosseum.c
	gcc colosseum.o sem_helper.o -o colosseum 
	rm colosseum.o
	rm sem_helper.o

gladiator: jlatz2.c
	gcc sem_helper.c jlatz2.c -o jbeasy

clean:
	rm colosseum gladiator 
 
