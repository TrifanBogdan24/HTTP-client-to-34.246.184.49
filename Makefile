build:
	g++ -I./include server.cpp -o server



install-checker:
	rm -rf homework4-public
	git clone https://gitlab.cs.pub.ro/pcom/homework4-public.git


clean:
	rm -f ./a.out ./server


run:
	./server

archive:
	rm -rf ~/Downloads/322CD_Trifan_Bogdan_Cristian_Tema4PC
	zip -r ~/Downloads/322CD_Trifan_Bogdan_Cristian_Tema4PC Makefile *.h *c *.cpp
