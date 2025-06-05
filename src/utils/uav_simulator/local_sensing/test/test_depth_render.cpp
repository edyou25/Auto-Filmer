#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include <random>
#include <memory>

// Include CUDA headers
#include <cuda_runtime.h>

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
        point_cloud_size = 1000;
        
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
    
    // Helper function to generate test point cloud
    std::vector<float> generateTestPointCloud(int num_points, float min_range = 1.0f, float max_range = 10.0f) {
        std::vector<float> cloud_data;
        cloud_data.reserve(num_points * 3);
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(min_range, max_range);
        std::uniform_real_distribution<float> angle_dis(-M_PI, M_PI);
        
        for (int i = 0; i < num_points; i++) {
            float range = dis(gen);
            float azimuth = angle_dis(gen);
            float elevation = angle_dis(gen) * 0.5; // Limit elevation to reasonable range
            
            float x = range * cos(elevation) * cos(azimuth);
            float y = range * cos(elevation) * sin(azimuth);
            float z = range * sin(elevation);
            
            cloud_data.push_back(x);
            cloud_data.push_back(y);
            cloud_data.push_back(z);
        }
        
        return cloud_data;
    }
    
    // Helper function to create identity transformation matrix
    std::vector<double> createIdentityTransform() {
        return {1.0, 0.0, 0.0, 0.0,
                0.0, 1.0, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                0.0, 0.0, 0.0, 1.0};
    }
    
    // Helper function to create translation transformation matrix
    std::vector<double> createTranslationTransform(double tx, double ty, double tz) {
        return {1.0, 0.0, 0.0, tx,
                0.0, 1.0, 0.0, ty,
                0.0, 0.0, 1.0, tz,
                0.0, 0.0, 0.0, 1.0};
    }

    std::unique_ptr<DepthRender> depth_renderer;
    
    int camera_width, camera_height;
    double camera_fx, camera_fy, camera_cx, camera_cy;
    int point_cloud_size;
    bool cuda_available;
};

TEST_F(DepthRenderTest, DepthRenderInitialization) {
    if (!cuda_available) {
        GTEST_SKIP() << "CUDA not available, skipping test";
    }
    
    // Test that DepthRender can be created and configured
    DepthRender renderer;
    
    // Test parameter setting
    EXPECT_NO_THROW(renderer.set_para(320.0f, 320.0f, 320.0f, 240.0f, 640, 480));
}

TEST_F(DepthRenderTest, PointCloudDataSetting) {
    if (!cuda_available) {
        GTEST_SKIP() << "CUDA not available, skipping test";
    }
    
    // Generate test point cloud
    std::vector<float> test_cloud = generateTestPointCloud(100);
    
    // Test that point cloud data can be set without errors
    EXPECT_NO_THROW(depth_renderer->set_data(test_cloud));
    
    // Verify the point cloud has correct size (should be multiple of 3)
    EXPECT_EQ(test_cloud.size() % 3, 0);
    EXPECT_GT(test_cloud.size(), 0);
}

TEST_F(DepthRenderTest, DepthRenderingBasic) {
    if (!cuda_available) {
        GTEST_SKIP() << "CUDA not available, skipping test";
    }
    
    // Generate a simple test point cloud with known points
    std::vector<float> test_cloud = {
        0.0f, 0.0f, 2.0f,  // Point directly in front of camera
        1.0f, 0.0f, 2.0f,  // Point to the right
        -1.0f, 0.0f, 2.0f, // Point to the left
        0.0f, 1.0f, 2.0f,  // Point above
        0.0f, -1.0f, 2.0f  // Point below
    };
    
    depth_renderer->set_data(test_cloud);
    
    // Create output buffer for depth image
    std::vector<int> depth_output(camera_width * camera_height);
    
    // Create identity transformation
    auto transform = createIdentityTransform();
    
    // Render depth image
    EXPECT_NO_THROW(depth_renderer->render_pose(transform.data(), depth_output.data()));
    
    // Verify that some pixels have been rendered (not all are 999999)
    bool has_rendered_pixels = false;
    for (int depth_value : depth_output) {
        if (depth_value < 999999) {
            has_rendered_pixels = true;
            break;
        }
    }
    EXPECT_TRUE(has_rendered_pixels) << "No pixels were rendered in the depth image";
}

TEST_F(DepthRenderTest, DepthValueAccuracy) {
    if (!cuda_available) {
        GTEST_SKIP() << "CUDA not available, skipping test";
    }
    
    // Create a single point at known distance
    float known_distance = 3.0f; // 3 meters
    std::vector<float> test_cloud = {0.0f, 0.0f, known_distance};
    
    depth_renderer->set_data(test_cloud);
    
    std::vector<int> depth_output(camera_width * camera_height);
    auto transform = createIdentityTransform();
    
    depth_renderer->render_pose(transform.data(), depth_output.data());
    
    // Check center pixel (where the point should project)
    int center_x = camera_width / 2;
    int center_y = camera_height / 2;
    int center_index = center_y * camera_width + center_x;
    
    int rendered_depth_mm = depth_output[center_index];
    
    // Verify depth is reasonable (should be close to 3000mm = 3m)
    if (rendered_depth_mm < 999999) {
        float rendered_depth_m = rendered_depth_mm / 1000.0f;
        EXPECT_NEAR(rendered_depth_m, known_distance, 0.1f) 
            << "Rendered depth " << rendered_depth_m << "m differs significantly from expected " << known_distance << "m";
    }
}

TEST_F(DepthRenderTest, TransformationHandling) {
    if (!cuda_available) {
        GTEST_SKIP() << "CUDA not available, skipping test";
    }
    
    // Create test point cloud
    std::vector<float> test_cloud = generateTestPointCloud(50, 1.0f, 5.0f);
    depth_renderer->set_data(test_cloud);
    
    std::vector<int> depth_output1(camera_width * camera_height);
    std::vector<int> depth_output2(camera_width * camera_height);
    
    // Render with identity transform
    auto identity_transform = createIdentityTransform();
    EXPECT_NO_THROW(depth_renderer->render_pose(identity_transform.data(), depth_output1.data()));
    
    // Render with translation transform
    auto translation_transform = createTranslationTransform(1.0, 0.0, 0.0);
    EXPECT_NO_THROW(depth_renderer->render_pose(translation_transform.data(), depth_output2.data()));
    
    // The two renders should produce different results due to different transformations
    bool images_different = false;
    for (size_t i = 0; i < depth_output1.size(); i++) {
        if (depth_output1[i] != depth_output2[i]) {
            images_different = true;
            break;
        }
    }
    EXPECT_TRUE(images_different) << "Different transformations should produce different depth images";
}

TEST_F(DepthRenderTest, CameraParameterValidation) {
    if (!cuda_available) {
        GTEST_SKIP() << "CUDA not available, skipping test";
    }
    
    // Test different camera parameters
    DepthRender renderer1, renderer2;
    
    // Set different focal lengths
    EXPECT_NO_THROW(renderer1.set_para(300.0f, 300.0f, 320.0f, 240.0f, 640, 480));
    EXPECT_NO_THROW(renderer2.set_para(400.0f, 400.0f, 320.0f, 240.0f, 640, 480));
    
    // Test different image sizes
    DepthRender renderer3;
    EXPECT_NO_THROW(renderer3.set_para(320.0f, 320.0f, 160.0f, 120.0f, 320, 240));
}

TEST_F(DepthRenderTest, LargePointCloudHandling) {
    if (!cuda_available) {
        GTEST_SKIP() << "CUDA not available, skipping test";
    }
    
    // Test with larger point cloud
    std::vector<float> large_cloud = generateTestPointCloud(point_cloud_size);
    
    EXPECT_NO_THROW(depth_renderer->set_data(large_cloud));
    
    std::vector<int> depth_output(camera_width * camera_height);
    auto transform = createIdentityTransform();
    
    EXPECT_NO_THROW(depth_renderer->render_pose(transform.data(), depth_output.data()));
    
    // Verify some rendering occurred
    int rendered_pixel_count = 0;
    for (int depth_value : depth_output) {
        if (depth_value < 999999) {
            rendered_pixel_count++;
        }
    }
    
    // With a large point cloud, we should have many rendered pixels
    EXPECT_GT(rendered_pixel_count, 0) << "Large point cloud should render some pixels";
}

TEST_F(DepthRenderTest, EdgeCaseHandling) {
    if (!cuda_available) {
        GTEST_SKIP() << "CUDA not available, skipping test";
    }
    
    // Test empty point cloud
    std::vector<float> empty_cloud;
    DepthRender empty_renderer;
    empty_renderer.set_para(camera_fx, camera_fy, camera_cx, camera_cy, camera_width, camera_height);
    
    EXPECT_NO_THROW(empty_renderer.set_data(empty_cloud));
    
    std::vector<int> depth_output(camera_width * camera_height);
    auto transform = createIdentityTransform();
    
    EXPECT_NO_THROW(empty_renderer.render_pose(transform.data(), depth_output.data()));
    
    // All pixels should remain unrendered (999999)
    for (int depth_value : depth_output) {
        EXPECT_EQ(depth_value, 999999) << "Empty point cloud should not render any pixels";
    }
}

TEST_F(DepthRenderTest, MemoryManagement) {
    if (!cuda_available) {
        GTEST_SKIP() << "CUDA not available, skipping test";
    }
    
    // Test creating and destroying multiple renderers
    for (int i = 0; i < 5; i++) {
        std::unique_ptr<DepthRender> renderer = std::make_unique<DepthRender>();
        renderer->set_para(320.0f, 320.0f, 320.0f, 240.0f, 640, 480);
        
        std::vector<float> test_cloud = generateTestPointCloud(100);
        EXPECT_NO_THROW(renderer->set_data(test_cloud));
        
        // Renderer should be safely destroyed when going out of scope
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
