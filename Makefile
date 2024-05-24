build: client.c helpers.c buffer.c requests.c
	gcc -o client client.c helpers.c buffer.c requests.c



clean:
	rm -f ./a.out ./client


run: 
	./client

archive:
	rm -rf ~/Downloads/322CD_Trifan_Bogdan_Cristian_Tema4PC
	zip ~/Downloads/322CD_Trifan_Bogdan_Cristian_Tema4PC.zip README.md Makefile *.h *c



install-checker:
	rm -rf homework4-public
	git clone https://gitlab.cs.pub.ro/pcom/homework4-public.git


checker-help:
	python3 homework4-public/checker/checker.py --help

check-1:
	python3 homework4-public/checker/checker.py ./client

check-2:
	python3 homework4-public/checker/checker.py --user 'myuser-1337:hunter2' ./client


check-all:
	python3 homework4-public/checker/checker.py ./client  --script ALL
