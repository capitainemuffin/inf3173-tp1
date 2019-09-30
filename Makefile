programme=tp1
options=-Wall -pedantic -std=c11

.PHONY : clean default push  

default : $(programme)

$(programme) : $(programme).o
	gcc $< -o $@ $(options)

$(programme).o : $(programme).c
	gcc -c $< -o $@ $(options)

test : $(programme)
	./$(programme) $@

clean : 
	rm -fr *.out *.o tp1

push :
	git add .
	git commit -m 'automated commit'
	git push origin master  