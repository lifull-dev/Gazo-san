///////////////////////////////////////////////////////////////////////////
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:

//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.

//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.

//   * Neither the names of the copyright holders nor the names of the contributors
//     may be used to endorse or promote products derived from this software
//     without specific prior written permission.

// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall copyright holders or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
///////////////////////////////////////////////////////////////////////////

#include <fstream>
#include "gtest/gtest.h"
#include "imageDiffCalc.cpp"

class ImageDiffCalcTest : public ::testing::Test {
protected:
    std::string want;
    std::string strHHMMSS = GetTimeHHMMSS();
    std::string strYYYYMMDDHHMMSS = GetYYMMDDHHMMSS();
    std::stringstream logBuffer;
    std::streambuf *sbuf = std::clog.rdbuf();

    bool FileExists(const std::string& str) {
        std::ifstream fs(str);
        return fs.is_open();
    }

    bool DirectoryExists(const std::string& str) {
        struct stat buffer;
        return (stat (str.c_str(), &buffer) == 0);
    }

    void StartRecordClog() {
        std::clog.rdbuf(logBuffer.rdbuf());
    }

    std::string GetRecordClog() {
        std::clog.rdbuf(sbuf);
        return logBuffer.str();
    }

    std::string GetYYMMDDHHMMSS(){
        std::time_t rawtime;
        std::tm* timeinfo;
        char buffer [16];
        std::time(&rawtime);
        timeinfo = std::localtime(&rawtime);
        std::strftime(buffer,16,"%Y%m%d_%H%M%S",timeinfo);
        std::string result = buffer;
        return result;
    }

    std::string GetTimeHHMMSS() {
        std::time_t rawtime;
        std::tm* timeinfo;
        char buffer [8];
        std::time(&rawtime);
        timeinfo = std::localtime(&rawtime);
        std::strftime(buffer,8,"%H%M%S",timeinfo);
        std::string result = buffer;
        return result;
    }
};

class CreateDirectoryTest : public ::ImageDiffCalcTest {
protected:
    virtual void TearDown() {
        if (DirectoryExists(want)) {
            remove(want.c_str());
        } else {
            std::cout << "no such file or directory -> " << want << std::endl;
        }
    };
};

class CreatePNGfromCVMATTest : public ::ImageDiffCalcTest {
protected:
    virtual void TearDown() {
        if (FileExists(want)) {
            remove(want.c_str());
        } else {
            std::cout << "no such file -> " << want << std::endl;
        }
    };
};

class GetTimeYYYYMMDDHHMMSSTest : public ::ImageDiffCalcTest {
protected:
    std::string gotYYYYMMDD, gotHHMMSS;
    std::string wantYYYYMMDD, wantHHMMSS;
    virtual void SetUp() {
        SetWantTime();
    };

    void SetWantTime() {
        std::time_t rawtime;
        std::tm* timeinfo;
        char buffer1 [9], buffer2 [9];
        std::time(&rawtime);
        timeinfo = std::localtime(&rawtime);
        std::strftime(buffer1,9,"%Y%m%d",timeinfo);
        std::strftime(buffer2,9,"%H%M%S",timeinfo);
        wantYYYYMMDD = buffer1;
        wantHHMMSS = buffer2;
    }
};

class GetPNGFileTest : public ::CreatePNGfromCVMATTest {
protected:
    std::string SetTimeInFileName (std::string strOutputFolder) {
        std::time_t rawtime;
        std::tm* timeinfo;
        std::string fileName = strOutputFolder + "ImgSeg_%Y%m%d_%H%M%S-0000.png";
        char buffer [50];
        std::time(&rawtime);
        timeinfo = std::localtime(&rawtime);
        std::strftime(buffer,50, fileName.c_str(), timeinfo);
        return buffer;
    }
};

class SetProcesstMsgTest : public ::ImageDiffCalcTest {
protected:
    std::string strFuncName = "testFuncName";
    std::string strStepName = "testStepName";
    int nStepNo = 1;
};

TEST(ConvertColorToGrayTest, FuncConvertColorToGray) {
    cv::Mat got_img(cv::Size(5, 5), CV_8UC3, cv::Scalar(220,68,198));
    cv::Mat want_img(cv::Size(5, 5), CV_8UC3, cv::Scalar(162,162,162));
    ConvertColorToGray(got_img);
    bool isEqual = (sum(want_img != got_img) == cv::Scalar(0,0,0));
    ASSERT_TRUE(isEqual);
}

TEST_F(CreateDirectoryTest, FuncCreateDirectory) {
    want = "./CreateDirectoryTest";
    CreateDirectory(want);
    bool isExists = DirectoryExists(want);
    ASSERT_TRUE(isExists);
}

TEST_F(CreatePNGfromCVMATTest, FuncCreatePNGfromCVMAT) {
    cv::Mat sample_img(cv::Size(5, 5), CV_8UC3, cv::Scalar(220,68,198));
    std::string strOutputFolder = "./";
    CreatePNGfromCVMAT(0, sample_img, strOutputFolder);
    want = GetPNGFile(0, strOutputFolder);
    bool isExists = FileExists(want);
    ASSERT_TRUE(isExists);
}

TEST(ConvertCVMATtoUCHARTest, FuncConvertCVMATtoUCHAR) {
    cv::Mat sample_img(cv::Size(1, 1), CV_8UC3, cv::Scalar(220,68,198));
    unsigned char* got;
    got = ConvertCVMATtoUCHAR(sample_img);
    unsigned char* want = new unsigned char[3];
    want[0] = '\xDC'; //220
    want[1] = '\x44'; //68
    want[2] = '\xC6'; //198
    ASSERT_EQ(*want, *got);
    delete[] want;
}

TEST_F(GetTimeYYYYMMDDHHMMSSTest, FuncGetTimeYYYYMMDD) {
    GetTimeYYYYMMDDHHMMSS(NULL, gotYYYYMMDD, gotHHMMSS);
    ASSERT_EQ(wantYYYYMMDD, gotYYYYMMDD);
}

TEST_F(GetTimeYYYYMMDDHHMMSSTest, FuncGetTimeHHMMSS) {
    GetTimeYYYYMMDDHHMMSS(NULL, gotYYYYMMDD, gotHHMMSS);
    ASSERT_EQ(wantHHMMSS, gotHHMMSS);
}

TEST_F(GetTimeYYYYMMDDHHMMSSTest, FuncGetTimeYYYYMMDDHHMMSS) {
    bool result;
    result = GetTimeYYYYMMDDHHMMSS(NULL, gotYYYYMMDD, gotHHMMSS);
    ASSERT_TRUE(result);
}

TEST_F(GetPNGFileTest, FuncGetPNGFile) {
    std::string strOutputFolder = "./GetPNGFileTest_";
    cv::Mat sample_img(cv::Size(5, 5), CV_8UC3, cv::Scalar(220,68,198));
    CreatePNGfromCVMAT(0, sample_img, strOutputFolder);
    want = SetTimeInFileName(strOutputFolder);
    std::string got = GetPNGFile(0, strOutputFolder);
    ASSERT_EQ(want, got);
}

typedef CreatePNGfromCVMATTest CreatePNGfromUCHARTest;
TEST_F(CreatePNGfromUCHARTest, FuncCreatePNGfromUCHAR) {
    std::string strOutputFolder = "./";
    int nNum = 10;
    int w = 100;
    int h = 100;
    unsigned char* pDstImg = new unsigned char[3];
    CreatePNGfromUCHAR(nNum, w, h, pDstImg, strOutputFolder);
    want = "ImgSeg_" + strYYYYMMDDHHMMSS + "-0000.png";
    bool isExists = FileExists(want);
    ASSERT_TRUE(isExists);
    delete[] pDstImg;
}

TEST_F(SetProcesstMsgTest, FuncSetProcessStartMsg) {
    want = strFuncName + " : Step" + std::to_string(nStepNo) + ". " + strStepName + " --START ( " + strHHMMSS + " )--\n";
    StartRecordClog();
    SetProcessStartMsg(strFuncName, nStepNo, strStepName);
    std::string got = GetRecordClog();
    ASSERT_EQ(want, got);
}

TEST_F(SetProcesstMsgTest, FuncSetProcessEndMsg) {
    want = strFuncName + " : Step" + std::to_string(nStepNo) + ". " + strStepName + " --END ( " + strHHMMSS + " )--\n";
    StartRecordClog();
    SetProcessEndMsg(strFuncName, nStepNo, strStepName);
    std::string got = GetRecordClog();
    ASSERT_EQ(want, got);
}

TEST_F(SetProcesstMsgTest, FuncSetProcessErrorMsg) {
    want = " -> Error : Step" + std::to_string(nStepNo) + "\n";
    StartRecordClog();
    SetProcessErrorMsg(nStepNo);
    std::string got = GetRecordClog();
    ASSERT_EQ(want, got);
}
