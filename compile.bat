@echo on
CD "{ROOT}"
g++ -std=c++11 main.cpp IO.cpp gen.cpp phonoids.cpp rand.cpp
pause
