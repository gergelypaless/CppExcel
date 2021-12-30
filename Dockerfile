FROM ubuntu:latest
FROM gcc:latest

RUN apt-get update
RUN apt-get -y install cmake

COPY ./src /usr/src/CppExcel/src
COPY ./test /usr/src/CppExcel/test
COPY ./CMakeLists.txt /usr/src/CppExcel/CMakeLists.txt
WORKDIR /usr/src/CppExcel

RUN cmake -B ./build/Release -DCMAKE_BUILD_TYPE=Release
RUN cmake --build ./build/Release

CMD ./build/Release/CppExcel < test/input.txt > test_output.txt test/testExcel.csv -sep , && diff test_output.txt test/output.txt && rm test_output.txt

#ENTRYPOINT ./build/Release/CppExcel

