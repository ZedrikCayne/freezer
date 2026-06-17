valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind.txt build/freezer --info --port 8087
#gdb --args build/freezer --info --port 8087
