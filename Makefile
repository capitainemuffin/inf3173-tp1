programme=tp1
options=-Wall -pedantic -std=c11

.PHONY : clean default push test debug

default : $(programme)

$(programme) : $(programme).o
	gcc $< -o $@ $(options)

$(programme).o : $(programme).c
	gcc -c $< -o $@ $(options)

test : $(programme)
	./tests/$@.sh $(fichier)

debug : tp1.c
	gcc -c $< -o tp1.development.o $(options) -DDEBUG
	gcc -o tp1.development tp1.development.o $(options) -DDEBUG
	./tp1.development $(fichier)

clean : 
	@rm -fr *.out *.o tp1 *.development

push :
	git add .
	git commit -m 'automated commit'
	git push origin master  