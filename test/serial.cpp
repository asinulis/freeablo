#include <gtest/gtest.h>

#include <serial/bitstream.h>

#define private public
#include "../apps/freeablo/faworld/position.h"

TEST(Serial, TestBoolPingPong)
{
    std::vector<uint8_t> buf(10, 0);

    Serial::WriteBitStream write(&buf[0], buf.size());
    for (uint32_t i = 0; i < buf.size() * 8; i++)
    {
        bool tmp = i % 2 == 0;
        Serial::Error::Error success = write.handleBool(tmp);
        ASSERT_EQ(success, Serial::Error::Success);
    }

    Serial::ReadBitStream read(&buf[0], buf.size());
    for (uint32_t i = 0; i < buf.size() * 8; i++)
    {
        bool val;
        Serial::Error::Error success = read.handleBool(val);
        ASSERT_EQ(success, Serial::Error::Success);

        bool groundTruth = i % 2 == 0;
        ASSERT_EQ(val, groundTruth);
    }
}

TEST(Serial, TestBoolPredefined)
{
    std::vector<uint8_t> buf(10, 0);

    std::vector<bool> groundTruthVec(buf.size() * 8, false);

    groundTruthVec[4] = true;
    groundTruthVec[15] = true;
    groundTruthVec[3] = true;
    groundTruthVec[12] = true;

    Serial::WriteBitStream write(&buf[0], buf.size());
    for (uint32_t i = 0; i < buf.size() * 8; i++)
    {
        bool tmp = groundTruthVec[i];
        Serial::Error::Error success = write.handleBool(tmp);
        ASSERT_EQ(success, Serial::Error::Success);
    }

    Serial::ReadBitStream read(&buf[0], buf.size());
    for (uint32_t i = 0; i < buf.size() * 8; i++)
    {
        bool val;
        Serial::Error::Error success = read.handleBool(val);
        ASSERT_EQ(success, Serial::Error::Success);

        bool groundTruth = groundTruthVec[i];
        ASSERT_EQ(val, groundTruth);
    }
}

TEST(Serial, TestBoolOverflow)
{
    uint8_t v = 0;
    Serial::WriteBitStream write(&v, 1);
    
    bool tmp = false;
    ASSERT_EQ(write.handleBool(tmp), Serial::Error::Success);
    ASSERT_EQ(write.handleBool(tmp), Serial::Error::Success);
    ASSERT_EQ(write.handleBool(tmp), Serial::Error::Success);
    ASSERT_EQ(write.handleBool(tmp), Serial::Error::Success);
    ASSERT_EQ(write.handleBool(tmp), Serial::Error::Success);
    ASSERT_EQ(write.handleBool(tmp), Serial::Error::Success);
    ASSERT_EQ(write.handleBool(tmp), Serial::Error::Success);
    ASSERT_EQ(write.handleBool(tmp), Serial::Error::Success);

    ASSERT_EQ(write.handleBool(tmp), Serial::Error::EndOfStream);

    Serial::ReadBitStream read(&v, 1);

    bool testVal;
    ASSERT_EQ(read.handleBool(testVal), Serial::Error::Success);
    ASSERT_EQ(read.handleBool(testVal), Serial::Error::Success);
    ASSERT_EQ(read.handleBool(testVal), Serial::Error::Success);
    ASSERT_EQ(read.handleBool(testVal), Serial::Error::Success);
    ASSERT_EQ(read.handleBool(testVal), Serial::Error::Success);
    ASSERT_EQ(read.handleBool(testVal), Serial::Error::Success);
    ASSERT_EQ(read.handleBool(testVal), Serial::Error::Success);
    ASSERT_EQ(read.handleBool(testVal), Serial::Error::Success);

    ASSERT_EQ(read.handleBool(testVal), Serial::Error::EndOfStream);
}

TEST(Serial, TestIntBasic)
{
    std::vector<uint8_t> buf(10, 0);
    Serial::WriteBitStream write(&buf[0], buf.size());
    Serial::ReadBitStream read(&buf[0], buf.size());

    constexpr int64_t min = 0;
    constexpr int64_t max = 2047;

    int64_t testVal = 0;
    int64_t readVal = 0;
    Serial::Error::Error success = Serial::Error::Success;

    testVal = 0;
    success = write.handleInt<min, max>(testVal);
    ASSERT_EQ(success, Serial::Error::Success);
    success = read.handleInt<min, max>(readVal);
    ASSERT_EQ(success, Serial::Error::Success);
    ASSERT_EQ(testVal, readVal);

    testVal = 1;
    success = write.handleInt<min, max>(testVal);
    ASSERT_EQ(success, Serial::Error::Success);
    success = read.handleInt<min, max>(readVal);
    ASSERT_EQ(success, Serial::Error::Success);
    ASSERT_EQ(testVal, readVal);

    testVal = 1024;
    success = write.handleInt<min, max>(testVal);
    ASSERT_EQ(success, Serial::Error::Success);
    success = read.handleInt<min, max>(readVal);
    ASSERT_EQ(success, Serial::Error::Success);
    ASSERT_EQ(testVal, readVal);

    testVal = 2047;
    success = write.handleInt<min, max>(testVal);
    ASSERT_EQ(success, Serial::Error::Success);
    success = read.handleInt<min, max>(readVal);
    ASSERT_EQ(success, Serial::Error::Success);
    ASSERT_EQ(testVal, readVal);
}

TEST(Serial, TestIntNegative)
{
    std::vector<uint8_t> buf(10, 0);
    Serial::WriteBitStream write(&buf[0], buf.size());
    Serial::ReadBitStream read(&buf[0], buf.size());

    constexpr int64_t min = -10;
    constexpr int64_t max = 10;

    int64_t testVal = 0;
    int64_t readVal = 0;
    Serial::Error::Error success = Serial::Error::Success;

    testVal = -5;
    success = write.handleInt<min, max>(testVal);
    ASSERT_EQ(success, Serial::Error::Success);
    success = read.handleInt<min, max>(readVal);
    ASSERT_EQ(success, Serial::Error::Success);
    ASSERT_EQ(testVal, readVal);

    testVal = -1;
    success = write.handleInt<min, max>(testVal);
    ASSERT_EQ(success, Serial::Error::Success);
    success = read.handleInt<min, max>(readVal);
    ASSERT_EQ(success, Serial::Error::Success);
    ASSERT_EQ(testVal, readVal);

    testVal = 0;
    success = write.handleInt<min, max>(testVal);
    ASSERT_EQ(success, Serial::Error::Success);
    success = read.handleInt<min, max>(readVal);
    ASSERT_EQ(success, Serial::Error::Success);
    ASSERT_EQ(testVal, readVal);

    testVal = 3;
    success = write.handleInt<min, max>(testVal);
    ASSERT_EQ(success, Serial::Error::Success);
    success = read.handleInt<min, max>(readVal);
    ASSERT_EQ(success, Serial::Error::Success);
    ASSERT_EQ(testVal, readVal);

    testVal = 1;
    success = write.handleInt<min, max>(testVal);
    ASSERT_EQ(success, Serial::Error::Success);
    success = read.handleInt<min, max>(readVal);
    ASSERT_EQ(success, Serial::Error::Success);
    ASSERT_EQ(testVal, readVal);

    testVal = -10;
    success = write.handleInt<min, max>(testVal);
    ASSERT_EQ(success, Serial::Error::Success);
    success = read.handleInt<min, max>(readVal);
    ASSERT_EQ(success, Serial::Error::Success);
    ASSERT_EQ(testVal, readVal);
}


TEST(Serial, TestInt32Range)
{
    std::vector<uint8_t> buf(100, 0);
    Serial::WriteBitStream write(&buf[0], buf.size());
    Serial::ReadBitStream read(&buf[0], buf.size());

    int32_t readVal = 0;
    Serial::Error::Error success = Serial::Error::Success;

    int64_t min = std::numeric_limits<int32_t>::min();
    int64_t max = std::numeric_limits<int32_t>::max();
    int64_t spacer = 100000; // can't run for every possible value, it would take too long

    // use 64-bit int for the counter to avoid overflow issues
    for (int64_t i = min; i < max; i += spacer)
    {
        int32_t val = (int32_t)i;
        success = write.handleInt32(val);
        ASSERT_EQ(success, Serial::Error::Success);
        success = read.handleInt32(readVal);
        ASSERT_EQ(success, Serial::Error::Success);
        ASSERT_EQ(i, readVal);

        write.seek(0, Serial::BSPos::Start);
        read.seek(0, Serial::BSPos::Start);
    }
}

TEST(Serial, TestInt32Basic)
{
    std::vector<uint8_t> buf(100, 0);
    Serial::WriteBitStream write(&buf[0], buf.size());
    Serial::ReadBitStream read(&buf[0], buf.size());

    int32_t testVal = 0;
    int32_t readVal = 0;
    Serial::Error::Error success = Serial::Error::Success;
    
    
    testVal = 78;
    success = write.handleInt32(testVal);
    ASSERT_EQ(success, Serial::Error::Success);
    success = read.handleInt32(readVal);
    ASSERT_EQ(success, Serial::Error::Success);
    ASSERT_EQ(testVal, readVal);

    testVal = 200;
    success = write.handleInt32(testVal);
    ASSERT_EQ(success, Serial::Error::Success);
    success = read.handleInt32(readVal);
    ASSERT_EQ(success, Serial::Error::Success);
    ASSERT_EQ(testVal, readVal);

    testVal = 40065;
    success = write.handleInt32(testVal);
    ASSERT_EQ(success, Serial::Error::Success);
    success = read.handleInt32(readVal);
    ASSERT_EQ(success, Serial::Error::Success);
    ASSERT_EQ(testVal, readVal);

    testVal = 4194304;
    success = write.handleInt32(testVal);
    ASSERT_EQ(success, Serial::Error::Success);
    success = read.handleInt32(readVal);
    ASSERT_EQ(success, Serial::Error::Success);
    ASSERT_EQ(testVal, readVal);
   
    testVal = 2147483647;
    success = write.handleInt32(testVal);
    ASSERT_EQ(success, Serial::Error::Success);
    success = read.handleInt32(readVal);
    ASSERT_EQ(success, Serial::Error::Success);
    ASSERT_EQ(testVal, readVal);
}

TEST(Serial, TestPos)
{
    std::vector<uint8_t> buf(100, 0);
    Serial::WriteBitStream write(&buf[0], buf.size());
    Serial::ReadBitStream read(&buf[0], buf.size());

    FAWorld::Position p(234, 754, 2);
    p.mDist = 50;
    Serial::Error::Error success = write.handleObject(p);
    ASSERT_EQ(success, Serial::Error::Success);

    FAWorld::Position p2;
    read.handleObject(p2);
    ASSERT_EQ(success, Serial::Error::Success);

    ASSERT_EQ(p.mCurrent, p2.mCurrent);
    ASSERT_EQ(p.mDist, p2.mDist);
    ASSERT_EQ(p.mDirection, p2.mDirection);
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    int retval = RUN_ALL_TESTS();
    return retval;
}
