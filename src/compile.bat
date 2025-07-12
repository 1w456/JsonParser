@REM g++ -I../include -c main.cpp -o main.o
@REM g++ -I../include -c JObject.cpp -o JObject.o
@REM g++ -I../include -c JsonParser.cpp -o JsonParser.o
@REM g++ main.o JObject.o JsonParser.o -o output

g++ -I../include main.cpp JObject.cpp JsonParser.cpp -o main