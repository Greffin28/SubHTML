CXX = g++

FLAGS = -Os -s -static -static-libstdc++

%.o : %.c
	$(CXX) -c -o $@ $< $(FLAGS)

SubHTML.exe : SubHTML.o
	$(CXX) -o $@ $^ $(FLAGS)
