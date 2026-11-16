#include <unity.h>
#include "Obd2Parser.h"

void setUp(void) {
    // Set up routine if needed
}

void tearDown(void) {
    // Clean up routine if needed
}

void test_obd2_query_creation(void) {
    twai_message_t query = Obd2Parser::createQuery(0x0C);
    TEST_ASSERT_EQUAL_HEX32(0x7DF, query.identifier);
    TEST_ASSERT_EQUAL(0, query.extd);
    TEST_ASSERT_EQUAL(8, query.data_length_code);
    TEST_ASSERT_EQUAL(0x02, query.data[0]);
    TEST_ASSERT_EQUAL(0x01, query.data[1]);
    TEST_ASSERT_EQUAL(0x0C, query.data[2]);
}

void test_obd2_parse_rpm(void) {
    twai_message_t msg;
    msg.identifier = 0x7E8;
    msg.data_length_code = 8;
    msg.data[0] = 0x03;
    msg.data[1] = 0x41;
    msg.data[2] = 0x0C; // RPM PID
    msg.data[3] = 0x1A; // A = 26
    msg.data[4] = 0xF0; // B = 240 -> (26*256 + 240) / 4 = (6656 + 240)/4 = 6896 / 4 = 1724 RPM
    
    OBD2Data data;
    bool success = Obd2Parser::parseResponse(msg, data);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_TRUE(data.rpmValid);
    TEST_ASSERT_EQUAL(1724, data.rpm);
}

void test_obd2_parse_speed(void) {
    twai_message_t msg;
    msg.identifier = 0x7E8;
    msg.data_length_code = 8;
    msg.data[0] = 0x03;
    msg.data[1] = 0x41;
    msg.data[2] = 0x0D; // Speed PID
    msg.data[3] = 0x32; // 50 km/h
    
    OBD2Data data;
    bool success = Obd2Parser::parseResponse(msg, data);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_TRUE(data.speedValid);
    TEST_ASSERT_EQUAL(50, data.speed);
}

void test_obd2_parse_coolant_temp(void) {
    twai_message_t msg;
    msg.identifier = 0x7E8;
    msg.data_length_code = 8;
    msg.data[0] = 0x03;
    msg.data[1] = 0x41;
    msg.data[2] = 0x05; // Coolant Temp PID
    msg.data[3] = 0x7B; // 123 -> 123 - 40 = 83 °C
    
    OBD2Data data;
    bool success = Obd2Parser::parseResponse(msg, data);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_TRUE(data.coolTempValid);
    TEST_ASSERT_EQUAL(83, data.coolTemp);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_obd2_query_creation);
    RUN_TEST(test_obd2_parse_rpm);
    RUN_TEST(test_obd2_parse_speed);
    RUN_TEST(test_obd2_parse_coolant_temp);
    return UNITY_END();
}
