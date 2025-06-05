#!/usr/bin/env python3
"""
Map generator script to create environment point cloud data
Based on map_generator_easy.py
"""

import math
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle, Circle

class MapGenerator:
    def __init__(self):
        self.points = []
        self.resolution = 0.05
        self.dimx = 10.0
        self.dimy = 10.0
        self.dimz = 3.0
        self.floor_bias = 0.50
        # Store objects for visualization
        self.boxes = []
        self.cylinders = []

    def add_box(self, size, position):
        """Add a box to the environment
        size: [x,y,z]
        position: [x,y,z] - center position
        """
        # Store box info for visualization
        self.boxes.append({'size': size.copy(), 'position': position.copy()})
        
        position[2] -= self.floor_bias
        x_low = math.floor((position[0] - size[0] / 2) / self.resolution) * self.resolution + 0.5 * self.resolution
        x_high = math.floor((position[0] + size[0] / 2) / self.resolution) * self.resolution + 0.5 * self.resolution
        y_low = math.floor((position[1] - size[1] / 2) / self.resolution) * self.resolution + 0.5 * self.resolution
        y_high = math.floor((position[1] + size[1] / 2) / self.resolution) * self.resolution + 0.5 * self.resolution
        z_low = math.floor((position[2] - size[2] / 2) / self.resolution) * self.resolution + 0.5 * self.resolution
        z_high = math.floor((position[2] + size[2] / 2) / self.resolution) * self.resolution + 0.5 * self.resolution

        x = x_low
        while x <= x_high:
            y = y_low
            while y <= y_high:
                z = z_low
                while z <= z_high:
                    if (math.fabs(x - x_low) < self.resolution) or (math.fabs(x - x_high) < self.resolution) \
                        or (math.fabs(y - y_low) < self.resolution) or (math.fabs(y - y_high) < self.resolution) \
                        or (math.fabs(z - z_low) < self.resolution) or (math.fabs(z - z_high) < self.resolution):
                        self.points.append([x, y, z])
                    z += self.resolution
                y += self.resolution
            x += self.resolution

    def add_cylinder(self, size, position):
        """Add a cylinder to the environment
        size: [r, h]
        position: [x,y,z] - center position
        """
        # Store cylinder info for visualization
        self.cylinders.append({'size': size.copy(), 'position': position.copy()})
        
        position[2] -= self.floor_bias
        center_x = position[0]
        center_y = position[1]
        z_low = math.floor((position[2] - size[1] / 2) / self.resolution) * self.resolution + 0.5 * self.resolution
        z_high = math.floor((position[2] + size[1] / 2) / self.resolution) * self.resolution + 0.5 * self.resolution

        radius_num = math.floor(size[0] / self.resolution)
        x = -radius_num
        while x <= radius_num:
            y = -radius_num
            while y <= radius_num:
                radius2 = x ** 2 + y ** 2
                if radius2 < (radius_num + 0.5) ** 2:
                    z = z_low
                    while z <= z_high:
                        if radius2 > (radius_num - 0.5) ** 2 or \
                            (math.fabs(z - z_low) < self.resolution) or (math.fabs(z - z_high) < self.resolution):
                            self.points.append([center_x + x * self.resolution, center_y + y * self.resolution, z])
                        z += self.resolution
                y += 1
            x += 1

    def visualize_top_view(self, save_path=None, camera_positions=None):
        """Visualize environment from top view with optional camera positions
        
        Args:
            save_path: Path to save the visualization image
            camera_positions: List of camera positions [[x, y, z, yaw, name], ...] or simple [x, y, z] format
        """
        fig, ax = plt.subplots(1, 1, figsize=(14, 12))
        
        # Plot boxes
        for box in self.boxes:
            pos = box['position']
            size = box['size']
            # Rectangle bottom-left corner
            rect_x = pos[0] - size[0] / 2
            rect_y = pos[1] - size[1] / 2
            
            rect = Rectangle((rect_x, rect_y), size[0], size[1], 
                           linewidth=2, edgecolor='blue', facecolor='lightblue', alpha=0.7)
            ax.add_patch(rect)
            
            # Add size label
            ax.text(pos[0], pos[1], f'{size[0]:.1f}×{size[1]:.1f}×{size[2]:.1f}', 
                   ha='center', va='center', fontsize=12, fontweight='bold')
        
        # Plot cylinders
        for cyl in self.cylinders:
            pos = cyl['position']
            size = cyl['size']  # [radius, height]
            
            circle = Circle((pos[0], pos[1]), size[0], 
                          linewidth=2, edgecolor='red', facecolor='lightcoral', alpha=0.7)
            ax.add_patch(circle)
            
            # Add size label
            ax.text(pos[0], pos[1], f'R{size[0]:.1f}\nH{size[1]:.1f}', 
                   ha='center', va='center', fontsize=12, fontweight='bold')
        
        # Plot point cloud (sample points for visualization)
        if self.points:
            points_array = np.array(self.points)
            ax.scatter(points_array[:, 0], points_array[:, 1], 
                      c='green', s=0.5, alpha=0.3, label=f'Points ({len(self.points)})')
        
        # Plot camera positions with orientation
        if camera_positions is not None:
            # Handle both simple [x,y,z] and full [x,y,z,yaw,name] formats
            if isinstance(camera_positions[0], (int, float)):
                # Single position
                camera_positions = [camera_positions]
            
            for i, cam_data in enumerate(camera_positions):
                if len(cam_data) >= 5:  # Full format [x, y, z, yaw, name]
                    x, y, z, yaw, name = cam_data[:5]
                elif len(cam_data) >= 4:  # [x, y, z, yaw] format
                    x, y, z, yaw = cam_data[:4]
                    name = f"Cam{i+1}"
                else:  # Simple [x, y, z] format
                    x, y, z = cam_data[:3]
                    yaw = 0.0
                    name = f"Cam{i+1}"
                
                # Plot camera position as a circle
                ax.scatter(x, y, c='orange', s=120, marker='o', 
                          edgecolors='darkorange', linewidth=2, 
                          label='Camera' if i == 0 else '', zorder=5)
                
                # Plot camera orientation as arrow
                arrow_length = 1.0
                dx = arrow_length * np.cos(yaw)
                dy = arrow_length * np.sin(yaw)
                
                ax.arrow(x, y, dx, dy, head_width=0.2, head_length=0.2, 
                        fc='red', ec='darkred', linewidth=2, zorder=6)
                
                # Add camera label with height and orientation info
                label_text = f"{name} Z:{z:.1f} Yaw:{np.degrees(yaw):.0f}°"
                ax.text(x + 0.4, y + 0.4, label_text, 
                       fontsize=12, fontweight='bold', color='darkorange',
                       bbox=dict(boxstyle='round,pad=0.3', facecolor='white', alpha=0.9))
        
        # Set axis properties
        ax.set_xlim(-15, 15)
        ax.set_ylim(-15, 20)
        ax.set_aspect('equal')
        ax.grid(True, alpha=0.3)
        ax.set_xlabel('X (meters)', fontsize=22)
        ax.set_ylabel('Y (meters)', fontsize=22)
        ax.set_title('Environment Top View\n(Blue: Boxes, Red: Cylinders, Green: Point Cloud, Orange: Cameras with Red Arrows)', 
                    fontsize=24, fontweight='bold')
        
        # Add legend
        from matplotlib.patches import Patch
        legend_elements = [
            Patch(facecolor='lightblue', edgecolor='blue', label='Boxes'),
            Patch(facecolor='lightcoral', edgecolor='red', label='Cylinders'),
            plt.Line2D([0], [0], marker='o', color='w', markerfacecolor='green', 
                      markersize=5, label='Point Cloud', alpha=0.7)
        ]
        
        # Add camera to legend if cameras are present
        if camera_positions is not None:
            legend_elements.extend([
                plt.Line2D([0], [0], marker='o', color='w', markerfacecolor='orange',
                          markeredgecolor='darkorange', markersize=8, label='Camera Position'),
                plt.Line2D([0], [0], color='red', linewidth=2, label='Camera Direction')
            ])
        
        ax.legend(handles=legend_elements, loc='upper right')
        
        plt.tight_layout()
        
        if save_path:
            plt.savefig(save_path, dpi=300, bbox_inches='tight')
            print(f"Top view saved to {save_path}")
        
        plt.show()
        return fig, ax

    def add_test_cameras(self):
        """Add test camera positions from C++ unit test"""
        # Camera poses from C++ test: {x, y, z, yaw, name}
        test_cameras = [
            # [0.0, 0.0, 1.5, 0.0, "center_view"],           # Center of room
            # [-3.0, 0.0, 1.5, 0.0, "left_view"],            # Left side
            # [3.0, 0.0, 1.5, np.pi, "right_view"],          # Right side
            # [0.0, -3.0, 1.5, np.pi/2, "front_view"],       # Front
            # [0.0, 3.0, 1.5, -np.pi/2, "back_view"],        # Back
            [0.0, 0.0, 4.5, 0.0, "high_view"],             # Higher viewpoint
            # [-2.0, -2.0, 1.0, np.pi/4, "corner_view"]      # Corner view
        ]
        return test_cameras

    def make_map(self):
        """Generate a simple environment with multiple objects"""
        print("Generating environment map...")
        
        # Add various objects to create an interesting environment
        self.add_box([2.0, 4.0, 3.0], [-2.0, -5.5, 1.5])
        self.add_box([1.5, 4.0, 3.0], [-3.0, 4.0, 1.5])
        self.add_box([1.5, 1.0, 3.0], [10.0, -2.0, 1.5])
        self.add_box([1.0, 1.5, 3.0], [5.0, -8.0, 1.5])
        self.add_box([2.0, 3.0, 3.0], [-10.0, 0.0, 1.5])
        self.add_box([5.0, 2.0, 3.0], [5.0, 14.0, 1.5])
        self.add_box([2.0, 5.0, 3.0], [-4.0, 13.0, 1.5])
        self.add_box([2.0, 4.0, 3.0], [3.0, 0.0, 1.5])
        self.add_box([2.0, 6.0, 3.0], [-10.0, 15.0, 1.5])
        self.add_box([4.0, 2.0, 3.0], [6.0, 6.0, 1.5])
        self.add_box([3.0, 2.0, 3.0], [-8.0, -10.0, 1.5])
        
        # Add some cylinders for variety
        self.add_cylinder([0.5, 2.0], [0.0, 0.0, 1.0])
        self.add_cylinder([0.3, 1.5], [2.0, 3.0, 0.75])
        self.add_cylinder([0.4, 2.5], [-1.0, -2.0, 1.25])
        
        print(f"Generated {len(self.points)} points")
        return self.points

    def save_to_file(self, filename):
        """Save point cloud to a text file"""
        with open(filename, 'w') as f:
            f.write(f"{len(self.points)}\n")  # Number of points
            for point in self.points:
                f.write(f"{point[0]:.6f} {point[1]:.6f} {point[2]:.6f}\n")
        print(f"Saved {len(self.points)} points to {filename}")

    def get_points_as_vector(self):
        """Get points as a flat vector [x1,y1,z1,x2,y2,z2,...]"""
        flat_points = []
        for point in self.points:
            flat_points.extend(point)
        return flat_points

if __name__ == "__main__":
    generator = MapGenerator()
    generator.make_map()
    generator.save_to_file("/root/Auto-Filmer/src/utils/uav_simulator/local_sensing/test/environment_points.txt")
    
    # Add test camera positions
    test_cameras = generator.add_test_cameras()
    
    # Generate and display top view visualization with cameras
    generator.visualize_top_view(
        "/root/Auto-Filmer/src/utils/uav_simulator/local_sensing/test/environment_top_view.png",
        camera_positions=test_cameras
    )