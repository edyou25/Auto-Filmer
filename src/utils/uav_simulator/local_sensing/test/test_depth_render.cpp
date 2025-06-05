#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include <random>
#include <memory>
#include <fstream>
#include <iostream>
#include <sstream>

// Include CUDA headers
#include <cuda_runtime.h>

// Include OpenCV for visualization
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

// Include the depth render functionality
#include "../src/depth_render.cuh"

class DepthRenderTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Test parameters
        camera_width = 640;
        camera_height = 480;
        camera_fx = 320.0;
        camera_fy = 320.0;
        camera_cx = 320.0;
        camera_cy = 240.0;
        
        // Initialize CUDA device (if available)
        int device_count;
        cudaError_t cuda_error = cudaGetDeviceCount(&device_count);
        cuda_available = (cuda_error == cudaSuccess && device_count > 0);
        
        if (cuda_available) {
            cudaSetDevice(0);
            std::cout << "CUDA device available for testing" << std::endl;
        } else {
            std::cout << "CUDA device not available, some tests will be skipped" << std::endl;
        }
        
        // Create test depth renderer
        if (cuda_available) {
            depth_renderer = std::make_unique<DepthRender>();
            depth_renderer->set_para(camera_fx, camera_fy, camera_cx, camera_cy, camera_width, camera_height);
        }
    }
    
    void TearDown() override {
        if (cuda_available) {
            cudaDeviceReset();
        }
    }
    
    // Load environment point cloud from generated file
    std::vector<float> loadEnvironmentPointCloud() {
        std::vector<float> cloud_data;
        
        // First try to generate the map data
        int result = system("cd /root/Auto-Filmer/src/utils/uav_simulator/local_sensing/test && python3 generate_map_data.py");
        if (result != 0) {
            std::cout << "Warning: Failed to generate map data, using fallback data" << std::endl;
            return generateFallbackPointCloud();
        }
        
        // Load the generated point cloud file
        std::ifstream file("/root/Auto-Filmer/src/utils/uav_simulator/local_sensing/test/environment_points.txt");
        if (!file.is_open()) {
            std::cout << "Warning: Could not open environment_points.txt, using fallback data" << std::endl;
            return generateFallbackPointCloud();
        }
        
        int num_points;
        file >> num_points;
        
        cloud_data.reserve(num_points * 3);
        
        for (int i = 0; i < num_points; i++) {
            float x, y, z;
            if (file >> x >> y >> z) {
                cloud_data.push_back(x);
                cloud_data.push_back(y);
                cloud_data.push_back(z);
            } else {
                std::cout << "Warning: Error reading point " << i << ", stopping" << std::endl;
                break;
            }
        }
        
        file.close();
        std::cout << "Loaded " << cloud_data.size()/3 << " points from environment file" << std::endl;
        return cloud_data;
    }
    
    // Fallback point cloud in case file loading fails
    std::vector<float> generateFallbackPointCloud() {
        std::vector<float> cloud_data;
        
        // Create a simple room-like environment
        // Floor
        for (float x = -5.0f; x <= 5.0f; x += 0.1f) {
            for (float y = -5.0f; y <= 5.0f; y += 0.1f) {
                cloud_data.push_back(x);
                cloud_data.push_back(y);
                cloud_data.push_back(0.0f);
            }
        }
        
        // Walls
        for (float z = 0.0f; z <= 3.0f; z += 0.1f) {
            for (float x = -5.0f; x <= 5.0f; x += 0.1f) {
                // Front and back walls
                cloud_data.push_back(x); cloud_data.push_back(-5.0f); cloud_data.push_back(z);
                cloud_data.push_back(x); cloud_data.push_back(5.0f); cloud_data.push_back(z);
            }
            for (float y = -5.0f; y <= 5.0f; y += 0.1f) {
                // Left and right walls
                cloud_data.push_back(-5.0f); cloud_data.push_back(y); cloud_data.push_back(z);
                cloud_data.push_back(5.0f); cloud_data.push_back(y); cloud_data.push_back(z);
            }
        }
        
        // Add some objects
        // Box 1
        for (float x = 1.0f; x <= 2.0f; x += 0.1f) {
            for (float y = 1.0f; y <= 2.0f; y += 0.1f) {
                for (float z = 0.0f; z <= 1.0f; z += 0.1f) {
                    cloud_data.push_back(x); cloud_data.push_back(y); cloud_data.push_back(z);
                }
            }
        }
        
        std::cout << "Generated fallback point cloud with " << cloud_data.size()/3 << " points" << std::endl;
        return cloud_data;
    }
    
    // Helper function to create identity transformation matrix
    std::vector<double> createIdentityTransform() {
        return {1.0, 0.0, 0.0, 0.0,
                0.0, 1.0, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                0.0, 0.0, 0.0, 1.0};
    }
    
    // Helper function to create camera transformation matrix
    std::vector<double> createCameraTransform(double x, double y, double z, double yaw = 0.0) {
        double cos_yaw = cos(yaw);
        double sin_yaw = sin(yaw);
        
        return {cos_yaw, -sin_yaw, 0.0, x,
                sin_yaw, cos_yaw, 0.0, y,
                0.0, 0.0, 1.0, z,
                0.0, 0.0, 0.0, 1.0};
    }
    
    // Visualize depth image using OpenCV
    void visualizeDepthImage(const std::vector<int>& depth_data, const std::string& window_name = "Depth Image") {
        cv::Mat depth_mat(camera_height, camera_width, CV_32FC1);
        
        // Convert depth data to OpenCV format and scale
        float min_depth = 0.5f;
        float max_depth = 1.0f;
        
        for (int i = 0; i < camera_height; i++) {
            for (int j = 0; j < camera_width; j++) {
                float depth = static_cast<float>(depth_data[i * camera_width + j]) / 1000.0f;
                depth = (depth < 500.0f) ? depth : 0.0f;
                max_depth = std::max(depth, max_depth);
                depth_mat.at<float>(i, j) = depth;
            }
        }
        
        // Convert to 8-bit for visualization
        cv::Mat depth_8bit;
        depth_mat.convertTo(depth_8bit, CV_8UC3, 255.0 / 13.0, -min_depth);
        
        // Apply color map for better visualization
        cv::Mat colored_depth;
        cv::applyColorMap(depth_8bit, colored_depth, cv::COLOR_GRAY2BGR);
        
        // Display the image
        cv::imshow(window_name, colored_depth);
        cv::waitKey(0);        
        // Save the image
        std::string filename = "/root/Auto-Filmer/src/utils/uav_simulator/local_sensing/test/" + window_name + ".png";
        cv::imwrite(filename, colored_depth);
        std::cout << "Saved depth visualization to: " << filename << std::endl;
    }

    std::unique_ptr<DepthRender> depth_renderer;
    
    int camera_width, camera_height;
    double camera_fx, camera_fy, camera_cx, camera_cy;
    bool cuda_available;
};

TEST_F(DepthRenderTest, EnvironmentDepthRenderingWithVisualization) {
    if (!cuda_available) {
        GTEST_SKIP() << "CUDA not available, skipping test";
    }
    
    std::cout << "=== Environment Depth Rendering Test ===" << std::endl;
    
    // Load environment point cloud
    std::vector<float> environment_cloud = loadEnvironmentPointCloud();
    ASSERT_GT(environment_cloud.size(), 0) << "Failed to load environment point cloud";
    ASSERT_EQ(environment_cloud.size() % 3, 0) << "Point cloud data should be multiple of 3";
    
    std::cout << "Loaded point cloud with " << environment_cloud.size()/3 << " points" << std::endl;
    
    // Set the point cloud data
    EXPECT_NO_THROW(depth_renderer->set_data(environment_cloud));
    
    // Create output buffer for depth image
    std::vector<int> depth_output(camera_width * camera_height);
    
    // Test different camera positions and orientations
    std::vector<std::tuple<double, double, double, double, std::string>> camera_poses = {
        {0.0, 0.0, 1.5, 0.0, "center_view"},           // Center of room
        {-3.0, 0.0, 1.5, 0.0, "left_view"},            // Left side
        {3.0, 0.0, 1.5, M_PI, "right_view"},           // Right side
        {0.0, -3.0, 1.5, M_PI/2, "front_view"},        // Front
        {0.0, 3.0, 1.5, -M_PI/2, "back_view"},         // Back
        {0.0, 0.0, 2.5, 0.0, "high_view"},             // Higher viewpoint
        {-2.0, -2.0, 1.0, M_PI/4, "corner_view"}       // Corner view
    };
    camera_poses = {
        {0.0, 0.0, 4.5, 0.0, "high_view"}
    };
    bool any_successful_render = false;
    
    for (const auto& pose : camera_poses) {
        double x, y, z, yaw;
        std::string view_name;
        std::tie(x, y, z, yaw, view_name) = pose;
        
        std::cout << "\nTesting camera pose: " << view_name 
                  << " at (" << x << ", " << y << ", " << z << ") with yaw " << yaw << std::endl;
        
        // Create camera transformation
        auto transform = createCameraTransform(x, y, z, yaw);
        
        // Clear the output buffer
        std::fill(depth_output.begin(), depth_output.end(), 999999);
        
        // Render depth image
        EXPECT_NO_THROW(depth_renderer->render_pose(transform.data(), depth_output.data()));
        
        // Check if any pixels were rendered
        int rendered_pixel_count = 0;
        int min_depth = INT_MAX;
        int max_depth = 0;
        
        for (int depth_value : depth_output) {
            if (depth_value < 999999) {
                rendered_pixel_count++;
                min_depth = std::min(min_depth, depth_value);
                max_depth = std::max(max_depth, depth_value);
            }
        }
        
        std::cout << "Rendered pixels: " << rendered_pixel_count << " / " << depth_output.size() 
                  << " (" << (100.0 * rendered_pixel_count / depth_output.size()) << "%)" << std::endl;
        
        if (rendered_pixel_count > 0) {
            any_successful_render = true;
            std::cout << "Depth range: " << min_depth/1000.0f << "m to " << max_depth/1000.0f << "m" << std::endl;
            
            // Visualize the depth image
            visualizeDepthImage(depth_output, view_name + "_depth");
        } else {
            std::cout << "Warning: No pixels rendered for " << view_name << std::endl;
        }
    }
    
    // At least one view should have rendered something
    EXPECT_TRUE(any_successful_render) << "No successful renders in any camera position";
    
    std::cout << "\n=== Test completed successfully ===" << std::endl;
    std::cout << "Depth visualizations saved to test directory" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
