# Music-Player-Playlist-C

This is a terminal-based Music Player built in C, showcasing the use of Data Structures (Doubly Linked List), File Handling, and Windows Multimedia API.
    The player allows users to manage a playlist, play .wav files, and navigate between songs using a menu-driven interface.

✨ Features

    📂 Playlist Management
        Add songs
        Remove songs
        Search songs
        Display all songs

    ▶️ Playback Controls
        Play selected song
        Stop music
        Next / Previous song navigation

    💾 File Handling
        Save playlist automatically to playlist.txt
        Load playlist from file when program restarts

    ⚡Technical Details
        Uses doubly linked lists for playlist navigation
        Dynamic memory management (malloc, free)
        Windows API (PlaySound) for asynchronous playback


🛠️ Technologies Used

        Language: C
        Concepts: Data Structures (Doubly Linked List), File Handling        
        Libraries Used:        
           <string.h> for string operations     
           <mmsystem.h> for audio playback (Windows)      
           <windows.h> for system-level functions
        

📖 How It Works

1. Run the program in a Windows environment (since it uses PlaySound).
        # Compile (on Windows using GCC / MinGW)
            gcc music_player.c -o music_player -lwinmm
        # Run
            music_player.exe

2. Add songs by entering the .wav file names (make sure they are in the  same folder as the program or provide full path).

3. Use the menu to play, stop, search, or navigate songs.

4. Playlist is automatically saved to playlist.txt and reloaded on the next run.

📌 Future Improvements

 -> Add support for more audio formats (e.g., MP3).
 -> Build a GUI version with graphics libraries.
 -> Implement shuffle and repeat modes.


