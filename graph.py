import tkinter as tk
from tkinter import filedialog, messagebox
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

class TelemetryVisualizerGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("F1 2020 Telemetry Motion Visualizer")
        self.root.geometry("1100x700")

        # Top Control Frame
        control_frame = tk.Frame(root, pady=10)
        control_frame.pack(side=tk.TOP, fill=tk.X)

        load_btn = tk.Button(
            control_frame, 
            text="Load CSV File", 
            command=self.load_csv, 
            font=("Arial", 11, "bold"),
            bg="#007ACC", 
            fg="white", 
            padx=15, 
            pady=5
        )
        load_btn.pack(side=tk.LEFT, padx=20)

        self.status_label = tk.Label(
            control_frame, 
            text="No file loaded.", 
            font=("Arial", 10, "italic")
        )
        self.status_label.pack(side=tk.LEFT, padx=10)

        # Matplotlib Figure with 2 Subplots (Track Trace + G-Force diagram)
        self.fig = plt.Figure(figsize=(10, 6), dpi=100)
        
        # Plot 1: 2D Track Map (X vs Z)
        self.ax_track = self.fig.add_subplot(121)
        self.ax_track.set_title("3D/2D Track Trajectory (World X vs Z)")
        self.ax_track.set_xlabel("World Position X (m)")
        self.ax_track.set_ylabel("World Position Z (m)")
        self.ax_track.grid(True, linestyle="--", alpha=0.5)

        # Plot 2: G-Force Diagram (Lat vs Long)
        self.ax_gforce = self.fig.add_subplot(122)
        self.ax_gforce.set_title("G-Force Diagram (G-Lat vs G-Long)")
        self.ax_gforce.set_xlabel("Lateral G-Force (G)")
        self.ax_gforce.set_ylabel("Longitudinal G-Force (G)")
        self.ax_gforce.grid(True, linestyle="--", alpha=0.5)

        self.fig.tight_layout(pad=3.0)

        # Embed Matplotlib Canvas inside Tkinter GUI
        self.canvas = FigureCanvasTkAgg(self.fig, master=root)
        self.canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=True)

    def load_csv(self):
        file_path = filedialog.askopenfilename(
            title="Select Motion Log CSV",
            filetypes=[("CSV Files", "*.csv"), ("All Files", "*.*")]
        )

        if not file_path:
            return

        try:
            # Read CSV using pandas
            df = pd.read_csv(file_path)

            # Validate expected columns exist
            required_cols = {"WorldPosX", "WorldPosZ", "GForceLat", "GForceLong"}
            if not required_cols.issubset(df.columns):
                messagebox.showerror(
                    "Header Error", 
                    f"CSV missing required columns. Expected: {required_cols}"
                )
                return

            # Clear previous plots
            self.ax_track.clear()
            self.ax_gforce.clear()

            # 1. Plot Track Trajectory (X vs Z map out the circuit from above)
            self.ax_track.plot(df["WorldPosX"], df["WorldPosZ"], color="#E10600", linewidth=1.5, label="Racing Line")
            self.ax_track.set_title("Track Trajectory (World X vs Z)")
            self.ax_track.set_xlabel("World Position X (m)")
            self.ax_track.set_ylabel("World Position Z (m)")
            self.ax_track.axis("equal") # Equal aspect ratio preserves true track proportions
            self.ax_track.grid(True, linestyle="--", alpha=0.5)
            self.ax_track.legend()

            # 2. Plot G-Force Scatter Diagram
            scatter = self.ax_gforce.scatter(
                df["GForceLat"], 
                df["GForceLong"], 
                c=df["GForceLat"].abs() + df["GForceLong"].abs(), 
                cmap="inferno", 
                s=10, 
                alpha=0.6
            )
            self.ax_gforce.set_title("G-Force Diagram")
            self.ax_gforce.set_xlabel("Lateral G (Cornering)")
            self.ax_gforce.set_ylabel("Longitudinal G (Accel / Braking)")
            self.ax_gforce.axhline(0, color="gray", linewidth=0.8)
            self.ax_gforce.axvline(0, color="gray", linewidth=0.8)
            self.ax_gforce.grid(True, linestyle="--", alpha=0.5)

            # Redraw Tkinter Canvas
            self.canvas.draw()
            self.status_label.config(text=f"Loaded: {file_path.split('/')[-1]} ({len(df)} samples)")

        except Exception as e:
            messagebox.showerror("Error", f"Failed to parse file:\n{str(e)}")

if __name__ == "__main__":
    root = tk.Tk()
    app = TelemetryVisualizerGUI(root)
    root.mainloop()