#include <conio.h>

namespace Silver {

	__declspec(dllimport) void Print();

}

void main()
{
	Silver::Print();
	_getch();
}