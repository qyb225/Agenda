cc := g++-4.8 
FLAGS := -std=c++11 -w
LOCAL := ./build/main.o ./build/AgendaUI.o ./build/AgendaService.o ./build/Storage.o ./build/Meeting.o ./build/Date.o   ./build/User.o  
bin/main: $(LOCAL) 
	@mkdir -p bin
	@mkdir -p data
	$(cc) $(FLAGS) -I ./include $(LOCAL) -o ./bin/main

build/main.o: src/main.cpp  include/AgendaUI.hpp include/AgendaService.hpp include/Storage.hpp include/Meeting.hpp include/Date.hpp include/User.hpp
	@mkdir -p build
	$(cc) $(FLAGS) -I ./include ./src/main.cpp -c -o ./build/main.o

build/AgendaUI.o: src/AgendaUI.cpp include/AgendaUI.hpp include/AgendaService.hpp include/Storage.hpp include/Meeting.hpp include/Date.hpp include/User.hpp
	@mkdir -p build
	$(cc) $(FLAGS) -I ./include ./src/AgendaUI.cpp -c -o ./build/AgendaUI.o

build/AgendaService.o: src/AgendaService.cpp include/AgendaService.hpp include/Storage.hpp include/Meeting.hpp include/Date.hpp include/User.hpp
	@mkdir -p build
	$(cc) $(FLAGS) -I ./include ./src/AgendaService.cpp -c -o ./build/AgendaService.o

build/Storage.o: src/Storage.cpp include/Path.hpp include/Meeting.hpp include/Date.hpp include/User.hpp
	@mkdir -p build
	$(cc) $(FLAGS) -I ./include ./src/Storage.cpp -c -o ./build/Storage.o

build/Meeting.o: src/Meeting.cpp include/Meeting.hpp include/Date.hpp 
	@mkdir -p build
	$(cc) $(FLAGS) -I ./include ./src/Meeting.cpp -c -o ./build/Meeting.o

build/Date.o: src/Date.cpp include/Date.hpp
	@mkdir -p build
	$(cc) $(FLAGS) -I ./include ./src/Date.cpp -c -o ./build/Date.o

build/User.o: src/User.cpp include/User.hpp
	@mkdir -p build
	$(cc) $(FLAGS) -I ./include ./src/User.cpp -c -o ./build/User.o

clean:
	@rm -rf  $(LOCAL) ./bin
	@rm -rf  $(LOCAL) ./build
	@rm -rf  $(LOCAL) ./data
