import time
import tkinter as tk
from datetime import datetime

import psutil

from main import choose_state


BACKGROUND = "#171322"
PANEL = "#241C35"
PURPLE = "#D9B8FF"
WHITE = "#FFFFFF"
MUTED = "#B9A9CD"
GOLD = "#FFD166"


def main():
    window = tk.Tk()
    window.title("Luna Desk — Preview")
    window.geometry("480x320")
    window.resizable(False, False)
    window.configure(bg=BACKGROUND)

    # La navegación se coloca primero para reservar espacio abajo.
    navigation = tk.Frame(window, bg=PANEL, height=40)
    navigation.pack(side="bottom", fill="x")
    navigation.pack_propagate(False)

    character_panel = tk.Frame(window, bg=PANEL, width=200)
    character_panel.pack(side="left", fill="y")
    character_panel.pack_propagate(False)

    tk.Label(
        character_panel, text="LUNA",
        font=("Segoe UI", 20, "bold"),
        fg=PURPLE, bg=PANEL,
    ).pack(pady=(20, 10))

    tk.Label(
        character_panel, text="Character\npreview",
        font=("Segoe UI", 14),
        fg=MUTED, bg=PANEL,
    ).pack(pady=30)

    state_label = tk.Label(
        character_panel, text="State: waiting",
        font=("Segoe UI", 12),
        fg=PURPLE, bg=PANEL,
    )
    state_label.pack(pady=5)

    content = tk.Frame(window, bg=BACKGROUND)
    content.pack(side="right", fill="both", expand=True)

    stats_page = tk.Frame(content, bg=BACKGROUND)
    timer_page = tk.Frame(content, bg=BACKGROUND)

    def show_page(page):
        stats_page.pack_forget()
        timer_page.pack_forget()
        page.pack(fill="both", expand=True)

    def make_label(parent, text, size=18, color=PURPLE):
        return tk.Label(
            parent, text=text,
            font=("Segoe UI", size),
            fg=color, bg=BACKGROUND,
        )

    make_label(stats_page, "PC STATUS", 18, WHITE).pack(
        pady=(20, 15)
    )

    cpu_label = make_label(stats_page, "CPU: -- %")
    cpu_label.pack(pady=8)

    ram_label = make_label(stats_page, "RAM: -- %")
    ram_label.pack(pady=8)

    clock_label = make_label(stats_page, "--:--:--", 16, MUTED)
    clock_label.pack(pady=15)

    make_label(timer_page, "FOCUS TIMER", 18, WHITE).pack(
        pady=(20, 15)
    )

    timer_label = make_label(timer_page, "25:00", 32)
    timer_label.pack(pady=10)

    timer_status = make_label(timer_page, "Ready", 12, MUTED)
    timer_status.pack(pady=5)

    remaining = 25 * 60
    deadline = None

    def toggle_timer():
        nonlocal remaining, deadline

        if deadline is None:
            if remaining <= 0:
                return
            deadline = time.monotonic() + remaining
            start_button.configure(text="Pause")
            timer_status.configure(text="Focus time", fg=MUTED)
        else:
            remaining = max(0, deadline - time.monotonic())
            deadline = None
            start_button.configure(text="Start")
            timer_status.configure(text="Paused", fg=MUTED)

    def reset_timer():
        nonlocal remaining, deadline

        remaining = 25 * 60
        deadline = None
        start_button.configure(text="Start")
        timer_status.configure(text="Ready", fg=MUTED)
        timer_label.configure(text="25:00")

    controls = tk.Frame(timer_page, bg=BACKGROUND)
    controls.pack(pady=10)

    start_button = tk.Button(
        controls, text="Start", command=toggle_timer,
        width=8, bg=PANEL, fg=WHITE,
    )
    start_button.pack(side="left", padx=4)

    tk.Button(
        controls, text="Reset", command=reset_timer,
        width=8, bg=PANEL, fg=WHITE,
    ).pack(side="left", padx=4)

    tk.Button(
        navigation, text="PC Status",
        command=lambda: show_page(stats_page),
        bg=PANEL, fg=WHITE,
    ).pack(side="left", expand=True, fill="both", padx=4, pady=4)

    tk.Button(
        navigation, text="Focus Timer",
        command=lambda: show_page(timer_page),
        bg=PANEL, fg=WHITE,
    ).pack(side="left", expand=True, fill="both", padx=4, pady=4)

    def update_stats():
        cpu = psutil.cpu_percent(interval=None)
        ram = psutil.virtual_memory().percent
        state = choose_state({"cpu_percent": cpu})

        cpu_label.configure(text=f"CPU: {cpu:.1f} %")
        ram_label.configure(text=f"RAM: {ram:.1f} %")
        clock_label.configure(
            text=datetime.now().strftime("%H:%M:%S")
        )
        state_label.configure(
            text=f"State: {state}",
            fg=GOLD if state == "working" else PURPLE,
        )

        window.after(1000, update_stats)

    def update_timer():
        nonlocal remaining, deadline

        if deadline is not None:
            remaining = max(0, deadline - time.monotonic())

            if remaining <= 0:
                deadline = None
                start_button.configure(text="Start")
                timer_status.configure(text="Session complete!", fg=GOLD)

        seconds = int(remaining)
        if remaining > seconds:
            seconds += 1

        minutes, seconds = divmod(seconds, 60)
        timer_label.configure(text=f"{minutes:02d}:{seconds:02d}")

        window.after(100, update_timer)

    show_page(stats_page)
    psutil.cpu_percent(interval=None)
    window.after(1000, update_stats)
    update_timer()
    window.mainloop()


if __name__ == "__main__":
    main()

