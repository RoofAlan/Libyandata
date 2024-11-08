all:
	@echo "GCC     libkami.c"
	@gcc -c -o libkami.o libkami.c
	@echo "AR      libkami.o"
	@ar rcs libkami.a libkami.o
	
	@echo "GCC     libnotice.c"
	@gcc -c -o libnotice.o libnotice.c
	@echo "AR      libnotice.o"
	@ar rcs libnotice.a libnotice.o
	
	@rm -rf libnotice.o libkami.o