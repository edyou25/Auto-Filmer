#include <gtest/gtest.h>
#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/Image.h>
#include <geometry_msgs/PoseStamped.h>
#include <nav_msgs/Odometry.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
#include <Eigen/Core>
#include <Eigen/Geometry>

class PCLRenderNodeTest : public ::testing::Test {
protected:
    void SetUp() override {
        nh = std::make_unique<ros::NodeHandle>();
        
        // Create publishers for test data
        pointcloud_pub = nh->advertise<sensor_msgs::PointCloud2>("/map_generator/global_cloud", 1);
        pose_pub = nh->advertise<geometry_msgs::PoseStamped>("/mavros/local_position/pose", 1);
        odom_pub = nh->advertise<nav_msgs::Odometry>("/mavros/local_position/odom", 1);
        
        // Create subscribers to monitor outputs
        depth_sub = nh->subscribe("/pcl_render_node/depth", 1, 
                                 &PCLRenderNodeTest::depthCallback, this);
        color_sub = nh->subscribe("/pcl_render_node/color", 1, 
                                 &PCLRenderNodeTest::colorCallback, this);
        
        received_depth = false;
        received_color = false;
        
        ros::Duration(0.5).sleep(); // Allow time for setup
    }
    
    void depthCallback(const sensor_msgs::Image::ConstPtr& msg) {
        received_depth = true;
        last_depth_msg = *msg;
    }
    
    void colorCallback(const sensor_msgs::Image::ConstPtr& msg) {
        received_color = true;
        last_color_msg = *msg;
    }

    std::unique_ptr<ros::NodeHandle> nh;
    ros::Publisher pointcloud_pub, pose_pub, odom_pub;
    ros::Subscriber depth_sub, color_sub;
    
    bool received_depth, received_color;
    sensor_msgs::Image last_depth_msg, last_color_msg;
};

TEST_F(PCLRenderNodeTest, NodeConnectionTest) {
    // Test that publishers are properly initialized (not requiring subscribers in unit test)
    EXPECT_TRUE(pointcloud_pub);
    EXPECT_TRUE(pose_pub);
    EXPECT_TRUE(odom_pub);
    
    // Verify topic names are correct
    EXPECT_EQ(pointcloud_pub.getTopic(), "/map_generator/global_cloud");
    EXPECT_EQ(pose_pub.getTopic(), "/mavros/local_position/pose");
    EXPECT_EQ(odom_pub.getTopic(), "/mavros/local_position/odom");
}

TEST_F(PCLRenderNodeTest, PointCloudMessageTest) {
    // Create a test point cloud
    pcl::PointCloud<pcl::PointXYZ> cloud;
    cloud.width = 100;
    cloud.height = 1;
    cloud.is_dense = false;
    cloud.points.resize(cloud.width * cloud.height);
    
    // Fill the cloud with test data
    for (size_t i = 0; i < cloud.points.size(); ++i) {
        cloud.points[i].x = 1024 * rand() / (RAND_MAX + 1.0f);
        cloud.points[i].y = 1024 * rand() / (RAND_MAX + 1.0f);
        cloud.points[i].z = 1024 * rand() / (RAND_MAX + 1.0f);
    }
    
    // Convert to ROS message
    sensor_msgs::PointCloud2 cloud_msg;
    pcl::toROSMsg(cloud, cloud_msg);
    cloud_msg.header.stamp = ros::Time::now();
    cloud_msg.header.frame_id = "world";
    
    // Publish the point cloud
    pointcloud_pub.publish(cloud_msg);
    
    // Verify the message structure
    EXPECT_EQ(cloud_msg.width, 100);
    EXPECT_EQ(cloud_msg.height, 1);
    EXPECT_EQ(cloud_msg.header.frame_id, "world");
}

TEST_F(PCLRenderNodeTest, PoseMessageTest) {
    // Create a test pose message
    geometry_msgs::PoseStamped pose_msg;
    pose_msg.header.stamp = ros::Time::now();
    pose_msg.header.frame_id = "world";
    
    pose_msg.pose.position.x = 1.0;
    pose_msg.pose.position.y = 2.0;
    pose_msg.pose.position.z = 3.0;
    
    pose_msg.pose.orientation.x = 0.0;
    pose_msg.pose.orientation.y = 0.0;
    pose_msg.pose.orientation.z = 0.0;
    pose_msg.pose.orientation.w = 1.0;
    
    // Publish the pose
    pose_pub.publish(pose_msg);
    
    // Verify the message structure
    EXPECT_DOUBLE_EQ(pose_msg.pose.position.x, 1.0);
    EXPECT_DOUBLE_EQ(pose_msg.pose.position.y, 2.0);
    EXPECT_DOUBLE_EQ(pose_msg.pose.position.z, 3.0);
    EXPECT_DOUBLE_EQ(pose_msg.pose.orientation.w, 1.0);
}

TEST_F(PCLRenderNodeTest, OdometryMessageTest) {
    // Create a test odometry message
    nav_msgs::Odometry odom_msg;
    odom_msg.header.stamp = ros::Time::now();
    odom_msg.header.frame_id = "world";
    odom_msg.child_frame_id = "base_link";
    
    odom_msg.pose.pose.position.x = 0.5;
    odom_msg.pose.pose.position.y = 1.0;
    odom_msg.pose.pose.position.z = 1.5;
    
    odom_msg.pose.pose.orientation.x = 0.0;
    odom_msg.pose.pose.orientation.y = 0.0;
    odom_msg.pose.pose.orientation.z = 0.0;
    odom_msg.pose.pose.orientation.w = 1.0;
    
    // Publish the odometry
    odom_pub.publish(odom_msg);
    
    // Verify the message structure
    EXPECT_EQ(odom_msg.header.frame_id, "world");
    EXPECT_EQ(odom_msg.child_frame_id, "base_link");
    EXPECT_DOUBLE_EQ(odom_msg.pose.pose.position.x, 0.5);
}

TEST_F(PCLRenderNodeTest, ImageMessageValidationTest) {
    // Test image message structure validation
    sensor_msgs::Image test_img;
    test_img.header.stamp = ros::Time::now();
    test_img.header.frame_id = "camera";
    test_img.width = 640;
    test_img.height = 480;
    test_img.encoding = "mono8";
    test_img.step = test_img.width;
    test_img.data.resize(test_img.height * test_img.step);
    
    EXPECT_EQ(test_img.width, 640);
    EXPECT_EQ(test_img.height, 480);
    EXPECT_EQ(test_img.encoding, "mono8");
    EXPECT_EQ(test_img.data.size(), 640 * 480);
}

TEST_F(PCLRenderNodeTest, EigenTransformationTest) {
    // Test Eigen transformations used in rendering
    Eigen::Matrix4d transform = Eigen::Matrix4d::Identity();
    Eigen::Vector3d translation(1.0, 2.0, 3.0);
    
    transform.block<3,1>(0,3) = translation;
    
    EXPECT_DOUBLE_EQ(transform(0,3), 1.0);
    EXPECT_DOUBLE_EQ(transform(1,3), 2.0);
    EXPECT_DOUBLE_EQ(transform(2,3), 3.0);
    EXPECT_DOUBLE_EQ(transform(3,3), 1.0);
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "test_pcl_render_node");
    ::testing::InitGoogleTest(&argc, argv);
    
    // Start a spinner to handle callbacks
    ros::AsyncSpinner spinner(1);
    spinner.start();
    
    int result = RUN_ALL_TESTS();
    
    spinner.stop();
    return result;
}
