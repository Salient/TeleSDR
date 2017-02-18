HEADER = -I/usr/local/include
LIBB = -L/usr/local/lib
LIBRA = -lportaudio

pexam: pexam.o
	gcc $(HEADER)   $(LIBB)  $<  -o  $@ $(LIBRA)

teleclient: teleclient.o
	gcc $(HEADER)   $(LIBB)  $<  -o  $@ $(LIBRA)

pexam.o: 
	gcc -c -o pexam.o $(HEADER) example.c 

TeleSDR: main.c
	gcc -I /usr/local/include main.c -o TeleSDR
	
clean:
	rm -f ./*.o; rm pexam
