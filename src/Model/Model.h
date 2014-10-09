#include <string>

class Model
{
public:
	Model(std::string fileName);
	virtual void Render() = 0;
	const std::string FileName;
};


//No Reason to make this its own Class.
Model::Model(std::string fileName)
:FileName(fileName)
{
//Thats All we need todo
}