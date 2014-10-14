
a.out: test_APInt_wrap.cpp
	gcc -std=c++11 -I ../../llvm/include -I ../../llvm.build/include \
	      -c test_APInt_wrap.cpp 
	g++ -o test_APInt_wrap test_APInt_wrap.o \
	      `find ../../llvm.build/ -iname *.a` \
	      -lstdc++ -lpthread -rdynamic -ldl -lncurses


