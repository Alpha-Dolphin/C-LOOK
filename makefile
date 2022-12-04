#Ben Elleman and Chris Barlas
#Lab 4 makefile
all:  lab4b lab4 tags headers 

headers: *.c tags
	headers.sh

tags: *.c
	ctags -R .

lab4: lab4.o shared.o
	gcc -g -m64  -o $@ $^ 

plab4: plab4.o
	gcc -lpthread -g -m64  -o $@ $^ 

lab4b: lab4b.o shared.o
	gcc -g -m64  -o $@ $^ 

#change the lab number
lab4.zip:	makefile *.o *.c *.h README_LAB4 
	zip $@ $^
	rm -rf install
	mkdir install
	unzip lab4.zip -d install

# this is our master compiler rule to generate .o files.
# It needs all 4 warnings

%.o:%.c *.h
	gcc -ansi -pedantic -Wimplicit-function-declaration -Wreturn-type -g -c $< -o $@


