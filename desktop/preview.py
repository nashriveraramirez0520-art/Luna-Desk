import tkinter as tk
import psutil
from main import choose_state

def main():
    window = tk.Tk()
    window.title("Luna Desk — Preview")
    window.geometry("480x320")
    window.resizable(False, False)
    window.configure(bg="#171322")
    character_panel = tk.Frame(window, bg="#241C35", width=200)
    character_panel.pack(side="left", fill="y")
    character_panel.pack_propagate(False)
    character_title = tk.Label(
        character_panel, text="LUNA",
        font=("Segoe UI", 20, "bold"),
        fg="#D9B8FF", bg="#241C35"
    )
    character_title.pack(pady=(35, 15))

    character_placeholder = tk.Label(
        character_panel, text="Character\npreview",
        font=("Segoe UI", 14),
        fg="#B9A9CD", bg="#241C35"
    )
    character_placeholder.pack(pady=45)
    stats_panel = tk.Frame(window, bg="#171322")
    stats_panel.pack(side="right", fill="both", expand=True)
    stats_title = tk.Label(
        stats_panel, text="PC STATUS",
        font=("Segoe UI", 18, "bold"),
        fg="#FFFFFF", bg="#171322"
    )
    stats_title.pack(pady=(30, 20))

    cpu_label = tk.Label(
        stats_panel, text="CPU: -- %",
        font=("Segoe UI", 18),
        fg="#D9B8FF", bg="#171322"
    )
    cpu_label.pack(pady=10)

    ram_label = tk.Label(
        stats_panel, text="RAM: -- %",
        font=("Segoe UI", 18),
        fg="#D9B8FF", bg="#171322"
    )

    state_label = tk.Label(
        character_panel, text="State: waiting",
        font=("Segoe UI", 12),
        fg="#D9B8FF", bg="#241C35"
    )

    state_label.pack(pady=5)
    ram_label.pack(pady=10)
    def update_stats():
        cpu = psutil.cpu_percent(interval=None)
        ram = psutil.virtual_memory().percent

        cpu_label.configure(text=f"CPU: {cpu:.1f} %")
        ram_label.configure(text=f"RAM: {ram:.1f} %")

        state = choose_state({"cpu_percent": cpu})
        state_label.configure(text=f"State: {state}")

        window.after(1000, update_stats)
    psutil.cpu_percent(interval=None)
    window.after(1000, update_stats)

    window.mainloop()

if __name__ == "__main__":
    main()

