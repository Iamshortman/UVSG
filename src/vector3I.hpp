#ifndef VECTOR3I_HPP
#define VECTOR3I_HPP

class vector3I
{

public:
	vector3I(){ x = 0; y = 0; z = 0; };
	vector3I(int X, int Y, int Z){ x = X; y = Y; z = Z; };
	unsigned int x;
	unsigned int y;
	unsigned int z;
};

#endif //VECTOR3I_HPP