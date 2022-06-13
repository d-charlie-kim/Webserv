#include <iostream>
#include <sstream>

int main(void) {
	std::string test = "123  abc       hhhh";
	std::string printing;
	std::stringstream iss(test);

	while (iss >> printing)
	{
		std::cout << iss.eof() << std::endl;
		std::cout << printing << std::endl;
	}
	std::cout << iss.eof() << std::endl;
	iss >> printing;
	std::cout << iss.good() << std::endl;
	return 0;
}

//뒤에 공백이 없거나 --> eof is true
//공백 밖에 없다면   --> eof is false but fail is true  -> 확실하지 않다면, 오른쪽 공백을 지우는 작업 | 오른쪽 공백 없이 conf가 들어온다고 가정.
