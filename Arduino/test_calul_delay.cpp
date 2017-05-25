#include <iostream>
#include <math.h>

#define MAX_DISTANCE 100
#define VITESSE_SON 340

using namespace std;

int main(void)
{
	int delay = static_cast<int>((2.0*MAX_DISTANCE)/(VITESSE_SON)*1000);
	cout << delay << endl;
	return 0;
}
