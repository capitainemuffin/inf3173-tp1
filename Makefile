programme=tp1
options=-Wall -pedantic -std=c11

.PHONY : clean default push test

default : $(programme)

$(programme) : $(programme).o
	gcc $< -o $@ $(options)

$(programme).o : $(programme).c
	gcc -c $< -o $@ $(options)

test : $(programme)
	./tests/$@.sh $(fichier)

clean : 
	@rm -fr *.out *.o tp1 *.development

push :
	git add .
	git commit -m 'automated commit'
	git push origin master  