#include "../include/App.hpp"


int main(int argc, char** argv)
{
	App* pApp = App::getInstance();
	pApp->OnExecute();

	return 0;
}
