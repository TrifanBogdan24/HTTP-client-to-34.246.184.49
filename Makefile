build: client.c helpers.c buffer.c parson.c
	gcc -o client client.c helpers.c buffer.c parson.c



install-checker:
	rm -rf homework4-public
	git clone https://gitlab.cs.pub.ro/pcom/homework4-public.git


clean:
	rm -f ./a.out ./client


run: 
	./client

archive:
	rm -rf ~/Downloads/322CD_Trifan_Bogdan_Cristian_Tema4PC
	zip -r ~/Downloads/322CD_Trifan_Bogdan_Cristian_Tema4PC Makefile *.h *c *.cpp
