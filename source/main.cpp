#include "../include/App.hpp"


int main(int argc, char** argv)
{
	App* pApp = App::GetInstance();
	pApp->OnExecute();

	delete pApp;

	return 0;
}
