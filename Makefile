CXX      := g++
CXXFLAGS := -std=c++14 -O2 -Wall -Wextra -Wpedantic
INC      := -Isrc -Ilib/fips202

LIB_SRC  := lib/fips202/fips202.c lib/fips202/prng.cpp
CORE_SRC := src/bigint.cpp src/dh.cpp src/primality.cpp

BIN := bin

all: $(BIN)/dh

$(BIN):
	mkdir -p $(BIN)

# fips202.c được biên dịch bằng g++ như trong code mẫu (header có extern "C")
$(BIN)/dh: src/main.cpp $(CORE_SRC) $(LIB_SRC) | $(BIN)
	$(CXX) $(CXXFLAGS) -x c++ lib/fips202/fips202.c -x c++ src/main.cpp $(CORE_SRC) lib/fips202/prng.cpp $(INC) -o $@

$(BIN)/test_bigint: tests/test_bigint.cpp $(CORE_SRC) $(LIB_SRC) | $(BIN)
	$(CXX) $(CXXFLAGS) -x c++ lib/fips202/fips202.c -x c++ tests/test_bigint.cpp $(CORE_SRC) lib/fips202/prng.cpp $(INC) -o $@

$(BIN)/test_prime: tests/test_prime.cpp $(CORE_SRC) $(LIB_SRC) | $(BIN)
	$(CXX) $(CXXFLAGS) -x c++ lib/fips202/fips202.c -x c++ tests/test_prime.cpp $(CORE_SRC) lib/fips202/prng.cpp $(INC) -o $@

run: $(BIN)/dh
	./$(BIN)/dh

test-bigint: $(BIN)/test_bigint
	./$(BIN)/test_bigint

test-prime: $(BIN)/test_prime
	./$(BIN)/test_prime

# Chạy chương trình rồi nhờ Python kiểm tra độc lập
check: $(BIN)/dh
	./$(BIN)/dh | python3 tools/check_dh.py

clean:
	rm -rf $(BIN)

.PHONY: all run test-bigint test-prime check clean
