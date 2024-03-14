import os
import tkinter as tk
from tkinter import messagebox

local = os.getenv('LOCALAPPDATA')
cache_path = local + '\\FiveM\\FiveM.app\\data'

def delete_files(path):
    def on_confirm():
        messagebox.showinfo("Bestätigung", "Die Dateien wurden gelöscht.")
        delete_recursive(path)

    def on_cancel():
        messagebox.showinfo("Abgebrochen", "Das Löschen der Datei wurde abgebrochen.")

    root = tk.Tk()
    root.withdraw()
    result = messagebox.askyesno("Bestätigung", "Sind Sie sicher, dass Sie alle Dateien löschen wollen? '{}'".format(path), parent=root)
    root.destroy()

    if result:
        on_confirm()
    else:
        on_cancel()

def delete_recursive(path):
    for root, dirs, files in os.walk(path, topdown=False):
        for file in files:
            file_path = os.path.join(root, file)
            os.remove(file_path)
        for dir in dirs:
            dir_path = os.path.join(root, dir)
            os.rmdir(dir_path)

delete_files(cache_path)
