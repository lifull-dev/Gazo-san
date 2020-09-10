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
    std::stringstream logBuffer;
    std::streambuf *sbufcerr = std::cerr.rdbuf();
    std::streambuf *sbufcout = std::cout.rdbuf();
    std::streambuf *sbufclog = std::clog.rdbuf();

    bool FileExists(const std::string& str) {
        std::ifstream fs(str);
        return fs.is_open();
    }
    void StartRecordCerr() {
        std::cerr.rdbuf(logBuffer.rdbuf());
    }

    std::string GetRecordCerr() {
        std::cerr.rdbuf(sbufcerr);
        return logBuffer.str();
    }

    void StartRecordCout() {
        std::cout.rdbuf(logBuffer.rdbuf());
    }

    std::string GetRecordCout() {
        std::cout.rdbuf(sbufcout);
        return logBuffer.str();
    }

    void StartRecordClog() {
        std::clog.rdbuf(logBuffer.rdbuf());
    }

    std::string GetRecordClog() {
        std::clog.rdbuf(sbufclog);
        return logBuffer.str();
    }

    std::string FindChangeImage(){
        std::array<char, 50> buffer;
        std::string resultStr;
        std::string cmd = "ls -1 image_difference_add.png image_difference_delete.png";
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
        if (!pipe) {
            throw std::runtime_error("popen() failed!");
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            resultStr += buffer.data();
        }
        return resultStr;
    }
};

class ImgSegMainTest : public ::ImageDiffCalcTest {
protected:
    std::string deleteList[3] = {"./image_difference_diff.png", "./image_difference_add.png", "./image_difference_delete.png"};
    virtual void TearDown() {
        logBuffer.str("");
        for(int i=0; i<3; i++){
            std::string image = deleteList[i];
            if (FileExists(image)) {
                remove(image.c_str());
            } else {
                std::cout << "no such file -> " << image << std::endl;
            }
        }
    };
};

TEST_F(ImgSegMainTest, GeneralExec) {
    std::string want = "./image_difference_diff.png";
    int argc = 3;
    const char* argv[] = {(char*)"./test", (char*)"tests/images/test_image_new.png", (char*)"tests/images/test_image_old.png"};
    ImgSegMain(argc, argv);
    bool isExists = FileExists(want);
    ASSERT_TRUE(isExists);
}

TEST_F(ImgSegMainTest, NotEnoughArgs) {
    int argc = 2;
    const char* argv[] = {(char*)"./test", (char*)"Not Enough"};
    std::string want;
    want.append("Not enough input\n ");
    want.append("-> 1st : Relative path for new color image file\n ");
    want.append("-> 2nd : Relative path for old color image file\n ");
    want.append("-> 3rd : Result file name prefix (default: image_difference) \n");
    StartRecordCerr();
    ImgSegMain(argc, argv);
    std::string got = GetRecordCerr();
    ASSERT_EQ(want, got);
}

TEST_F(ImgSegMainTest, HelpOption) {
    int argc = 2;
    const char* argv[] = {(char*)"./test", (char*)"--help"};
    std::string want;
    want.append("\nUsage:\n  options [OPTION...] positional parameters\n\n  ");
    want.append("-v, --verbose              Enable verbose output message\n  ");
    want.append("    --create-change-image  Generate 2 more output files.\n  ");
    want.append("                           1.Output_delete.png: An image shows decreasing part as the green\n  ");
    want.append("                           rectangle on the old image. 2.Output_add.png: An\n  ");
    want.append("                           image shows increasing part as the green rectangle\n  ");
    want.append("                           on the new image.\n  ");
    want.append("-h, --help                 Print help\n\n");
    StartRecordCout();
    ImgSegMain(argc, argv);
    std::string got = GetRecordCout();
    ASSERT_EQ(want, got);
}

TEST_F(ImgSegMainTest, VerboseOption) {
    int argc = 4;
    const char* argv[] = {(char*)"./test", (char*)"tests/images/test_image_new.png", (char*)"tests/images/test_image_old.png", (char*)"-v"};
    StartRecordClog();
    ImgSegMain(argc, argv);
    std::string got = GetRecordClog();
    ASSERT_GT(got.length(), 0);
}

TEST_F(ImgSegMainTest, CreateChangeImageOption) {
    std::string want = "image_difference_add.png\nimage_difference_delete.png\n";
    int argc = 4;
    const char* argv[] = {(char*)"./test", (char*)"tests/images/test_image_new.png", (char*)"tests/images/test_image_old.png", (char*)"--create-change-image"};
    ImgSegMain(argc, argv);
    std::string got = FindChangeImage();
    ASSERT_EQ(want, got);
}