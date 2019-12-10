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
    virtual void SetUp() {
        mkdir("image_diff_temp", S_IRWXU);
    };
    virtual void TearDown() {
		if (system("exec rm -r ./image_diff_temp") != 0)
		{
			std::cerr << "Fail in delete temp directoty." << std::endl;
		}
    };

    bool FileExists(const std::string& str) {
        std::ifstream fs(str);
        return fs.is_open();
    }
};

class ImgSeg01Test : public ::ImageDiffCalcTest {
protected:
    int CountFileInDirectory(const std::string& str)
    {
        std::array<char, 3> buffer;
        std::string resultStr;
        int resultInt;
        std::string cmd = "ls ./" + str + " | wc -l";
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
        if (!pipe) {
            throw std::runtime_error("popen() failed!");
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            resultStr += buffer.data();
        }
        resultInt = std::atoi(resultStr.c_str());
        return resultInt;
    }
};

class ImgSeg03Test : public :: ImageDiffCalcTest {
protected:
    std::map<int, std::vector<std::string> > strPartFileListMap;
    void SetPartFileListMap()
    {
        std::vector<std::string> partsVector;
        partsVector.push_back("tests/images/image_diff_temp/new/ImgSeg-0001.png");
        partsVector.push_back("tests/images/image_diff_temp/new/ImgSeg-0002.png");
        partsVector.push_back("tests/images/image_diff_temp/new/ImgSeg-0003.png");
        partsVector.push_back("tests/images/image_diff_temp/new/ImgSeg-0009.png");
        partsVector.push_back("tests/images/image_diff_temp/new/ImgSeg-0010.png");
        strPartFileListMap[2] = partsVector;
    }
};

TEST(ImgSeg00Test, SameImage) {
    int result;
    result = ImgSeg00("tests/images/test_image_old.png","tests/images/test_image_old.png");
    ASSERT_EQ(-1, result);
}

TEST(ImgSeg00Test, DifferentImage) {
    int result;
    result = ImgSeg00("tests/images/test_image_old.png","tests/images/test_image_new.png");
    ASSERT_EQ(0, result);
}

TEST(ImgSeg00Test, LoadImageFailed) {
    int result;
    result = ImgSeg00("wrong/path.png","wrong/path.png");
    ASSERT_EQ(-2, result);
}

TEST_F(ImgSeg01Test, CheckNumOfParts) {
    int got;
    ImgSeg01("tests/images/test_image_old.png","./image_diff_temp/");
    got = CountFileInDirectory("image_diff_temp");
    ASSERT_EQ(8, got);
}

TEST_F(ImgSeg03Test, CreateDiff) {
    std::string want = "./image_diff_temp/ImgSeg03_diff.png";
    SetPartFileListMap();
    ImgSeg03("tests/images/test_image_old.png", strPartFileListMap, "./image_diff_temp/ImgSeg03");
    bool isExists = FileExists(want);
    ASSERT_TRUE(isExists);
}