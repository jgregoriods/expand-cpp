import pandas
import matplotlib.pyplot as plt
import matplotlib.figure as mplfig
import matplotlib.backends.backend_tkagg as tkagg
import numpy as np
import tkinter as tk
import os

class App:
    def __init__(self, master):
        self.master = master
        self.figure = mplfig.Figure(figsize=(7, 7), dpi=100)
        self.ax = self.figure.add_subplot(111)
        self.canvas = tkagg.FigureCanvasTkAgg(self.figure, self.master)
        self.canvas.get_tk_widget().grid(row=1, column=2, columnspan=1,
                                         rowspan=10)

        self.toolbar_frame = tk.Frame(self.master)
        self.toolbar_frame.grid(row=12, column=4, columnspan=1)
        self.toolbar = tkagg.NavigationToolbar2Tk(self.canvas,
                                                  self.toolbar_frame)

        self.files = os.listdir('snapshots')
        self.files.sort(key=lambda x: int(x[:-4]), reverse=True)

        self.run_button = tk.Button(master, text='Run', width=10,
                                    command=self.play)
        self.run_button.grid(row=1, column=1)

        self.step_button = tk.Button(master, text='Step', width=10,
                                    command=self.step)
        self.step_button.grid(row=2, column=1)
        
        self.basemap = np.vstack(np.loadtxt('layers/ele.asc',
                                            skiprows=6).astype(float))
        self.basemap[self.basemap == -9999] = np.nan
        self.current = 0

    def plot_model(self, filename):
        sites = pandas.read_csv(f'snapshots/{filename}', header=None).values
        self.ax.cla()
        self.ax.set_facecolor('black')
        self.ax.imshow(self.basemap)
        self.ax.scatter(*zip(*sites), s=0.8, c='yellow')
        self.ax.text(580, 70, filename[:-4], color='white',
                     horizontalalignment='right')
        self.canvas.draw()

    def play(self):
        for filename in self.files:
            self.plot_model(filename)

    def back(self):
        pass

    def step(self):
        self.plot_model(self.files[self.current])
        self.current += 1


def main():
    root = tk.Tk()
    app = App(root)
    root.mainloop()


if __name__ == '__main__':
    main()
