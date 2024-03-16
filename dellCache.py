import os
import urllib.request
import webbrowser
from tkinter import messagebox

local = os.getenv('LOCALAPPDATA')
cache_path = local + '\\FiveM\\FiveM.app\\data\\cache'
version = "1.0.0.2"
remote_version = None

def on_confirm():
    messagebox.showinfo("Bestätigung", "Die Dateien wurden gelöscht.")
    delete_files(cache_path)

def on_cancel():
    messagebox.showinfo("Abgebrochen", "Das Löschen der Datei wurde abgebrochen.")

def on_download():
    webbrowser.open_new("https://github.com/xProxyRed/FiveM-Cache-Clear/releases")

def main():
    global remote_version
    req = urllib.request.Request("https://raw.githubusercontent.com/xProxyRed/FiveM-Cache-Clear/main/version", headers={'Cache-Control': 'no-cache'})
    remote_version = urllib.request.urlopen(req).read().decode("utf-8").strip()
    if remote_version != version:
        result = messagebox.askyesno("Aktualisierung", "Eine neue Version ({}) ist verfügbar. Möchtest du sie jetzt herunterladen?".format(remote_version))
        if result:
            on_download()
        else:
            result = messagebox.askyesno("Bestätigung", "Sind Sie sicher, dass Sie alle Dateien löschen wollen? '{}'".format(cache_path))
            if result:
                on_confirm()
            else:
                on_cancel()
    else:
        result = messagebox.askyesno("Bestätigung", "Sind Sie sicher, dass Sie alle Dateien löschen wollen? '{}'".format(cache_path))
        if result:
            on_confirm()
        else:
            on_cancel()


def delete_files(path):
    for root, dirs, files in os.walk(path, topdown=False):
        for file in files:
            file_path = os.path.join(root, file)
            os.remove(file_path)
        for dir in dirs:
            dir_path = os.path.join(root, dir)
            os.rmdir(dir_path)

main()
