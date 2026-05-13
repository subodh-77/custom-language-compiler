all:
	g++ -std=c++17 main.cpp -o nlpc

clean:
	rm -f nlpc *.o *.s program program.nlp

run: all
	./nlpc program.nlp program.s
	nasm -f elf64 program.s -o program.o
	gcc program.o -o program -no-pie
	./program

interact: all
	@echo "====================================================="
	@echo " 🧠 NLP-to-Executable Compiler : Interactive Mode"
	@echo " Type your English code below."
	@echo " Press [Ctrl+D] on an empty line when finished to run."
	@echo "====================================================="
	@cat > program.nlp
	@echo "\n⚙️  Compiling to x86_64 Assembly..."
	@./nlpc program.nlp program.s
	@nasm -f elf64 program.s -o program.o
	@gcc program.o -o program -no-pie
	@echo "🚀 Execution Output:\n"
	@./program
	@echo "\n\n====================================================="