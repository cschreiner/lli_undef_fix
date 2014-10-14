LLVM_LIBS=$(shell find ../../llvm.build -iname *.a)

../bin/test_APInt_wrap: test_APInt_wrap.cpp $(LLVM_LIBS)
	gcc -std=c++11 -I ../../llvm/include -I ../../llvm.build/include \
	      -c test_APInt_wrap.cpp 	
	g++ -o $@ test_APInt_wrap.o \
	      $(LLVM_LIBS) \
	      -lstdc++ -lpthread -rdynamic -ldl -lncurses

clean: 
	rm ../bin/test_APInt_wrap


