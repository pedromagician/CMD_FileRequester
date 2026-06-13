# FileRequester

FileRequester for Windows command line.

Repository: [https://github.com/pedromagician/CMD_FileRequester]

A lightweight Windows console application that displays a native file requester dialog from the command line.
Designed for developers, automation scripts, and retro‑inspired workflows.

## ✨ Overview

FileRequester is a small utility that opens a standard Windows file or directory selection dialog using command‑line arguments.
It supports Open, Save, and Directory modes, customizable titles, filters, default filenames, and more.

The tool returns the selected path to stdout, making it easy to integrate into batch files, PowerShell scripts, or other applications.

## 🚀 Usage Examples

```bat
FileRequester.exe -title "Select a file" -o -path C:\ -ok OK

FileRequester.exe -open -filter "Text|*.txt|All files|*.*"
```

## 🧰 Command‑line Options

| Switch | Description |
|--------|-------------|
| `-help`, `-h`, `-?` | Displays this help information. |
| `-title`, `-t` | Specifies the window title of the requester. |
| `-open`, `-o` | Select an existing file. The file must already exist. |
| `-save`, `-s` | Choose a destination filename. The file does not need to exist. |
| `-directory`, `-d` | Select a directory instead of a file. |
| `-filename`, `-f` | Sets a pre-filled filename in the requester. |
| `-ok` | Custom text for the confirmation button. |
| `-path` | Sets the initial directory (e.g., "D:\"). |
| `-drawersonly`, `-foldersonly` | Removes file selection; directories only. Mostly for Open mode. |
| `-filter` | Defines file type filters using "Description\|\*.ext" format.<br>Example: "Text\|\*.txt\|All files\|\*.\*" |


## 🔧 Integration Examples

Store the selected path into a variable:

```bat
@set "FILE="
@for /f "usebackq delims=" %%A in (`FileRequester.exe -o`) do @set "FILE=%%A"
@echo Selected: %FILE%
```

```bat
@echo|set /p="@set mytmp=">tmp.bat
@filerequester -open>>tmp.bat
@call tmp.bat
@echo %mytmp%
```

## 📜 License

Free to use.
Amiga Rulez!
