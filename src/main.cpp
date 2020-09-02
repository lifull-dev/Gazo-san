#include <iostream>

int ImgSegMain(int argc, const char** argv);
bool GetTimeHHMMSS(tm* pTM, std::string& strHHMMSS);

int main(int argc, const char** argv)
{
	std::string strHHMMSS_Start;
	GetTimeHHMMSS(NULL, strHHMMSS_Start);

	//ImgSeg
	ImgSegMain(argc, argv);

	std::string strHHMMSS_End;
	GetTimeHHMMSS(NULL, strHHMMSS_End);

	std::clog << "Process time : " << strHHMMSS_Start << " - " << strHHMMSS_End << std::endl;

	return 0;
}