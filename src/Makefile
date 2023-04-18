ifeq ($(shell uname), Linux)
	CHKFLAGS=-lgtest -lgtest_main -pthread -lrt -lm -lsubunit
else
	CHKFLAGS=-lgtest -lgtest_main
endif

PF=maze/
DISTDIR=build
ZIPNAME=A1_Maze
all: install dvi tests
install:
	@-mkdir $(DISTDIR)
	@-cd $(DISTDIR) && qmake6 $(CURDIR)/maze/maze.pro
	@-cd $(DISTDIR) && make
	@-cd $(DISTDIR) && make clean
	@-cd $(DISTDIR) && rm -f Makefile
	@-cd $(DISTDIR) && rm -f *.stash
uninstall:
	@rm -rf $(DISTDIR)
clean:
	@rm -rf *.o
dvi:
	@echo "user manual"
	@open A1_Maze.html
dist:
	@mkdir $(ZIPNAME)
	@cp -R maze $(ZIPNAME)/maze
	@cp Makefile $(ZIPNAME)/Makefile
	@cp A1_Maze.texi $(ZIPNAME)/A1_Maze.texi
	@tar -cf $(ZIPNAME).tar $(ZIPNAME)
	@gzip $(ZIPNAME).tar
	@rm -rf $(ZIPNAME)
tests:
	g++ -g maze/model/matrix/*.cc maze/model/cave.cc tests/*.cc maze/model/parser.cc -o test $(CHKFLAGS) -lstdc++fs -std=c++17
	./test
style:
	cp ../materials/linters/.clang-format .
	clang-format -i $(PF)model/*.cc  $(PF)view/*.cc $(PF)main.cc $(PF)model/matrix/*.cc 
	clang-format -i $(PF)model/*.h $(PF)view/*.h $(PF)controller/*.h $(PF)model/matrix/*.h
	clang-format -n $(PF)model/*.cc $(PF)view/*.cc $(PF)main.cc $(PF)model/matrix/*.cc 
	clang-format -n $(PF)model/*.h $(PF)view/*.h $(PF)controller/*.h $(PF)model/matrix/*.h
	rm .clang-format

.PHONY: style tests gcov_report
