import tkinter as tk
from tkinter import ttk, filedialog, messagebox
import re
from datetime import datetime
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib.dates as mdates
from collections import defaultdict
import pandas as pd

class FSMVisualizer:
    def __init__(self, root):
        self.root = root
        self.root.title("FSM State Transition Visualizer")
        self.root.geometry("1200x800")
        
        # 数据存储
        self.transitions = []
        self.states = set()
        self.current_index = 0
        
        # 创建GUI
        self.create_widgets()
        
    def create_widgets(self):
        # 主框架
        main_frame = ttk.Frame(self.root)
        main_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)
        
        # 控制面板
        control_frame = ttk.Frame(main_frame)
        control_frame.pack(fill=tk.X, pady=(0, 10))
        
        # 文件选择
        ttk.Button(control_frame, text="Load Log File", 
                  command=self.load_file).pack(side=tk.LEFT, padx=(0, 10))
        
        # 状态显示
        self.status_label = ttk.Label(control_frame, text="No file loaded")
        self.status_label.pack(side=tk.LEFT, padx=(0, 20))
        
        # 播放控制
        play_frame = ttk.Frame(control_frame)
        play_frame.pack(side=tk.RIGHT)
        
        self.play_button = ttk.Button(play_frame, text="▶ Play", 
                                     command=self.toggle_play, state='disabled')
        self.play_button.pack(side=tk.LEFT, padx=(0, 5))
        
        self.reset_button = ttk.Button(play_frame, text="⟲ Reset", 
                                      command=self.reset_animation, state='disabled')
        self.reset_button.pack(side=tk.LEFT)
        
        # 进度条框架
        progress_frame = ttk.Frame(main_frame)
        progress_frame.pack(fill=tk.X, pady=(0, 10))
        
        ttk.Label(progress_frame, text="Progress:").pack(side=tk.LEFT)
        
        self.progress_var = tk.DoubleVar()
        self.progress_scale = ttk.Scale(progress_frame, from_=0, to=100, 
                                       orient=tk.HORIZONTAL, variable=self.progress_var,
                                       command=self.on_progress_change)
        self.progress_scale.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=(10, 10))
        
        self.progress_label = ttk.Label(progress_frame, text="0/0")
        self.progress_label.pack(side=tk.RIGHT)
        
        # 信息显示框架
        info_frame = ttk.Frame(main_frame)
        info_frame.pack(fill=tk.X, pady=(0, 10))
        
        # 当前状态信息
        current_info = ttk.LabelFrame(info_frame, text="Current Transition")
        current_info.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=(0, 5))
        
        self.current_text = tk.Text(current_info, height=4, wrap=tk.WORD)
        self.current_text.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        
        # 统计信息
        stats_info = ttk.LabelFrame(info_frame, text="Statistics")
        stats_info.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True, padx=(5, 0))
        
        self.stats_text = tk.Text(stats_info, height=4, wrap=tk.WORD)
        self.stats_text.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        
        # 可视化框架
        viz_frame = ttk.Frame(main_frame)
        viz_frame.pack(fill=tk.BOTH, expand=True)
        
        # 创建图表
        self.fig, (self.ax1, self.ax2) = plt.subplots(2, 1, figsize=(12, 8))
        self.canvas = FigureCanvasTkAgg(self.fig, viz_frame)
        self.canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)
        
        # 动画控制
        self.is_playing = False
        self.animation_id = None
        
    def load_file(self):
        file_path = filedialog.askopenfilename(
            title="Select Log File",
            filetypes=[("Log files", "*.log"), ("All files", "*.*")]
        )
        
        if file_path:
            try:
                self.parse_log_file(file_path)
                self.status_label.config(text=f"Loaded: {len(self.transitions)} transitions")
                self.play_button.config(state='normal')
                self.reset_button.config(state='normal')
                self.update_display()
            except Exception as e:
                messagebox.showerror("Error", f"Failed to load file: {str(e)}")
    
    def parse_log_file(self, file_path):
        self.transitions = []
        self.states = set()
        
        fsm_pattern = r'\[(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\.\d{3})\].*\[FSM\].*from (\w+) to (\w+)'
        
        with open(file_path, 'r') as file:
            for line in file:
                match = re.search(fsm_pattern, line)
                if match:
                    timestamp_str = match.group(1)
                    from_state = match.group(2)
                    to_state = match.group(3)
                    
                    timestamp = datetime.strptime(timestamp_str, '%Y-%m-%d %H:%M:%S.%f')
                    
                    self.transitions.append({
                        'timestamp': timestamp,
                        'from_state': from_state,
                        'to_state': to_state,
                        'duration': 0
                    })
                    
                    self.states.add(from_state)
                    self.states.add(to_state)
        
        # 计算每个状态的持续时间
        for i in range(len(self.transitions) - 1):
            duration = (self.transitions[i+1]['timestamp'] - 
                       self.transitions[i]['timestamp']).total_seconds()
            self.transitions[i]['duration'] = duration
        
        if self.transitions:
            self.transitions[-1]['duration'] = 1.0  # 最后一个状态默认1秒
        
        self.current_index = 0
        self.progress_var.set(0)
        
    def update_display(self):
        if not self.transitions:
            return
            
        # 更新进度条
        max_index = len(self.transitions) - 1
        if max_index > 0:
            progress = (self.current_index / max_index) * 100
            self.progress_var.set(progress)
        
        self.progress_label.config(text=f"{self.current_index + 1}/{len(self.transitions)}")
        
        # 更新当前转换信息
        current = self.transitions[self.current_index]
        current_info = f"""Time: {current['timestamp'].strftime('%H:%M:%S.%f')[:-3]}
From: {current['from_state']}
To: {current['to_state']}
Duration: {current['duration']:.3f}s"""
        
        self.current_text.delete(1.0, tk.END)
        self.current_text.insert(1.0, current_info)
        
        # 更新统计信息
        self.update_statistics()
        
        # 更新图表
        self.update_plots()
        
    def update_statistics(self):
        if not self.transitions:
            return
            
        # 统计状态转换次数
        transition_counts = defaultdict(int)
        state_durations = defaultdict(list)
        
        for i in range(self.current_index + 1):
            trans = self.transitions[i]
            key = f"{trans['from_state']} → {trans['to_state']}"
            transition_counts[key] += 1
            state_durations[trans['from_state']].append(trans['duration'])
        
        # 显示最频繁的转换
        sorted_transitions = sorted(transition_counts.items(), 
                                  key=lambda x: x[1], reverse=True)
        
        stats_text = "Most frequent transitions:\n"
        for trans, count in sorted_transitions[:5]:
            stats_text += f"{trans}: {count}\n"
        
        # 显示平均状态持续时间
        stats_text += "\nAverage durations:\n"
        for state, durations in list(state_durations.items())[:3]:
            avg_duration = sum(durations) / len(durations)
            stats_text += f"{state}: {avg_duration:.3f}s\n"
        
        self.stats_text.delete(1.0, tk.END)
        self.stats_text.insert(1.0, stats_text)
    
    def update_plots(self):
        if not self.transitions:
            return
            
        self.ax1.clear()
        self.ax2.clear()
        
        # 获取当前显示的数据
        current_transitions = self.transitions[:self.current_index + 1]
        
        if not current_transitions:
            self.canvas.draw()
            return
        
        # 时间线图
        timestamps = [t['timestamp'] for t in current_transitions]
        states = [t['to_state'] for t in current_transitions]
        
        # 为每个状态分配颜色和y位置
        unique_states = list(self.states)
        state_colors = plt.cm.Set3(range(len(unique_states)))
        state_positions = {state: i for i, state in enumerate(unique_states)}
        
        y_positions = [state_positions[state] for state in states]
        colors = [state_colors[state_positions[state]] for state in states]
        
        # 绘制状态转换时间线
        self.ax1.scatter(timestamps, y_positions, c=colors, s=50, alpha=0.7)
        
        # 连接线
        if len(timestamps) > 1:
            self.ax1.plot(timestamps, y_positions, 'k-', alpha=0.3, linewidth=1)
        
        # 高亮当前点
        if current_transitions:
            current = current_transitions[-1]
            current_y = state_positions[current['to_state']]
            self.ax1.scatter([current['timestamp']], [current_y], 
                           c='red', s=100, marker='o', edgecolors='black', linewidths=2)
        
        self.ax1.set_ylabel('States')
        self.ax1.set_title('State Transitions Timeline')
        self.ax1.set_yticks(range(len(unique_states)))
        self.ax1.set_yticklabels(unique_states)
        self.ax1.grid(True, alpha=0.3)
        
        # 格式化x轴时间
        self.ax1.xaxis.set_major_formatter(mdates.DateFormatter('%H:%M:%S'))
        plt.setp(self.ax1.xaxis.get_majorticklabels(), rotation=45)
        
        # 状态持续时间柱状图
        state_durations = defaultdict(list)
        for trans in current_transitions:
            state_durations[trans['from_state']].append(trans['duration'])
        
        if state_durations:
            states_list = list(state_durations.keys())
            avg_durations = [sum(state_durations[state])/len(state_durations[state]) 
                           for state in states_list]
            
            bars = self.ax2.bar(states_list, avg_durations, 
                               color=[state_colors[state_positions[state]] for state in states_list],
                               alpha=0.7)
            
            self.ax2.set_ylabel('Average Duration (s)')
            self.ax2.set_title('Average State Durations')
            self.ax2.tick_params(axis='x', rotation=45)
            
            # 添加数值标签
            for bar, duration in zip(bars, avg_durations):
                height = bar.get_height()
                self.ax2.text(bar.get_x() + bar.get_width()/2., height,
                             f'{duration:.2f}s', ha='center', va='bottom')
        
        plt.tight_layout()
        self.canvas.draw()
    
    def on_progress_change(self, value):
        if not self.transitions:
            return
            
        max_index = len(self.transitions) - 1
        new_index = int((float(value) / 100.0) * max_index)
        
        if new_index != self.current_index:
            self.current_index = new_index
            self.update_display()
    
    def toggle_play(self):
        if self.is_playing:
            self.stop_animation()
        else:
            self.start_animation()
    
    def start_animation(self):
        if not self.transitions:
            return
            
        self.is_playing = True
        self.play_button.config(text="⏸ Pause")
        self.animate()
    
    def stop_animation(self):
        self.is_playing = False
        self.play_button.config(text="▶ Play")
        if self.animation_id:
            self.root.after_cancel(self.animation_id)
    
    def animate(self):
        if not self.is_playing:
            return
            
        if self.current_index < len(self.transitions) - 1:
            self.current_index += 1
            self.update_display()
            # 根据实际持续时间调整动画速度，但最少100ms
            delay = max(100, int(self.transitions[self.current_index-1]['duration'] * 100))
            self.animation_id = self.root.after(delay, self.animate)
        else:
            self.stop_animation()
    
    def reset_animation(self):
        self.stop_animation()
        self.current_index = 0
        self.update_display()

def main():
    root = tk.Tk()
    app = FSMVisualizer(root)
    
    # 自动加载默认日志文件（如果存在）
    default_log = "/root/Auto-Filmer/log/aflog-20250604-202030.log"
    try:
        app.parse_log_file(default_log)
        app.status_label.config(text=f"Loaded: {len(app.transitions)} transitions")
        app.play_button.config(state='normal')
        app.reset_button.config(state='normal')
        app.update_display()
    except:
        pass  # 如果默认文件不存在，忽略错误
    
    root.mainloop()

if __name__ == "__main__":
    main()