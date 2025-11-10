test: btree_test.o
	g++ -o test test_btree_example.o
	
btree_test.o: test_btree_example.cpp btree.h btree.cpp btree_delete.cpp 
	g++ -c test_btree_example.cpp