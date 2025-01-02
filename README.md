# YouTubeDownloader - consloeApp(only window)
I created a program that wasn't really necessary. 
This program uses the **YT-DLP** and **FFMPEG** libraries for downloading audio or video clips from YouTube. 
I wrote it in terminal format. Of course, **YT-DLP** is much easier to use than this program.
I made it like mini project for study C programming ,idk it's just a student project Haha.
I also give the sauce code of this program.

## Set-up
1. Create Folder 
2. move the .exe file to that Folder you just Create
3. create program shortcut for easy use
4. run program
   -The first run may take a few minutes and there may be a warning from windown protection that it may be a Trojan.
   dont worry you can check the sauce code and build by yourself

## build by yourself
   1. Install [**TDM GCC**](https://jmeubank.github.io/tdm-gcc/)
   2. Open Terminal ( PowerShell , CMD, etc..)
   3. You can use this command to complie C code
         - `gcc "<sauce code file path>" -c -o main`
           
   *__Optional__*
      1. install **IDE or text editor**( Vscode, vistual code, nodepad++, Code::Blocks, etc..)
      2. in case you install VScode
         - install [**TDM GCC**](https://jmeubank.github.io/tdm-gcc/)
         - open extention and install [**C/C++ Extention pack**](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack) after install "**C/C++ Extention pack**" do this
            - Ctrl+Shift+P type `C/C++:Edit COnfigurations (UI)` and click it
            - then set "**Compiler path**" tp `C:\TDM-GCC-64\bin\gcc.exe`
            - then set "**IntelliSense mode**" to `windown-gcc-x64`
            - then set "**C standard**" and "**C++ standard**" to `c17` and `c++17`
      3. Install [**Code Runner**](https://marketplace.visualstudio.com/items?itemName=formulahendry.code-runner) For Easy run
         - After install "**Code Runner**" open Setting(*Ctrl + `*)
         - Find Extention and click it
         - find "**Run Code configuration**"
         - then find "**Run In Terminal**" and click enable
         - then find "**Save File Before Run**" and click enable
           ![image](https://github.com/user-attachments/assets/4e7ce34f-ed2e-4aad-af80-56e8fefc64ec)
         - then open project
         - after open project click at "**Add debug Configuration**"
           ![image](https://github.com/user-attachments/assets/bbd3d678-6bc7-4869-a5fe-1743814080ff)
         - then click at "**C/C++: gcc.exe build and debug active file**"
           ![image](https://github.com/user-attachments/assets/82ebc8a0-38fc-4eb1-aa29-2440d57103b5)
           ![image](https://github.com/user-attachments/assets/06e75b67-861f-4747-9d9f-44eaccc9f9c5)

   

