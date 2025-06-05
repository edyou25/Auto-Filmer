#include <gtest/gtest.h>
#include <ros/ros.h>
#include <Eigen/Core>
#include "../src/shot.hpp"

class ShotGeneratorTest : public ::testing::Test {
protected:
    void SetUp() override {
        ros::NodeHandle nh;
        shot_generator = std::make_unique<shot::ShotGenerator>(nh);
    }

    std::unique_ptr<shot::ShotGenerator> shot_generator;
};

TEST_F(ShotGeneratorTest, InitializationTest) {
    ASSERT_NE(shot_generator, nullptr);
    // Test that the ShotGenerator is properly initialized
}

TEST_F(ShotGeneratorTest, ConfigurationTest) {
    // Test ShotConfig structure
    shot::ShotConfig config;
    config.image_p << 320.0, 240.0;
    config.image_v << 0.0, 0.0;
    config.distance = 3.5;
    config.view_angle = 3.14;
    
    EXPECT_DOUBLE_EQ(config.image_p.x(), 320.0);
    EXPECT_DOUBLE_EQ(config.image_p.y(), 240.0);
    EXPECT_DOUBLE_EQ(config.distance, 3.5);
    EXPECT_DOUBLE_EQ(config.view_angle, 3.14);
}

TEST_F(ShotGeneratorTest, StateEnumTest) {
    // Test state enumeration
    shot::state test_state = shot::STATIC;
    EXPECT_EQ(test_state, shot::STATIC);
    
    test_state = shot::TRANSITION;
    EXPECT_EQ(test_state, shot::TRANSITION);
}

TEST_F(ShotGeneratorTest, VectorOperationsTest) {
    // Test Eigen vector operations used in shot configuration
    Eigen::Vector2d vec1(1.0, 2.0);
    Eigen::Vector2d vec2(3.0, 4.0);
    
    Eigen::Vector2d result = vec1 + vec2;
    EXPECT_DOUBLE_EQ(result.x(), 4.0);
    EXPECT_DOUBLE_EQ(result.y(), 6.0);
    
    vec1.setZero();
    EXPECT_DOUBLE_EQ(vec1.x(), 0.0);
    EXPECT_DOUBLE_EQ(vec1.y(), 0.0);
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "test_shot_generator");
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
